#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include "matrix4x4.h"
#include "transform.h"
#include "shader.h"
#include <memory>
#include <vector>
#include "material.h"
//#include "camera.h"

class Vertex;
class Matrix4x4;
class RenderWindow;

class SceneObject : protected QOpenGLFunctions_4_1_Core
{
public:
    SceneObject();
    SceneObject(RenderWindow* Owner);
    ~SceneObject();

    // = 0 because we have to override in children
    virtual void createObject() = 0;
    virtual void drawObject() = 0;
    virtual Vec3* normalAverage();

    virtual void collisionCheck(SceneObject* other);
    virtual void update(float deltaTime);
    virtual void switchSmooth();

    void setAvgNormalToggle(bool nT);

    bool getAvgNormalToggle();

    Matrix4x4 getModelMatrix(){ return transform.getModelMatrix(); }

    Transform& getTransform();

    std::unique_ptr<Matrix4x4> mModelMatrix;
    int mNumberOfVertices{0};
    int mNumberOfIndices{0};

    Matrix4x4 mMVPMatrix;
    Matrix4x4 mMatrix;

    //Material mMaterial;

    Vertex *vertices() const;
    RenderWindow* mOwner{nullptr};
    Transform transform;

    Vertex* mVertices{nullptr};
    GLuint* mIndices{nullptr};

protected:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;

    //everything below is for texture try
    //    Vertex* mVertices;
    //    GLuint* mIndices {nullptr};
    Shader* mShader;

    Vec3* mRegNormals {nullptr};
    Vec3* mAvgNormals {nullptr};

    GLuint mMVPUniform;
    GLuint mElementBuffer;
    //Matrix4x4 mMVPMatrix{true};
    //RenderWindow* mOwner{nullptr};

    GLuint mViewUniform;
    GLuint mModelUniform;
    GLuint mLightPosUniform;
    GLuint mDistAlgorithmUniform;
    GLuint mLightIntensUniform;
    GLuint mLightPowUniform;
    GLuint mLightColUniform;
    GLuint mAmbColUniform;
    GLuint mSpecColUniform;
    GLuint mColUniform;

    // Transform transform;

    //    int mNumberOfIndices{0};
    //    int mNumberOfVertices{0};

    bool lastToggle {false};
    bool normalToggle {false};

    std::vector<Vertex> mmVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mmIndices;    //This is usually not needed after object is made

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)
};

#endif // SCENEOBJECT_H
