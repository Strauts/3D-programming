#include "beziercurvesmooth.h"
#include "beziercurve.h"
#include "vec2.h"
#include "vec3.h"
#include "camera.h"
#include "shader.h"
#include "vertex.h"
#include "renderwindow.h"
#include "matrix4x4.h"

BezierCurveSmooth::BezierCurveSmooth()
{}

BezierCurveSmooth::BezierCurveSmooth(RenderWindow *Owner, BezierCurve *curve, int smoothValue)
    : SceneObject(Owner)
{
    controllPoint[0] = curve->getA();
    controllPoint[1] = curve->getB();
    controllPoint[2] = curve->getC();
    controllPoint[3] = curve->getD();
    mSmoothVal = smoothValue;
    for(int i = 0; i < mSmoothVal; i ++)
    {
        Vec3 vec3temp = evaluateBezier(3, (1.f/mSmoothVal)*i);   // 3 is for n degree in the curve
        mvtx[i].set_xyz(vec3temp);
        mvtx[i].set_normal(0.0f, 1.0f, 0.0f);
        if(i == mSmoothVal - 1)
            mvtx[i+1].set_xyz(controllPoint[3]);
    }
    mSmoothVal++;
}

void BezierCurveSmooth::createObject()
{
    initializeOpenGLFunctions();

    mShader = new Shader("../3D-programming/plainvertex.vert",
                         "../3D-programming/plainfragment.frag");
    // Gen VAO
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Gen Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 mSmoothVal*sizeof(Vertex),
                 mvtx,
                 GL_STATIC_DRAW);

    // Tell our GPU how to read positions
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell our GPU how to read colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Tell our GPU how to read uv's
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void BezierCurveSmooth::drawObject()
{
    // Tell the GPU to use a shader
    glUseProgram(mShader->getProgram());

    // Read our vertex data
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    // Load the matrix into the shader
    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, mSmoothVal);

    mOwner->checkForGLerrors();
}


Vec3 BezierCurveSmooth::getControllPoint(int in)
{
    return controllPoint[in];
}

Vec3 BezierCurveSmooth::evaluateBezier(int degree, float t)
{
    Vec3 c[4];
    for(int i = 0; i < 4; i++)
        c[i] = controllPoint[i];

    for(int k = 1; k <= degree; k++)
    {
        for(int i = 0; i < degree - k +1; i++)
            c[i] = c[i] * (1 - t) + c[i + 1] * t;
    }
    return c[0];
}
