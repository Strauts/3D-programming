#include "light.h"
#include "cmath"
#include "vertex.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "renderwindow.h"
#include "shader.h"
#include "camera.h"

Light::Light()
{
}

Light::Light(RenderWindow *Owner, Vec3 Position, float lightPwr, Vec3 lightColor, float specularPwr, Vec3 specularColor)
    : Octahedron(Owner, 0),
    mLightPwr{lightPwr},
    mLightColor{lightColor},
    mSpecularPwr{specularPwr},
    mSpecularColor{specularColor}

{
    getTransform().setPosition(Position);
}

void Light::update(float deltaTime)
{
    rad += Speed*deltaTime;

    Vec3 Position = Vec3(10*cos(rad), getTransform().getPosition().getY(), 10*sin(rad));

    getTransform().setPosition(Position);
}

float Light::getLightPwr()
{
    return mLightPwr;
}

Vec3 Light::getLightColor()
{
    return mLightColor;
}

float Light::getSpecularPwr()
{
    return mSpecularPwr;
}

Vec3 Light::getSpecularColor()
{
    return mSpecularColor;
}
