#include "shader.h"
#include "vec3.h"
#include "vec4.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead


Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions ( std::ifstream::badbit );
    fShaderFile.exceptions ( std::ifstream::badbit );
    try
    {
        // Open files
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // close file handlers
        vShaderFile.close( );
        fShaderFile.close( );
        // Convert stream into string
        vertexCode = vShaderStream.str( );
        fragmentCode = fShaderStream.str( );
    }
    catch ( std::ifstream::failure e )
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    program = glCreateProgram( );
    glAttachShader(program, vertex );
    glAttachShader(program, fragment );
    glLinkProgram(program );
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader( vertex );
    glDeleteShader( fragment );

}

Shader::~Shader()
{
    //std::cout << "Shader deleted\n";
}

void Shader::Use()
{
    glUseProgram(program );
}

GLuint Shader::getProgram() const
{
    return program;
}

//void Shader::transmitUniformData(Matrix4x4 *modelMatrix, Material *material)
//{
//    glUniformMatrix4fv( vMatrixUniform, 1, GL_TRUE, mCurrentCamera->mViewMatrix.constData());
//    glUniformMatrix4fv( pMatrixUniform, 1, GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, modelMatrix->constData());
//}

//void Shader::setCurrentCamera(Camera *currentCamera)
//{
//    mCurrentCamera = currentCamera;
//}

//Camera *Shader::getCurrentCamera() const
//{
//    return mCurrentCamera;
//}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, Vec3 value)
{
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string &name, const Vec4 &value)
{
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
