#include <cmath>
#include "matrix4x4.h"
#include "quaternion.h"

Quaternion::Quaternion()
{
    w = 1.f;
    x = 0.f;
    y = 0.f;
    z = 0.f;
}

Quaternion::Quaternion(float ow, float ox, float oy, float oz)
    : w{ow}, x{ox}, y{oy}, z{oz}
{
    normalize();
}

Quaternion::Quaternion(float angle, Vec3 axis)
{
    w = std::cos(angle/2.f);
    x = axis.getX()*std::sin(angle/2.f);
    y = axis.getY()*std::sin(angle/2.f);
    z = axis.getZ()*std::sin(angle/2.f);
}

Quaternion Quaternion::operator*(const Quaternion &q)
{
    float yy = (w - y) * (q.w + q.z);
    float zz = (w + y) * (q.w - q.z);
    float ww = (z + x) * (q.x + q.y);
    float xx = ww + yy + zz;
    float qq = 0.5f * (xx + (z - x) * (q.x - q.y));

    return Quaternion(qq - ww + (z - y) * (q.y - q.z),
                      qq - xx + (x + w) * (q.x + q.w),
                      qq - yy + (w - x) * (q.y + q.z),
                      qq - zz + (z + y) * (q.w - q.x));
}

void Quaternion::normalize()
{
    float mag = magnitude();

    if(mag != 0.f)
    {
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }
}

float Quaternion::magnitude()
{
    return std::sqrt(w*w+x*x+y*y+z*z);
}

Matrix4x4 Quaternion::toRotationMatrix()
{
    normalize();
    Matrix4x4 rot =
    {
        {1 - 2*y*y - 2*z*z, 2*x*y - 2*z*w, 2*x*z + 2*y*w, 0.f},
        {2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z, 2*y*z - 2*x*w, 0.f},
        {2*x*z - 2*y*w, 2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y, 0.f},
        {     0.f,           0.f,       0.f,              1.f}
    };

    return rot;
}
