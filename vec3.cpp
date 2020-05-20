#include "vec3.h"
#include "matrix4x4.h"
#include <cmath>
#include "vertex.h"

#define M_PI 3.14159265358979323846     //Visual studio



Vec3::Vec3(GLfloat x_in, GLfloat y_in, GLfloat z_in)
    : x{x_in}, y{y_in}, z{z_in}
{
}

const Vec3 &Vec3::operator =(const Vec3 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vec3 Vec3::operator +(const Vec3 &v) const
{
    Vec3 u;
    u.x = x + v.x;
    u.y = y + v.y;
    u.z = z + v.z;
    return u;
}

Vec3 Vec3::operator -(const Vec3 &v) const
{
    Vec3 u;
    u.x = x - v.x;
    u.y = y - v.y;
    u.z = z - v.z;
    return u;
}

bool Vec3::operator ==(const Vec3 &v) const
{
    float v11 = getX();
    float v12 = getY();
    float v13 = getZ();

    float v21 = v.getX();
    float v22 = v.getY();
    float v23 = v.getZ();

    bool b1 = (v11 == v21);
    bool b2 = (v12 == v22);
    bool b3 = (v13 == v23);

    return b1 && b2 && b3;
}


Vec3 Vec3::operator ^(const Vec3 &v) const
{
    Vec3 w;
    w.x =  y*v.z - z*v.y;
    w.y =  z*v.x - x*v.z;
    w.z =  x*v.y - y*v.x;
    return w;
}

void Vec3::normalize()
{
    GLfloat d = x*x+y*y+z*z;
    d = sqrt(d);
    if (d>0.0)
    {
        x=x/d;
        y=y/d;
        z=z/d;
    }
}

Vec3 Vec3::normalized()
{
    Vec3 normalized;
    GLfloat d = x*x+y*y+z*z;
    d = sqrt(d);
    if (d>0.0)
    {
        normalized.x=x/d;
        normalized.y=y/d;
        normalized.z=z/d;
    }
    return normalized;
}

void Vec3::rotateX(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x;
    dir.y = y * cos(angle) + z * -sin(angle);
    dir.z = y * sin(angle) + z * cos(angle) ;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateY(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * cos(angle) + z * sin(angle);
    dir.y = y;
    dir.z = x * -sin(angle) + z * cos(angle);

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateZ(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * cos(angle) + y * -sin(angle);
    dir.y = x * sin(angle) + y * cos(angle);
    dir.z = z;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

Vec3 Vec3::operator *(Matrix4x4 q) const
{
    q = q;  //just noncense for now to get rid of warning
    return *this;   //just noncense for now to get rid of warning
}

float Vec3::vecLength(Vec3 &a, Vec3 &b)
{
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

float Vec3::areal(float a, float b, float c)
{
    float s = (a + b + c) / 2;
    float v = s * (s - a) * (s - b) * (s - c);
    float areal = sqrt(v);
    return areal;
}

float Vec3::length()
{
    return sqrt(x*x+y*y+z*z);
}

double Vec3::rad2Deg(double &rad)
{
    return (rad * 180)/M_PI;    //M_PI is in cmath
}

double Vec3::deg2Rad(double &deg)
{
    return (deg * M_PI)/180;    //M_PI is in cmath
}

GLfloat Vec3::getX() const
{
    return x;
}

void Vec3::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec3::getY() const
{
    return y;
}

void Vec3::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vec3::getZ() const
{
    return z;
}

void Vec3::setZ(const GLfloat &value)
{
    z = value;
}

Vec3 Vertex::get_normal()
{
    return mNormal;
}

Vec3 Vec3::barycentCoord(Vec3 &p0, Vec3 &p1, Vec3 &p2, Vec3& normal)
{   
    Vec3 u = p2 - p0;
    Vec3 v = p1 - p0;
    Vec3 w = p1 - p2;

    Vec3 x = *this - p0;
    //Vec3 y = *this - p1;
    Vec3 z = *this - p2;

    float Au = (normal*(u^x)) / 2;
    float Av = (normal*(x^v)) / 2;
    float Aw = (normal*(w^z)) / 2;

    return Vec3(Au, Av, Aw);
}

Vec3 Vec3::operator *(float c) const
{
    return Vec3(x*c, y*c, z*c);
}

float Vec3::operator *(const Vec3 &v) const
{
    return x*v.x + y*v.y + z*v.z;
}
