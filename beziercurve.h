#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "sceneobject.h"
#include "vec3.h"
#include "vertex.h"

class Shader;

class BezierCurve : public SceneObject
{
public:
    BezierCurve();
    BezierCurve(RenderWindow *Owner, Vec3 a, Vec3 b, Vec3 c, Vec3 d);

    void createObject() override;
    void drawObject() override;
    void switchSmooth() override{}

    Vec3 getControllPoint(int in);
    Vec3 getA();
    Vec3 getB();
    Vec3 getC();
    Vec3 getD();

    Vec3 evaluateBezier(int degree, float t);

    Vec3 controllPoint[4];
private:

    Vec3 mA;
    Vec3 mB;
    Vec3 mC;
    Vec3 mD;

    Vertex* mControlPointVertices;
    int mNumControlPointVertices;

    Shader* mShader;

    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    GLuint mVertexArrayObject2;
    GLuint mVertexBufferObject2;
    GLuint mMVPMatrixUniform2;
};

#endif // BEZIERCURVE_H
