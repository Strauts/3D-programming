#include "matrix4x4.h"
#include <cmath>
#include <utility>
#include <cassert>

#ifndef M_PI
#define M_PI 3.14159265358979323846     //Visual Studio did not find it...
#endif

Matrix4x4::Matrix4x4(bool isIdentity)
{
    if(isIdentity)
    {
        identity();
    }
    else
    {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                matrix[i][j] = 0.f;
    }
}

Matrix4x4::Matrix4x4(std::initializer_list<std::initializer_list<float>> values)
{
    // initialiser matrisen klassen på samme måte som en 2d-array
    int i = 0, j = 0;

    for(auto secondlevel : values)
    {
        for(auto thirdLevel : secondlevel)
        {
            matrix[i][j++] = thirdLevel;
        }
        i++;
        j = 0;
    }
}

Matrix4x4 Matrix4x4::identity()
{
    int c = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(j == c)
                matrix[i][j] = 1.f;
            else
                matrix[i][j] = 0.f;
        }
        c++;
    }

    return *this;
}

void Matrix4x4::translateX(GLfloat x)
{
    translate(x, 0.f, 0.f);
}

void Matrix4x4::translateY(GLfloat y)
{
    translate(0.f, y, 0.f);
}

void Matrix4x4::translateZ(GLfloat z)
{
    translate(0.f, 0.f, z);
}

