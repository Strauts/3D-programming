#include "trophie.h"

trophie::trophie(RenderWindow* Owner, Vec3 Position, Quaternion Rotation)
    : Pyramid/*Triangle*/(Owner, Position, Rotation)
{
}

trophie::~trophie()
{

}

void trophie::update(float deltaTime)
{
    getTransform().addRotation(Quaternion(deltaTime, Vec3(0.f, 1.f, 0.f)));
}
