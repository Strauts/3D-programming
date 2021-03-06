#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material
{
public:
    Material();

    void setShader(class Shader *shader);
    void setTextureUnit(const GLuint &textureUnit);
    void setColor(const Vec3 &color);

    Vec3 mObjectColor{1.f, 1.f, 1.f};
    GLuint mTextureUnit{0};     //the actual texture to put into the uniform
    Shader *mShader{nullptr};
};

#endif // MATERIAL_H
