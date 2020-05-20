#include "billboard.h"
#include "vertex.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "renderwindow.h"
#include "camera.h"

Billboard::Billboard()
{
//    mNumberOfVertices = 4;
//    mVertices = new Vertex[mNumberOfVertices]
//    {
//        //Quad as triangle strip
//        // Positions                    // Colors not used          //UVs not used
//        {Vec3{-0.5f, -0.5f, 0.0f},      Vec3{1.0f, 0.0f, 0.0f},     Vec2{0.f, 0.f}},  // down-left
//        {Vec3{0.5f, -0.5f, 0.0f},      Vec3{1.0f, 0.0f, 0.0f},     Vec2{0.f, 0.f}},  // down-right
//        {Vec3{-0.5f,  0.5f, 0.0f},      Vec3{1.0f, 0.0f, 0.0f},     Vec2{0.f, 0.f}},  // up-left
//        {Vec3{0.5f,  0.5f, 0.0f},      Vec3{1.0f, 0.0f, 0.0f},     Vec2{0.f, 0.f}},  // up-right
//    };
//    mModelMatrix = std::unique_ptr<Matrix4x4>(new Matrix4x4{true});
}

Billboard::Billboard(RenderWindow *Owner, Vec3 Position, Quaternion Rotation)
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

Billboard::~Billboard()
{
    delete[] mVertices;
}

void Billboard::createObject()
{
    initializeOpenGLFunctions();

    //Creating the Shader
    mShader = new Shader("textureshader.vert",
                                           "textureshader.frag");

    //Generate Vertex Array Object
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    //Generate Vertex Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices * sizeof(Vertex), vertices(), GL_STATIC_DRAW);

    //Tell the GPU how to read buffer positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Tell the GPU how to read colours
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Tell the GPU how to read UV positions
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mTextureUniforms[0] = glGetUniformLocation(mShader->getProgram(), "textureSampler1");
    mTextureUniforms[1] = glGetUniformLocation(mShader->getProgram(), "textureSampler2");

    mTextures[0] = new Texture("hund.bmp");
    mTextures[1] = new Texture("uvtemplate.bmp");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTextures[1]->id());

    mMMatrixUniform = glGetUniformLocation(mShader->getProgram(), "mMatrix");
    mVMatrixUniform = glGetUniformLocation(mShader->getProgram(), "vMatrix");
    mPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "pMatrix");
}

void Billboard::drawObject()
{
    //Tell OpenGL to use this shader
    glUseProgram(mShader->getProgram());

    //Tell OpenGL to use this VertexArrayObject
    glBindVertexArray(mVertexArrayObject);

    glUniform1i(mTextureUniforms[0], 0);
    glUniform1i(mTextureUniforms[1], 1);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();

    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();

    Matrix4x4 Model = getModelMatrix();

    glUniformMatrix4fv(mPMatrixUniform, 1, GL_TRUE, Projection.constData());
    glUniformMatrix4fv(mVMatrixUniform, 1, GL_TRUE, View.constData());
    glUniformMatrix4fv(mMMatrixUniform, 1, GL_TRUE, Model.constData());

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();
}


