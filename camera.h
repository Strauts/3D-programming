#ifndef CAMERA_H
#define CAMERA_H

#include "sceneobject.h"
class Transform;
class Vec3;

class Camera : public SceneObject
{
public:
    Camera();
    Camera(RenderWindow* Owner,
           Vec3 Center,
           Vec3 Up,
           float fov,
           float nearPlane,
           float farPlane);

    void createObject() override;
    void drawObject() override;
    void switchSmooth() override{}

 //   const float velocity = 0.1f;

    Matrix4x4 getPerspectiveMatrix();
    Matrix4x4 getViewMatrix();

    void calculateRightUp();
    Vec3 position;
    Vec3 lookAtPosition;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
private:
    Vec3 mCenter;
    Vec3 mUp;


    float mFov;
    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
};

#endif // CAMERA_H
