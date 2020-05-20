#ifndef QUATERNION_H
#define QUATERNION_H

class Matrix4x4;
class Vec3;

class Quaternion
{
public:
    Quaternion();
    Quaternion(float ow, float ox, float oy, float oz);
    Quaternion(float angle, Vec3 axis);

    Quaternion operator*(const Quaternion& q);

    void normalize();
    float magnitude();

    Matrix4x4 toRotationMatrix();
private:
    float w, x, y, z;
};

#endif // QUATERNION_H
