#ifndef VEC3_H
#define VEC3_H

#include <QtGui/qopengl.h> // for GLfloat

#include <cmath>
#include <iostream>
#include <cassert>

class Matrix4x4;

class Vec3
{
public:
    Vec3(GLfloat x_in=0.0, GLfloat y_in=0.0, GLfloat z_in=0.0);

    const Vec3 &operator = (const Vec3& v);
    Vec3 operator + (const Vec3& v) const;
    Vec3 operator - (const Vec3& v) const;
    bool operator == (const Vec3& v) const;

    Vec3 operator ^ (const Vec3& v) const;  // Cross product

    void normalize();
    Vec3 normalized();

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    float operator * (const Vec3& v) const; // Dot product
    Vec3 operator * (float c) const;
    Vec3 operator * (Matrix4x4 q) const;

    float length();
    float vecLength(Vec3 &a, Vec3 &b);
    float areal(float a, float b, float c);

    double rad2Deg(double &rad);
    double deg2Rad(double &deg);

    //Operator overloading as friendfunction
    friend std::ostream &operator<<(std::ostream &output, const Vec3 &vIn )
    {
        output << "X = " << vIn.x << ", Y = " << vIn.y << ", Z = " << vIn.z;;
        return output;
    }

    const GLfloat& operator[] (const int index) const
    {
        assert(index <3 && index >=0);

        switch (index) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        }
        return x;   //to silence compiler warnings
    }


    GLfloat getX() const;
    void setX(const GLfloat &value);
    GLfloat getY() const;
    void setY(const GLfloat &value);
    GLfloat getZ() const;
    void setZ(const GLfloat &value);

    Vec3 barycentCoord(Vec3 &a, Vec3 &b, Vec3 &c, Vec3& normal);

    GLfloat x;
    GLfloat y;
    GLfloat z;

private:
//    GLfloat x;
//    GLfloat y;
//    GLfloat z;
};

#endif // VEC3_H
