#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "quaternion.h"

class Camera;

class Transform
{
public:
    Transform();
    Transform(Vec3 Position, Vec3 Scale, Quaternion Rotation);

    Matrix4x4 getModelMatrix();
    void setRotation(Quaternion r);
    void addRotation(Quaternion r);
    Quaternion getRotation();
    void setPosition(Vec3 p);
    Vec3 getPosition();
    void setScale(Vec3 s);
    Vec3 getScale();
private:
    Quaternion mRotation;
    Vec3 mPosition;
    Vec3 mScale;
};

#endif // TRANSFORM_H
