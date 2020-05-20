#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <memory>
#include "shader.h"
#include "mainwindow.h"
#include "matrix4x4.h"
#include "triangle.h"
#include "octahedron.h"
#include "plane.h"
#include "texture.h"
#include "vertex.h"
#include "camera.h"
#include "ball.h"
#include "beziercurve.h"
#include "beziercurvesmooth.h"
#include "curvedplane.h"
#include "light.h"
#include "trophie.h"
#include "enemy.h"
#include "constant.h"
#include "billboard.h"
#include "camera.h"
#include "skybox.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext{0}, mInitialized{false}, mPosition{0.0f}, mRotation{0.f}, mMainWindow{mainWindow},
      mDeltaTime{0.016f}

{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);    //automatically deleted
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = 0;
    }
}

RenderWindow::~RenderWindow()
{
    glDeleteBuffers( 1, &mVertexBufferObject );
    for(auto obj : objects)
        delete obj;
}

void RenderWindow::init()
{   
    mLight = new Light(this, Vec3(0.f, 10.f, 0.f), 10.f, Vec3(1.f, 1.f, 1.f), 4.f, Vec3(1.f, 1.f, 1.f));

    mPlayerCamera = new Camera(this,
                               Vec3(0.0f, 0.0f, 0.0f),
                               Vec3(0.0f, 2.0f, 0.0f),
                               45.f,
                               0.01f,
                               1000.f);

    mEditorCamera = new Camera(this,
                               Vec3(0.0f, 0.0f, 0.0f),
                               Vec3(0.0f, 2.0f, 0.0f),
                               45.f,
                               0.01f,
                               1000.f);


    mMainCamera = mEditorCamera;

    mCamPos = Vec3(-0.00807742, 0.717356, 0.69666);

    createGame();

    if (!mContext->makeCurrent(this)) {
        emit error(tr("makeCurrent() failed"));
        return;
    }

    if (!mInitialized)
        mInitialized = true;

    startOpenGLDebugger();

    initializeOpenGLFunctions();
    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    glDisable(GL_CULL_FACE);     //draws only front side of models
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT


    mLight->createObject();
    for(auto obj : objects)
        obj->createObject();

    for(auto tro : trophies)
        tro->createObject();

    emit ready();   //tell the mainWindow that init is finished


}

void RenderWindow::render()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();

    if (mWireframe)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    //turn on wireframe mode
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    //turn off wireframe mode

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUniform1i(mTextureUniform1, 0);
    glUniform1i(mTextureUniform2, 1);

    mLight->drawObject();

    for(auto obj : objects)
        obj->drawObject();


    for(auto tro : trophies)
        tro->drawObject();
    mLight->update(mDeltaTime);

    for(auto obj1: objects)
    {
        for(auto obj2: objects)
        {
            obj1->collisionCheck(obj2);
        }
    }

    for(auto obj : objects)
        obj->collisionCheck(obj);

    objects[0]->collisionCheck(objects[1]);
    objects[0]->collisionCheck(trophies[0]);
    objects[0]->collisionCheck(trophies[1]);
    objects[0]->collisionCheck(trophies[2]);
    objects[0]->collisionCheck(trophies[3]);



    for(auto obj : objects)
        obj->update(mDeltaTime);


    // Update camera
    //    mCamPos.setX(-std::cos(phi)*std::sin(theta));
    //    mCamPos.setY(-std::sin(phi));
    //    mCamPos.setZ(-std::cos(theta)*std::cos(phi));


    //std::cout << mCamPos << std::endl;

    mMainCamera->getTransform().setPosition(mCamPos * mDistance);

    mContext->swapBuffers(this);

    if(checkCamera == true)
    {
        for(auto i = pressedKey.begin(); i != pressedKey.end(); i++)
        {
            switch(*i)
            {
            case Qt::Key_W :
                static_cast<Ball*>(objects[0])->addVelocity(Vec3(0.f, 0.f, -0.1f));
                break;
            case Qt::Key_S :
                static_cast<Ball*>(objects[0])->addVelocity(Vec3(0.f, 0.f, 0.1f));
                break;
            case Qt::Key_A :
                static_cast<Ball*>(objects[0])->addVelocity(Vec3(-0.1f, 0.f, 0.f));
                break;
            case Qt::Key_D :
                static_cast<Ball*>(objects[0])->addVelocity(Vec3(0.1f, 0.f, 0.f));
                break;
            }
        }
    }
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);  //will be deleted automatically
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
            //Try to silence some messages that is not critical
            mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity);
        }
    }
}

void RenderWindow::setWireframe(bool wireframe)
{
    mWireframe = wireframe;
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "glGetError returns " << err;
        }
    }
}

void RenderWindow::resetGame()
{
    objects.clear();
    trophies.clear();
    init();
}

