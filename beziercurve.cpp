#include "beziercurve.h"
#include "vec2.h"
#include "vec3.h"
#include "camera.h"
#include "shader.h"
#include "vertex.h"
#include "renderwindow.h"
#include "matrix4x4.h"

BezierCurve::BezierCurve()
{}

BezierCurve::BezierCurve(RenderWindow *Owner, Vec3 a, Vec3 b, Vec3 c, Vec3 d)
    : SceneObject(Owner)
{
    mA = a;
    mB = b;
    mC = c;
    mD = d;

    mNumControlPointVertices = 4;
    mControlPointVertices = new Vertex[mNumControlPointVertices]
    {
        {a, Vec3(1.f, 0.f, 1.f), Vec2()},
        {b, Vec3(1.f, 0.f, 1.f), Vec2()},
        {c, Vec3(1.f, 0.f, 1.f), Vec2()},
        {d, Vec3(1.f, 0.f, 1.f), Vec2()}
    };

    mNumberOfVertices = 20;
    mVertices = new Vertex[mNumberOfVertices];

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i] = {evaluateBezier(3, (float)i/(mNumberOfVertices-1)), Vec3(1.f, 0.f, 0.f), Vec2()};
    }

}

void BezierCurve::createObject()
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
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(),
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

    //Create VAO for control points
    glGenVertexArrays(1, &mVertexArrayObject2);
    glBindVertexArray(mVertexArrayObject2);

    glGenBuffers(1, &mVertexBufferObject2);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject2);
    glBufferData(GL_ARRAY_BUFFER, mNumControlPointVertices*sizeof(Vertex), mControlPointVertices, GL_STATIC_DRAW);

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


}
void BezierCurve::drawObject()
{
    // Tell the GPU to use a shader
    glUseProgram(mShader->getProgram());

    std::string name = "ambient";
    glUniform4f(glGetUniformLocation(mShader->getProgram(), name.c_str()), 0.8f, 0.1, 0.4f, 1.0f);

    // Read our vertex data
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    // Load the matrix into the shader
    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, mNumberOfVertices);

    glBindVertexArray(mVertexArrayObject2);
    glDrawArrays(GL_LINE_STRIP, 0, mNumControlPointVertices);

    mOwner->checkForGLerrors();
}

Vec3 BezierCurve::getControllPoint(int in)
{
    return controllPoint[in];
}

Vec3 BezierCurve::getA()
{
    return mA;
}

Vec3 BezierCurve::getB()
{
    return mB;
}

Vec3 BezierCurve::getC()
{
    return mC;
}

Vec3 BezierCurve::getD()
{
    return mD;
}

Vec3 BezierCurve::evaluateBezier(int degree, float t)
{
    Vec3 c[4]{mA, mB, mC, mD};

    for(int k = 1; k <= degree; k++)
    {
        for(int i = 0; i < degree - k +1; i++)
            c[i] = c[i] * (1 - t) + c[i + 1] * t;
    }
    return c[0];
}
