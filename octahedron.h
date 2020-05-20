#ifndef OCTAHEDRON_H
#define OCTAHEDRON_H

#include <string>
#include "quaternion.h"
#include "sceneobject.h"

class Vec3;
class Shader;
class Matrix4x4;

class Octahedron : public SceneObject
{
public:
    Octahedron();
    Octahedron(RenderWindow* Owner, int n = 0);
    ~Octahedron();

    virtual void createObject() override;
    virtual void drawObject() override;
    GLuint mVertexArrayObject;

private:
    void makeTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void makeUnitOctahedron();

    int mRecursions;          //
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices

    Shader* mShader;

    //    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    float mAngle = 0;
};

#endif // OCTAHEDRON_H
