#include "enemy.h"
#include "beziercurve.h"
#include "renderwindow.h"
#include "camera.h"

Enemy::Enemy(RenderWindow *Owner, Vec3 Pos1, Vec3 Pos2, Vec3 Pos3, Vec3 Pos4, int n)
    : Octahedron(Owner, n)
{
    mBCurve = new BezierCurve(Owner, Pos1, Pos2, Pos3, Pos4);
}

void Enemy::createObject()
{
    Octahedron::createObject();
    mBCurve->createObject();
}

void Enemy::drawObject()
{
    Octahedron::drawObject();
    mBCurve->drawObject();
}

void Enemy::update(float deltaTime)
{
    float speed = deltaTime / 4;

    if(forward)
    {
        if(Time < 1.0f)
        {
            Time += speed;
        }
        else
        {
            forward = false;
        }
    }
    else
    {
        if(Time > 0.0f)
        {
            Time -= speed;
        }
        else
        {
            forward = true;
        }
    }
    getTransform().setPosition(mBCurve->evaluateBezier(3, Time));
}

Vec3 Enemy::getNormal()
{
    return (getModelMatrix() * Vec4(0.f, 1.f, 0.f, 0.f)).toVector3D();
}

