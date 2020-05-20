#include "transform.h"
#include "matrix4x4.h"
#include "camera.h"

Transform::Transform()
    : mScale{1.f, 1.f, 1.f}
{

}

Transform::Transform(Vec3 Position, Vec3 Scale, Quaternion Rotation)
    : mPosition{Position}, mScale{Scale}, mRotation{Rotation}
{

}

Matrix4x4 Transform::getModelMatrix()
{
    Matrix4x4 Model(true);

    // transler og roter objektet lokalt
    Model.translate(mPosition.getX(), mPosition.getY(), mPosition.getZ());

    Model.scale(mScale.getX(), mScale.getY(), mScale.getZ());

    Model = Model * mRotation.toRotationMatrix();

    return Model;
}

void Transform::setRotation(Quaternion r)
{
    mRotation = r;
}

void Transform::addRotation(Quaternion r)
{
    mRotation = r * mRotation;
}

Quaternion Transform::getRotation()
{
    return mRotation;
}

void Transform::setPosition(Vec3 p)
{
    mPosition = p;
}

Vec3 Transform::getPosition()
{
    return mPosition;
}

void Transform::setScale(Vec3 s)
{
    mScale = s;
}

Vec3 Transform::getScale()
{
    return mScale;
}
