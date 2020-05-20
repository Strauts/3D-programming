#include "ball.h"
#include "plane.h"
#include "renderwindow.h"
#include "trophie.h"
#include "enemy.h"
#include <QMessageBox>
#include "transform.h"

Ball::Ball(RenderWindow *Owner, Vec3 Position, Quaternion Rotation)
    : Octahedron(Owner, 3)
{
    getTransform().setPosition(Position);
    getTransform().setRotation(Rotation);
}

void Ball::update(float deltaTime)
{
    float radius = getTransform().getScale().getX();

    Vec3 positionChange = mVelocity*deltaTime;
    positionChange.setY(0.0f);

    Vec3 axis = positionChange^Vec3(0.0f, 1.0f, 0.0f);
    axis.normalize();

    float distanceTraveled = positionChange.length() / radius;

    Quaternion q(-distanceTraveled, axis);

    getTransform().addRotation(q);

    Vec3 Position = getTransform().getPosition();
    mVelocity = mVelocity + Vec3(0.0f, mGravity * deltaTime, 0.0f);
    getTransform().setPosition(Position + mVelocity * deltaTime);

    if(trohpiesCollected == 4)
    {
        //textUpdate->updateText();
        mOwner->resetGame();
        //mRenderWindow->close();
    }
    if(enemyHits == 3)
    {
        mOwner->resetGame();
       // mRenderWindow->close();
    }
}

void Ball::collisionCheck(SceneObject *Object)
{
    float radius = getTransform().getScale().getX();

    if(Plane* p = dynamic_cast<Plane*>(Object))
    {
        //        float distance;
        //        Vec3 normal = p->getNormal();
        //        Vec3 planeCenter = p->getTransform().getPosition();
        //        Vec3 sphereCenter = getTransform().getPosition();

        //        distance = normal * (sphereCenter - planeCenter);
        //        if(distance < radius && distance > 0)
        //        {
        //            Vec3 intersectPoint = sphereCenter - normal * distance;

        //            Vec3 position = intersectPoint + normal *radius;

        //            getTransform().setPosition(position);
        //            mVelocity = mVelocity - (normal * (normal * mVelocity));
        //        }
    }

    if(Ball* b = dynamic_cast<Ball*>(Object))
    {
        float distance;

        Vec3 sphereToSphere = b->getTransform().getPosition() - getTransform().getPosition();

        distance = sphereToSphere.length();

        if(distance < 2*radius)
        {
            Vec3 intersectPoint = getTransform().getPosition() - sphereToSphere.normalized() * distance;
            Vec3 position = intersectPoint + sphereToSphere.normalized() * radius;

            getTransform().setPosition(position);
            Vec3 velocity = mVelocity;

            mVelocity = b->getVelocity();
            b->setVelocity(velocity);
        }
    }

    if(curvedPlane* cp = dynamic_cast<curvedPlane*>(Object))
    {
        Vec3 ourPos = getTransform().getPosition();
        Matrix4x4 invCurvePlaneModel = cp->getTransform().getModelMatrix();
        invCurvePlaneModel.invertMatrix();
        //Transformer til local curveplane fra world
        ourPos = (invCurvePlaneModel * Vec4(ourPos)).toVector3D();

        int trinum = cp->findTriangle(ourPos);
        // std::cout << "The player is in triangle number: " << cp->findTriangle(ourPos) << std::endl;

        plane p = cp->getPlaneFromIndex(trinum);

        float distance;
        Vec3 normal = p.normal;
        Vec3 planeCenter = p.pos;
        Vec3 sphereCenter = getTransform().getPosition();

        distance = normal * (sphereCenter - planeCenter);

        if(distance < radius && distance > 0)
        {
            Vec3 intersectPoint = sphereCenter - normal * distance;
            Vec3 position = intersectPoint + normal * radius;
            getTransform().setPosition(position);
            mVelocity = mVelocity - (normal * (normal * mVelocity));
        }
    }
    if(trophie* b = dynamic_cast<trophie*>(Object))
    {
        float distance;

        Vec3 sphereToSphere = b->getTransform().getPosition() - getTransform().getPosition();

        distance = sphereToSphere.length();

        if(distance < 2*radius)
        {
            Vec3 intersectPoint = getTransform().getPosition() - sphereToSphere.normalized() * distance;
            Vec3 position = intersectPoint + sphereToSphere.normalized() * radius;

            b->getTransform().setPosition(Vec3(200.f,0.f,0.f));
            trohpiesCollected++;
            std::cout << "Trophie collected: " << trohpiesCollected << std::endl;
        }
    }
    if(Enemy* b = dynamic_cast<Enemy*>(Object))
    {
        float distance;

        Vec3 sphereToSphere = b->getTransform().getPosition() - getTransform().getPosition();

        distance = sphereToSphere.length();

        if(distance < 2*radius)
        {
            Vec3 intersectPoint = getTransform().getPosition() * 1.5 - sphereToSphere.normalized() * distance;
            Vec3 position = intersectPoint + sphereToSphere.normalized() * radius;

            getTransform().setPosition(position);
            enemyHits++;
            std::cout << "Enemy hits " << enemyHits << std::endl;
        }
    }
}

void Ball::addVelocity(Vec3 v)
{
    mVelocity = mVelocity + v;
}

Vec3 Ball::getVelocity()
{
    return mVelocity;
}

void Ball::setVelocity(Vec3 v)
{
    mVelocity = v;
}
