#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <QtGui/qopengl.h> // for GLfloat

#include <iostream>
#include <iomanip>
#include <cassert>
#include "vec3.h"
#include "vec4.h"

class Matrix4x4
{
public:
    Matrix4x4(bool isIdentity = false);
    Matrix4x4(std::initializer_list<std::initializer_list<float>> values);

    Matrix4x4 identity();

    bool invertMatrix();

    void translateX(GLfloat x = 0.f);
    void translateY(GLfloat y = 0.f);
    void translateZ(GLfloat z = 0.f);

    void rotateX(GLfloat degrees = 0.f);
    void rotateY(GLfloat degrees = 0.f);
    void rotateZ(GLfloat degrees = 0.f);

    void scale(GLfloat uniformScale = 1.f);
    void scale(GLfloat scaleX = 1.f, GLfloat scaleY = 1.f, GLfloat scaleZ = 1.f);

    GLfloat matrix[4][4]{};
    GLfloat* constData();

    void transpose();

    void ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat nearPlane, GLfloat farPlane);
    void perspective(GLfloat verticalAngle, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

    void lookAt(const Vec3& eye, const Vec3& center, const Vec3& up_axis);

    void translate(GLfloat x = 0.f, GLfloat y = 0.f, GLfloat z = 0.f);

    //strassen's matrix multiplication
    Matrix4x4 operator*(const Matrix4x4 &other)
    {
        // Multiply matrix by another one
        Matrix4x4 temp;
        // each column dotted with each row
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                temp.matrix[i][j] =
                        Vec4(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3])
                        *
                        Vec4(other.matrix[0][j], other.matrix[1][j], other.matrix[2][j], other.matrix[3][j]);
            }
        }

        return temp;
    }

    Vec4 operator*(const Vec4 &other)
    {
        float temp[4];
        // dot hver matrise kolonne med vektor rad
        for(int i = 0; i < 4; i++)
        {
            Vec4 row(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
            temp[i] = row*other;
        }

        return Vec4(temp[0], temp[1], temp[2], temp[3]);
    }

    //Operator overloading as friendfunction
    friend std::ostream &operator<<(std::ostream &output, const Matrix4x4 &mIn )
    {
        output << std::setprecision(4) <<
                  "{{" << mIn.matrix[0] << "\t, " << mIn.matrix[4] << "\t, " << mIn.matrix[8] << "\t, " << mIn.matrix[12] << "}\n" <<
                  " {" << mIn.matrix[1] << "\t, " << mIn.matrix[5] << "\t, " << mIn.matrix[9] << "\t, " << mIn.matrix[13] << "}\n" <<
                  " {" << mIn.matrix[2] << "\t, " << mIn.matrix[6] << "\t, " << mIn.matrix[10] << "\t, " << mIn.matrix[14] << "}\n" <<
                  " {" << mIn.matrix[3] << "\t, " << mIn.matrix[7] << "\t, " << mIn.matrix[11] << "\t, " << mIn.matrix[15] << "}}\n";
        return output;
    }

};
#endif // MATRIX4X4_H
