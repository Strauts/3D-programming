#include "triangle.h"
#include "vertex.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "shader.h"
#include "renderwindow.h"
#include "transform.h"
#include "camera.h"

Triangle::Triangle(RenderWindow* Owner, Vec3 Position, Quaternion Rotation)
    : SceneObject(Owner)
{
    getTransform().setPosition(Position);
    getTransform().setRotation(Rotation);
    mVertices = new Vertex[3]
    {
            // Positions                    // Colors                   //UVs
        {Vec3{-0.5f, -0.5f, 0.0f},       Vec3{1.0f, 0.0f, 0.0f},     Vec2{ 0.f, 0.f}},  // Bottom Left
        {Vec3{ 0.5f, -0.5f, 0.0f},       Vec3{0.0f, 1.0f, 0.0f},     Vec2{ 1.f, 0.f}}, // Bottom Right
        {Vec3{ 0.0f,  0.5f, 0.0f},       Vec3{0.0f, 0.0f, 1.0f},     Vec2{0.5f, 1.f}}   // Top
    };

    mNumberOfVertices = 3;
}

Triangle::Triangle()
{

}

Triangle::~Triangle()
{
    delete[] mVertices;
}

void Triangle::createObject()
{
    initializeOpenGLFunctions();

    mShader = new Shader("../3D-programming/plainvertex.vert",
                         "../3D-programming/plainfragment.frag");

    // Generer Vertex Array Objekt
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Generer Vertex Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(), // &mVertices[0]
                 GL_STATIC_DRAW);

    // Fortell GPU hvordan man leser posisjoner fra buffer
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Fortell GPU hvordan man leser farger fra buffer
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Fortell GPU hvordan man leser uv fra buffer
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void Triangle::drawObject()
{
    // Tell OpenGL to use this shader
    glUseProgram(mShader->getProgram());




    // Tell OpenGL to use this VertexArrayObject
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();
}

void Triangle::update(float deltaTime)
{
    float speed = 2.f;
    float rad = speed*deltaTime;

    Vec3 axis = Vec3(0.f, 1.f, 0.f);

    Quaternion rotation(rad, axis);

    transform.addRotation(rotation);
}
