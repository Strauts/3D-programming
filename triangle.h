#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "sceneobject.h"
#include "quaternion.h"

class Vertex;
class Matrix4x4;
class Shader;

class Triangle : public SceneObject
{
public:
    Triangle(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    Triangle();
    ~Triangle();

    void createObject() override;
    void drawObject() override;

    void update(float deltaTime) override;
    void switchSmooth() override{}
private:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    Shader* mShader;
};

#endif // TRIANGLE_H
