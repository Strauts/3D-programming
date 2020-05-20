#include "octahedron.h"
#include "cmath"
#include "vertex.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "renderwindow.h"
#include "shader.h"
#include "camera.h"


Octahedron::Octahedron()
{

}

Octahedron::Octahedron(RenderWindow *Owner, int n) : SceneObject(Owner),
                                                                                            mRecursions(n), mIndex(0)
{
    mNumberOfVertices = 3 * 8 * pow(8, mRecursions);
    mVertices = new Vertex[mNumberOfVertices];
    makeUnitOctahedron();
}

Octahedron::~Octahedron()
{
    delete[] mVertices;
}

void Octahedron::createObject()
{
    initializeOpenGLFunctions();

    //Creating the Shader
    mShader = new Shader("../3D-programming/plainvertex.vert",
                         "../3D-programming/plainfragment.frag");

    //Generate Vertex Array Object
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    //Generate Vertex Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices * sizeof(Vertex), vertices(), GL_DYNAMIC_DRAW);

    //Tell the GPU how to read Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Tell the GPU how to read colours
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Tell the GPU how to read UV positions
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void Octahedron::drawObject()
{
    //Tell OpenGL to use this shader
    glUseProgram(mShader->getProgram());

    //Tell OpenGL to use this VAO
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();

    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();

    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();
}

void Octahedron::makeTriangle(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
    mVertices[mIndex].set_xyz(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_normal(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_st(0.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_normal(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_st(1.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_normal(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_st(0.5f, 1.0f);
    ++mIndex;
}

void Octahedron::subDivide(const Vec3 &a, const Vec3 &b, const Vec3 &c, int n)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        makeTriangle(a, b, c);
    }
}

void Octahedron::makeUnitOctahedron()
{
    Vec3 v0(0, 0, 1);
    Vec3 v1(1, 0, 0);
    Vec3 v2(0, 1, 0);
    Vec3 v3(-1, 0, 0);
    Vec3 v4(0, -1, 0);
    Vec3 v5(0, 0, -1);

    subDivide(v0, v1, v2, mRecursions);
    subDivide(v0, v2, v3, mRecursions);
    subDivide(v0, v3, v4, mRecursions);
    subDivide(v0, v4, v1, mRecursions);
    subDivide(v5, v2, v1, mRecursions);
    subDivide(v5, v3, v2, mRecursions);
    subDivide(v5, v4, v3, mRecursions);
    subDivide(v5, v1, v4, mRecursions);
}
