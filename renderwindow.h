#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include "matrix4x4.h"
#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>
#include <memory>
#include <QSet>
#include "curvedplane.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class Triangle;
class SceneObject;
class Texture;
class Matrix4x4;
class QMatrix4x4;
class Camera;
class Light;
class trophie;
class constant;
class Octahedron;
class SkyBox;

class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow();
    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;
    void setWireframe(bool wireframe);
    void checkForGLerrors();
    void resetGame();
    void createGame();
    void collectTrhopie(trophie* object);

    // Try to refractor out
    curvedPlane* cp {nullptr};

    Light* mLight;
    Camera* getCamera();
    Light* getLight();
    bool smooth = false;
    bool lastCheck;
    int numObjects = -1;
    int numTrophies = -1;
    int collectedTrophies = 0;


signals:
    void ready();
    void error(const QString &msg);

private slots:
    void render();

private:
    void init();
    void startOpenGLDebugger();
    Vec3 mCamPos;
    QOpenGLContext *mContext;
    bool mInitialized;
    std::unique_ptr<Shader> mTextureShaderProgram;
    std::unique_ptr<Shader> mColorShaderProgram;

    GLint mMVPUniform;
    GLuint mTriangleVAO;
    GLuint mVertexBufferObject;

    const float cameraVelocity = 0.1f;
    float mDeltaTime;
    float mPosition;
    float mRotation;
    float mDistance = 50.0f;
    float phi = 0.0f;
    float theta = 0.0f;
    int trophiesCollected = 0;
    bool checkCamera = false;

    std::vector<SceneObject*> objects;
    std::vector<trophie*> trophies;
    std::unique_ptr<SceneObject> mBillboard;
    GLuint mBillboardVAO;
    GLuint mBillboardBufferObject;


    Camera* mMainCamera;
    Camera* mPlayerCamera;
    Camera* mEditorCamera;
    SkyBox* mSkyBox;
    QBasicTimer mTimer;     //timer that drives the gameloop
    QTime mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    std::unique_ptr<Texture> mTexture1;
    GLuint mTextureUniform1;
    std::unique_ptr<Texture> mTexture2;
    GLuint mTextureUniform2;

    std::unique_ptr<Matrix4x4> mViewMatrix;
    std::unique_ptr<Matrix4x4> mProjectionMatrix;
    GLint mModelMatrixUniform;
    GLint mViewMatrixUniform;
    GLint mProjectionMatrixUniform;

    Octahedron* tex;

    Matrix4x4 mMVPmatrix;

    bool mOrtho{false};
    bool mWireframe{false};
    bool doorOpen;
    int mouseXlast{0};
    int mouseYlast{0};
    int renderLoop{0};

    QSet<int> pressedKey;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
