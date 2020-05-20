#include "vec4.h"
#include "vec3.h"

Vec4::Vec4(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat w_in)
    : x{x_in}, y{y_in}, z{z_in}, w{w_in}
{
}

Vec4::Vec4(Vec3 vec3_in, GLfloat w_in)
    : x{vec3_in.getX()}, y{vec3_in.getY()}, z{vec3_in.getZ()}, w{w_in}
{
}

Vec3 Vec4::toVector3D()
{
    return Vec3(x, y, z);
}

const Vec4 &Vec4::operator =(const Vec4 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

Vec4 Vec4::operator +(const Vec4 &v) const
{
    Vec4 u;
    u.x = x + v.x;
    u.y = y + v.y;
    u.z = z + v.z;
    u.w = w + v.w;
    return u;
}

Vec4 Vec4::operator -(const Vec4 &v) const
{
    Vec4 u;
    u.x = x - v.x;
    u.y = y - v.y;
    u.z = z - v.z;
    u.w = w - v.w;
    return u;
}

void Vec4::normalize()
{
    GLfloat d = x*x+y*y+z*z;
    d = sqrt(d);
    if (d>0.0)
    {
        x=x/d;
        y=y/d;
        z=z/d;
        w=w/d;
    }
}

Vec4 Vec4::normalized()
{
    Vec4 normalized;
    GLfloat d = x*x+y*y+z*z+w*w;
    d = sqrt(d);
    if (d>0.0)
    {
        normalized.x=x/d;
        normalized.y=y/d;
        normalized.z=z/d;
        normalized.w=w/d;
    }
    return normalized;
}

float Vec4::operator *(const Vec4 &v) const
{
    return x*v.getX() + y*v.getY() + z*v.getZ() + w*v.getW();
}

GLfloat Vec4::getX() const
{
    return x;
}

void Vec4::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec4::getY() const
{
    return y;
}

void Vec4::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vec4::getZ() const
{
    return z;
}

void Vec4::setZ(const GLfloat &value)
{
    z = value;
}

GLfloat Vec4::getW() const
{
    return w;
}
void Vec4::setW(const GLfloat &value)
{
    if(value == 0.f || value == 1.f)    //w should be only 0 or 1
        w = value;
}

Vec3 Vec4::vec4tovec3()
{
    return Vec3(x, y, z);
}
