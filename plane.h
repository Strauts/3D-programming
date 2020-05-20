#ifndef PLANE_H
#define PLANE_H

#include "sceneobject.h"
#include "shader.h"
#include "renderwindow.h"
#include "camera.h"
#include "vertex.h"
#include "quaternion.h"
#include "triangle.h"

class Shader;

class Plane : public SceneObject
{
public:
    Plane();
    Plane(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    ~Plane();
    void createObject() override;
    void drawObject() override;
    void switchSmooth() override{}
    Vec3 getNormal();

private:
    Shader* mShader;
    GLuint mMVPMatrixUniform;

    GLuint mModelMatrixUniform;
    GLuint mViewMatrixUniform;
    GLuint mProjectionMatrixUniform;
    GLuint mIndexBuffer;
    Vec3 objectCol;

};

#endif // PLANE_H
