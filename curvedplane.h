#ifndef CURVEDPLANE_H
#define CURVEDPLANE_H

#include "shader.h"
#include "triangle.h"
#include "sceneobject.h"
#include <stack>
#include "constant.h"
#include "pyramid.h"

struct plane
{
    plane(Vec3 p, Vec3 n)
    {
        pos = p;
        normal = n;
    }
    Vec3 pos;
    Vec3 normal;
};

struct triangle
{
    triangle(){}
    triangle(int n1, int n2, int n3)
    {
        n[0] = n1;
        n[1] = n2;
        n[2] = n3;
    }
    int n[3];
};

struct triangleEdge
{

    int e[3];

};



class curvedPlane : public SceneObject
{
public:
    curvedPlane();
    curvedPlane(RenderWindow* Owner, Vec3 Position,
                double width, double height, double startX, double startZ, double delta);
    ~curvedPlane();
    void indexingPlane(double width, double height, double startX, double startZ, double delta);
    void switchSmooth();
    void createObject() override;
    void drawObject() override;
    double xzFunction(double x, double z);
    double derivedX(double x, double z);
    double derivedZ(double x, double z);
    //Vec3 terrainNormal(double x, double z);
    Vec3 getNormal(double x, double y);
    Vec3 getTriangleNormal(Vec3 a, Vec3 b, Vec3 c);
    Vec3 getCornerNormal(int b);
    Vec3* getNormalsFromAverageFaces();
    std::vector<triangle> triangles;
    std::vector<triangleEdge> triEdges;
    std::vector<GLuint> index;
    std::vector<int>tripos;

    int findTriangle(Vec3 point);


    bool has_index(const triangle& t1, int i);
    Vec3 normalFromTriangle(const triangle& t);

    void topologicalSort();
    void recursiveTopologicalSort(int i, bool* visited, std::stack<int>& stack);

    plane getPlaneFromIndex(int i);

private:
    GLuint mModelMatrixUniform;
    GLuint mViewMatrixUniform;
    GLuint mProjectionMatrixUniform;
    GLuint mIndexBuffer;
    Vec3 objectCol;
    Shader* mShader;

    int mWidth;
    int mHeight;
    int numbOfTriangles;
    double sX;
    double sZ;

    bool prev_smooth{false};
};

#endif // CURVEDPLANE_H
