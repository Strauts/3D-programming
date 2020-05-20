#include "plane.h"
#include "shader.h"
#include "renderwindow.h"
#include "camera.h"
#include "vertex.h"
#include "light.h"
#include "quaternion.h"

Plane::Plane()
{
}

Plane::Plane(RenderWindow *Owner, Vec3 Position, Quaternion Rotation)
    : SceneObject(Owner)
{
    transform.setPosition(Position);
    transform.setRotation(Rotation);

    mNumberOfVertices = 6;
    mVertices = new Vertex[mNumberOfVertices]
    {
        {Vec3(-0.5f, 0.f, -0.5f), Vec3(1.f, 0.f, 0.f), Vec2(),},
        {Vec3(-0.5f, 0.f,  0.5f), Vec3(0.f, 1.f, 0.f), Vec2(),},
        {Vec3( 0.5f, 0.f, -0.5f), Vec3(0.f, 0.f, 1.f), Vec2(),},

        {Vec3( 0.5f, 0.f, -0.5f), Vec3(0.f, 0.f, 1.f), Vec2(),},
        {Vec3(-0.5f, 0.f,  0.5f), Vec3(0.f, 1.f, 0.f), Vec2(),},
        {Vec3( 0.5f, 0.f,  0.5f), Vec3(1.f, 0.f, 0.f), Vec2(),}
    };
}

Plane::~Plane()
{

}

void Plane::createObject()
{
    initializeOpenGLFunctions();

    mShader = new Shader ("../3D-programming/plainvertex.vert",
                          "../3D-programming/plainfragment.frag");

    //Create VAO
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    //Create Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);

    glBufferData(GL_ARRAY_BUFFER,
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(),
                 GL_STATIC_DRAW);
    //Tell GPU how to read vertices
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)0);

    glEnableVertexAttribArray(0);

    //Tell GPU how to read colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(1);

    //Tell GPU how to read UV
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));

    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void Plane::drawObject()
{
    //Tell the GPU how to use a shader
    glUseProgram(mShader->getProgram());

    //Read our vertex data
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    //Load the matrix into the shader
    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();

}

Vec3 Plane::getNormal()
{
    return (getModelMatrix() * Vec4(0.f, 1.f, 0.f, 0.f)).toVector3D();
}
