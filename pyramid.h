#ifndef PYRAMID_H
#define PYRAMID_H

#include "sceneobject.h"
#include "shader.h"
#include "texture.h"

//Pyramid class inhereting from SceneObject with OpenGL functions to be used in the create and draw object functions
class Pyramid : public SceneObject
{
public:
    Pyramid();
    Pyramid(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    ~Pyramid();

    void createObject() override;
    void drawObject() override;

private:
    Shader* mShader;

    //Texture variable Arrays to get the dog and uvTemplate textures on the object
    Texture* mTextures[2];

    GLuint mTextureUniforms[2];
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMMatrixUniform;
    GLuint mVMatrixUniform;
    GLuint mPMatrixUniform;
};

#endif // PYRAMID_H
