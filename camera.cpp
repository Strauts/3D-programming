#include "camera.h"
#include "renderwindow.h"
#include "vec3.h"
#include "transform.h"

Camera::Camera()
{
}

Camera::Camera(RenderWindow* Owner,
               Vec3 Center,
               Vec3 Up,
               float fov,
               float nearPlane,
               float farPlane)
    : SceneObject(Owner),
      mCenter{Center},
      mUp{Up},
      mFov{fov},
      mNearPlane{nearPlane},
      mFarPlane{farPlane}
{
    mAspectRatio = (float)mOwner->width() / mOwner->height();
}

void Camera::createObject()
{

}

void Camera::drawObject()
{

}


Matrix4x4 Camera::getPerspectiveMatrix()
{
    Matrix4x4 Perspective(true);

    Perspective.perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);

    return Perspective;
}

Matrix4x4 Camera::getViewMatrix()
{
    Matrix4x4 View(true);
    View.lookAt(getTransform().getPosition(), mCenter, mUp);

    return View;
}

void Camera::calculateRightUp()
{
    forward = lookAtPosition - position;
    forward.normalize();
    right = forward ^ Vec3(0.f, 1.0, 0.f);
    right.normalize();
    up = right ^ forward;
    up.normalize();
}