void Matrix4x4::rotateX(GLfloat degrees)
{
    GLfloat rad = (degrees * M_PI)/180;

    Matrix4x4 temp =
    {
        {1.f,   0.f,       0.f,    0.f},
        {0.f, std::cos(rad),  std::sin(rad), 0.f},
        {0.f, -std::sin(rad), std::cos(rad), 0.f},
        {0.f,   0.f,       0.f,    1.f}
    };

    temp = (*this)*temp;
    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

void Matrix4x4::rotateY(GLfloat degrees)
{
    GLfloat rad = (degrees * M_PI)/180;

    Matrix4x4 temp =
    {
        {std::cos(rad), 0.f, -std::sin(rad), 0.f},
        {  0.f,    1.f,   0.f,     0.f},
        {std::sin(rad), 0.f, std::cos(rad),  0.f},
        {  0.f,   0.f,    0.f,  1.f}
    };

    temp = (*this)*temp;
    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

void Matrix4x4::rotateZ(GLfloat degrees)
{
    GLfloat rad = (degrees * M_PI)/180;

    Matrix4x4 temp =
    {
        {std::cos(rad),  std::sin(rad), 0.f, 0.f},
        {-std::sin(rad), std::cos(rad), 0.f, 0.f},
        {   0.f,      0.f,    1.f, 0.f},
        {   0.f,      0.f,    0.f, 1.f}
    };

    temp = (*this)*temp;
    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

void Matrix4x4::scale(GLfloat uniformScale)
{
    scale(uniformScale, uniformScale, uniformScale);
}

void Matrix4x4::scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
    Matrix4x4 temp =
    {
        {scaleX, 0.f,   0.f,   0.f},
        {0.f, scaleY,  0.f,   0.f},
        {0.f,   0.f,  scaleZ, 0.f},
        {0.f,   0.f,   0.f,   1.f}
    };

    temp = (*this)*temp;

    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

GLfloat *Matrix4x4::constData()
{
    return &matrix[0][0];
}

void Matrix4x4::transpose()
{
    std::swap(matrix[0][1], matrix[1][0]);
    std::swap(matrix[0][2], matrix[2][0]);
    std::swap(matrix[0][3], matrix[3][0]);

    std::swap(matrix[1][2], matrix[2][1]);
    std::swap(matrix[1][3], matrix[3][1]);
    std::swap(matrix[2][3], matrix[3][2]);
}

void Matrix4x4::ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat nearPlane, GLfloat farPlane)
{
    *this =
    {
        {2/(r-l), 0.f, 0.f, 0.f},
        {0.f, 2/(t-b), 0.f, 0.f},
        {0.f, 0.f, -2/(farPlane-nearPlane), 0.f},
        {-(r+l)/(r-l), -(t+b)/(t-b), -(farPlane+nearPlane)/(farPlane-nearPlane), 1.f}
    };
}

//https://stackoverflow.com/questions/18404890/how-to-build-perspective-projection-matrix-no-api
void Matrix4x4::perspective(GLfloat verticalAngle, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane)
{
    //checking numbers for no division on zero:
    if (verticalAngle <= 0.f)
        verticalAngle = 30.f;
    if (aspectRatio <= 0.f)
        aspectRatio = 1.3f;
    if (farPlane == nearPlane)
    {
        nearPlane = 1.f;
        farPlane = 100.f;
    }

    // finn right, og utled resten fra dette
    float scale = tan(verticalAngle * M_PI / 360) * nearPlane;
    float r = aspectRatio * scale;
    float t = scale;

    // Lag perspektiv-frustrum
    Matrix4x4 temp =
    {
        {nearPlane/r, 0.f, 0.f, 0.f},
        {0.f, nearPlane/t, 0.f, 0.f},
        {0.f, 0.f, -(farPlane+nearPlane)/(farPlane-nearPlane), -2*farPlane*nearPlane/(farPlane-nearPlane)},
        {0.f, 0.f, -1.f, 0.f}
    };

    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

void Matrix4x4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up_axis)
{
    Vec3 f = center-eye;
    f.normalize();
    Vec3 s = f^up_axis; // kryssprodukt
    s.normalize();
    Vec3 u = s^f;

    Matrix4x4 v =
    {
        { s.getX(),  s.getY(),  s.getZ(), -(s*eye)},
        { u.getX(),  u.getY(),  u.getZ(), -(u*eye)},
        {-f.getX(), -f.getY(), -f.getZ(), f*eye},
        {0.f, 0.f, 0.f, 1.f}
    };

    memcpy(matrix, v.matrix, 16*sizeof(GLfloat));
}

void Matrix4x4::translate(GLfloat x, GLfloat y, GLfloat z)
{
    Matrix4x4 temp =
    {
        {1.f, 0.f, 0.f, x},
        {0.f, 1.f, 0.f, y},
        {0.f, 0.f, 1.f, z},
        {0.f, 0.f, 0.f, 1.f}
    };

    temp = (*this)*temp;

    memcpy(matrix, temp.matrix, 16*sizeof(GLfloat));
}

bool Matrix4x4::invertMatrix()
{
    GLfloat m[16];
    memcpy(m, matrix, 16*sizeof(GLfloat));
    GLfloat inv[16], det;
    GLfloat invOut[16];

    int i;

    inv[0] = m[5]  * m[10] * m[15] -
            m[5]  * m[11] * m[14] -
            m[9]  * m[6]  * m[15] +
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] -
            m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
            m[4]  * m[11] * m[14] +
            m[8]  * m[6]  * m[15] -
            m[8]  * m[7]  * m[14] -
            m[12] * m[6]  * m[11] +
            m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
            m[4]  * m[11] * m[13] -
            m[8]  * m[5] * m[15] +
            m[8]  * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] -
            m[8]  * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
            m[1]  * m[11] * m[14] +
            m[9]  * m[2] * m[15] -
            m[9]  * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
            m[0]  * m[11] * m[14] -
            m[8]  * m[2] * m[15] +
            m[8]  * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
            m[0]  * m[11] * m[13] +
            m[8]  * m[1] * m[15] -
            m[8]  * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
            m[0]  * m[10] * m[13] -
            m[8]  * m[1] * m[14] +
            m[8]  * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
            m[1]  * m[7] * m[14] -
            m[5]  * m[2] * m[15] +
            m[5]  * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
            m[0]  * m[7] * m[14] +
            m[4]  * m[2] * m[15] -
            m[4]  * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
            m[0]  * m[7] * m[13] -
            m[4]  * m[1] * m[15] +
            m[4]  * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
            m[0]  * m[6] * m[13] +
            m[4]  * m[1] * m[14] -
            m[4]  * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    memcpy(matrix, invOut, 16*sizeof(GLfloat));

    return true;
}
