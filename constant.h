#ifndef CONSTANT_H
#define CONSTANT_H
#include <string>

namespace gsl   //Game School Lib
{
    const std::string assetFilePath{"../3D-programming/assets/"};
    const std::string shaderFilePath{"../3D-programming/"};

    typedef unsigned int* triangle;

    enum ShaderTypes
    {
        COLOR,
        TEXTURE,
        COLORLIGHT,
        TEXTURELIGHT
    };
}


#endif // CONSTANT_H
