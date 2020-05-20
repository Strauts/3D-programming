#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "sceneobject.h"
#include "texture.h"
#include "shader.h"
#include "triangle.h"

class Billboard : public SceneObject
{
public:
    Billboard();
    Billboard(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    ~Billboard();

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

#endif // BILLBOARD_H
