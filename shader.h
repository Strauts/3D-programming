#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "vec3.h"
#include "vec4.h"
#include "matrix4x4.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//#include "camera.h"

#include <QOpenGLFunctions_4_1_Core>

//#include "GL/glew.h" //We use QOpenGLFunctions instead

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h

class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath );
    ~Shader();
    // Uses the current shader
    void Use( );
    GLuint getProgram() const;

//    virtual void transmitUniformData(Matrix4x4 *modelMatrix, class Material *material = nullptr);

//    void setCurrentCamera(Camera *currentCamera);

//    Camera *getCurrentCamera() const;

    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, Vec3 value);
    void setVec4(const std::string &name, const Vec4 &value);
    void setVec4(const std::string &name, const float x, float y, float z, float w = 1.0f);

private:
  //  Camera *mCurrentCamera{nullptr};

    GLuint program;


    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};
};

#endif
