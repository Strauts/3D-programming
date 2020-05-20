#ifndef LIGHT_H
#define LIGHT_H

#include "shader.h"
#include "octahedron.h"


class Light : public Octahedron
{
public:
    Light();
    Light(RenderWindow* Owner,
          Vec3 Position,
          float lightPwr,
          Vec3 lightColor,
          float specularPwr,
          Vec3 specularColor);

    void update(float deltaTime) override;
    float getLightPwr();
    Vec3 getLightColor();
    float getSpecularPwr();
    Vec3 getSpecularColor();
private:
    float Speed = 0.5;
    float rad{0.f};

    float mLightPwr;
    Vec3 mLightColor;
    float mSpecularPwr;
    Vec3 mSpecularColor;
};

#endif // LIGHT_H
