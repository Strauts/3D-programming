#ifndef SKYBOX_H
#define SKYBOX_H

#include "sceneobject.h"
#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include "triangle.h"

class SkyBox : SceneObject
{
public:
    SkyBox();

    void createObject() override;
    void drawObject() override;
};

#endif // SKYBOX_H