void RenderWindow::createGame()
{
    numObjects = -1;
    numTrophies = -1;

    // Objects[0] - Ball
    objects.push_back(static_cast<SceneObject*>(new Ball(this,
                                                         Vec3(0.0f, 10.0f, 2.0f),
                                                         Quaternion(1.f, Vec3(0.0f, 0.0f, 0.0f))))); numObjects++;
    objects[0]->getTransform().setScale(Vec3(1.f, 1.f, 1.f));
    objects[0]->getTransform().setPosition(Vec3(0.0f, 1.5f, 0.0f));

    // Objects[1] - Enemy
    objects.push_back(static_cast<SceneObject*>(new Enemy(this,
                                                          Vec3( 5.0f, 2.0f,  5.0f),
                                                          Vec3( 10.0f, 2.0f, -14.0f),
                                                          Vec3(-10.0f, 2.0f, -14.0f),
                                                          Vec3(-5.0f, 2.0f,  5.0f),
                                                          3))); numObjects++;

    objects.push_back(static_cast<Billboard*>(new Billboard(this,
                                                            Vec3(0.f, 5.5f, -15.f),
                                                            Quaternion(1.0f, Vec3(1.f, 0.f, 0.f)))));
    numObjects++;
    objects[2]->getTransform().setScale(Vec3(8.f, 8.f, 8.f));


    // Object[2] - Curved plane
    cp = new curvedPlane(this,
                         Vec3(0.0f, 0.0f, 0.0f),
                         40,      // Width
                         40,      // Height
                         -20,      // Start x
                         -20,      // Start z
                         2);   // Distance between each triangle

    objects.push_back(static_cast<SceneObject*>(cp));
    numObjects++;
    // objects.push_back(static_cast<SceneObject*>(mEditorCamera)); numObjects++;


    //Trophies//
    // Trophy 1 - Object[3]
    trophies.push_back(static_cast<trophie*>(new trophie(this,
                                                         Vec3(5.0f, 2.0f, 5.0f),         // Position
                                                         Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f))))); // Rotation
    numTrophies++;
    trophies[numTrophies]->getTransform().setScale(Vec3(2.f, 2.f, 2.f));
    // Trophy 2 - Object[]
    trophies.push_back(static_cast<trophie*>(new trophie(this,
                                                         Vec3(-5.0f, 2.0f, 5.0f),         // Position
                                                         Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f))))); // Rotation

    numTrophies++;
    trophies[numTrophies]->getTransform().setScale(Vec3(2.f, 2.f, 2.f));
    // Trophy 3 - Object[]
    trophies.push_back(static_cast<trophie*>(new trophie(this,
                                                         Vec3(5.0f, 2.0f, -5.0f),         // Position
                                                         Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f))))); // Rotation

    numTrophies++;
    trophies[numTrophies]->getTransform().setScale(Vec3(2.f, 2.f, 2.f));
    // Trophy 4 - Object[]
    trophies.push_back(static_cast<trophie*>(new trophie(this,
                                                         Vec3(-5.0f, 2.0f, -5.0f),         // Position
                                                         Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f))))); // Rotation
    numTrophies++;
    trophies[numTrophies]->getTransform().setScale(Vec3(2.f, 2.f, 2.f));

    // Trophy 5 - Object[]
    trophies.push_back(static_cast<trophie*>(new trophie(this,
                                                         Vec3(-5.0f, -20.0f, -5.0f),         // Position
                                                         Quaternion(1.0f, Vec3(0.0f, 0.0f, 0.0f))))); // Rotation
    numTrophies++;
    trophies[numTrophies]->getTransform().setScale(Vec3(2.f, 2.f, 2.f));
}


void RenderWindow::collectTrhopie(trophie *object)
{

}

Camera *RenderWindow::getCamera()
{
    return mMainCamera;
}

Light *RenderWindow::getLight()
{
    return mLight;
}

//Called from Qt when window is exposed / shown
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    if (isExposed())
    {
        mTimer.start(16, this);
        mTimeStart.start();
    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mMainCamera == mEditorCamera)
    {
        //using mouseXYlast as deltaXY so we don't need extra variables
        mouseXlast = event->pos().x() - mouseXlast;
        mouseYlast = event->pos().y() - mouseYlast;

        //if delta is to big the movement will be jerky
        //Happens if mouse is moved much between presses.
        if (mouseXlast > 40 || mouseYlast > 40 || mouseXlast < -40 || mouseYlast < -40)
        {
            mouseXlast = 0;
            mouseYlast = 0;
        }
        if(event->buttons() == Qt::LeftButton)
        {
            // Rotate camera
            float cameraSpeed{0.01f};
            theta -= mouseXlast*cameraSpeed;
            phi -= mouseYlast*cameraSpeed;

            mCamPos.setX(-std::cos(phi)*std::sin(theta));
            mCamPos.setY(-std::sin(phi));
            mCamPos.setZ(-std::cos(theta)*std::cos(phi));
        }
        mouseXlast = event->pos().x();
        mouseYlast = event->pos().y();
    }
    else if(mMainCamera == mPlayerCamera)
    {
        mCamPos = Vec3(-0.00807742, 0.717356, 0.69666);
        mMainCamera->getTransform().setPosition(mCamPos * mDistance);
    }

}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    //http://doc.qt.io/qt-4.8/qwheelevent.html
    int numDegrees = event->delta() / 8;
    float numSteps = numDegrees / 15;
    mDistance -= numSteps;
    event->accept();
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();
    }
    if(event->key() == Qt::Key_N)
    {
        smooth = !smooth;
    }
    if(event->key() == Qt::Key_G)
    {
        mMainCamera = mPlayerCamera;
        checkCamera = true;
    }
    if(event->key() == Qt::Key_H)
    {
        mMainCamera = mEditorCamera;
        checkCamera = false;
    }

    pressedKey += event->key();
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    pressedKey -= event->key();
}

void RenderWindow::timerEvent(QTimerEvent *)
{
    render();

    //The rest here is just to show the frame rate:
    int msSinceLastFrame = mTimeStart.restart();    //restart() returns ms since last restart.

    mDeltaTime = msSinceLastFrame/1000.f;

    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 60) //once pr 60 frames =  update the message once pr second
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" FrameDraw: " + QString::number(msSinceLastFrame) + " ms");
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}
