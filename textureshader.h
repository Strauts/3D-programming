#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"

class TextureShader : public Shader
{
public:
    TextureShader(const GLchar *vertexPath, const GLchar *fragmentPath/*, const GLchar *geometryPath = nullptr*/);

    //void transmitUniformData(Matrix4x4 *modelMatrix, Material *material) override;

private:
    GLint objectColorUniform{-1};
    GLint textureUniform{-1};
};

#endif // TEXTURESHADER_H
