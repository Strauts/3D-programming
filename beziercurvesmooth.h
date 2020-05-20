#ifndef BEZIERCURVESMOOTH_H
#define BEZIERCURVESMOOTH_H

#include "sceneobject.h"
#include "beziercurvesmooth.h"
#include "vertex.h"

class Shader;
class BezierCurve;

class BezierCurveSmooth : public SceneObject
{
public:
    BezierCurveSmooth();
    BezierCurveSmooth(RenderWindow *Owner, BezierCurve *curve, int smoothValue);

    void createObject() override;
    void drawObject() override;
    void switchSmooth() override{}

    Vec3 getControllPoint(int in);
    Vec3 evaluateBezier(int degree, float t);

private:
    Vec3 controllPoint[4];

    Shader* mShader;

    int mSmoothVal;

    // Make sure constructor isn't going past 21, else make it larger
    Vertex mvtx[21];

    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;
};

#endif // BEZIERCURVESMOOTH_H
