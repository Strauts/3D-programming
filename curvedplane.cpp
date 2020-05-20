#include "curvedplane.h"
#include "shader.h"
#include "renderwindow.h"
#include "camera.h"
#include "vertex.h"
#include "light.h"
#include "quaternion.h"


typedef int trekant[3];

curvedPlane::curvedPlane()
{
}

curvedPlane::curvedPlane(RenderWindow *Owner, Vec3 Position, double width,
                         double height, double startX, double startZ, double delta)
    : SceneObject(Owner)
{
    transform.setPosition(Position);
    indexingPlane(width, height, startX, startZ, delta);
}

curvedPlane::~curvedPlane()
{}

void curvedPlane::indexingPlane(double width, double height, double startX, double startZ, double delta)
{
    //Round up to closest integer
    int realWidth = ceil(width/delta) + 1;
    // Round up to closest integer
    int realHeight = ceil(height/delta) + 1;

    mWidth = realWidth;
    mHeight = realHeight;

    mNumberOfVertices = realWidth*realHeight;
    // Used for testing
    std::cout << mNumberOfVertices << std::endl;
    mVertices = new Vertex[mNumberOfVertices];
    numbOfTriangles = (width * height * 2)/delta;
    objectCol = {1.0, 1.0, 1.0};
    // Where we start drawing from X axis
    sX = startX;
    // Where we start drawing from Z axis
    sZ = startZ;

    for(int i = 0; i < realHeight; i++)
    {
        for(int j = 0; j < realWidth; j++)
        {
            mVertices[i*realWidth + j] = {Vec3(sX, xzFunction(sX, sZ), sZ),
                                          getNormal(sX, sZ), //Vec3(0.f, 1.f, 0.f)
                                          Vec2()};
            sX += delta;
        }
        sZ += delta;
        sX = startX;
    }

    if(mOwner->smooth)
        for(int i = 0; i < realHeight; i++)
        {
            for(int j = 0; j < realWidth; j++)
            {
                mVertices[i*realWidth + j].set_normal(getCornerNormal(i*realWidth + j));
                sX += delta;
            }
            sZ += delta;
            sX = startX;
        }

    int k = 0;
    for(int i = 0; i < realHeight-1; i++)
    {
        for(int j = 0; j < realWidth-1; j++)
        {
            // First triangle
            //First point in first triangle
            index.push_back(realWidth*i + j);
            //Second point in first triangle
            index.push_back(realWidth*i + j + 1);
            //Third point in first triangle
            index.push_back(realWidth*i + realWidth + j);

            triangleEdge t;
            if(j > 0)
            {
                t.e[0] = k - 1;
            }
            else
            {
                t.e[0] = -1;
            }
            if(i > 0)
            {
                t.e[1] = k - (realWidth-1)*2;
            }
            else
            {
                t.e[i] = -1;
            }
            t.e[2] = k + 1;
            triEdges.push_back(t);
            k++;

            // Second triangle
            //First point in second triangle
            index.push_back(realWidth*i + j + 1);
            //Second point in second triangle
            index.push_back(realWidth*i + realWidth + 1 + j);
            //Third point in second triangle
            index.push_back(realWidth*i + realWidth + j);

            t = triangleEdge();
            t.e[2] = k - 1;

            if(i < realHeight - 2)
            {
                t.e[1] = k + (realWidth - 1) * 2 - 1;
            }
            else
            {
                t.e[1] = -1;
            }
            if(j < realWidth - 2)
            {
                t.e[0] = k + 1;
            }
            else
            {
                t.e[0] = -1;
            }
            triEdges.push_back(t);
            k++;
        }
    }

    mNumberOfIndices = index.size();
    mIndices = new GLuint[mNumberOfIndices];
    for(int i = 0; i < mNumberOfIndices; i++)
        mIndices[i] = index[i];

    for(int i = 0; i < mNumberOfIndices; i+= 3)
    {
        triangles.push_back(triangle(index[i], index[i+1], index[i+2]));
    }

    topologicalSort();
}

void curvedPlane::switchSmooth()
{
    if(mOwner->smooth != prev_smooth)
    {
        Vec3* normals = getNormalsFromAverageFaces();
        for(int i = 0; i < mNumberOfVertices; i++)
        {
            if(mOwner->smooth)
            {
                mVertices[i].set_normal(normals[i]);
            }
            else
            {
                float xCoord = mVertices[i].get_x();
                float zCoord = mVertices[i].get_z();
                mVertices[i].set_normal(getNormal(xCoord, zCoord));
            }
        }
        delete[] normals;
    }
    prev_smooth = mOwner->smooth;
}

void curvedPlane::createObject()
{
    initializeOpenGLFunctions();
    //Remember to put phong on all objects
    mShader = new Shader("../3D-programming/phong.vert",
                         "../3D-programming/phong.frag");
    // Gen VAO
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Gen Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(),
                 GL_DYNAMIC_DRAW);

    // Tell our GPU how to read positions
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell our GPU how to read colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Tell our GPU how to read uv's
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mModelMatrixUniform = glGetUniformLocation(mShader->getProgram(), "model");
    mViewMatrixUniform = glGetUniformLocation(mShader->getProgram(), "view");
    mProjectionMatrixUniform = glGetUniformLocation(mShader->getProgram(), "projection");

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfIndices*sizeof(GLuint), mIndices, GL_STATIC_DRAW);

}

void curvedPlane::drawObject()
{
    // Tell the GPU to use a shader
    glUseProgram(mShader->getProgram());
    mShader->setVec3("ambientCol", Vec3(0.2f, 0.2f, 0.2f));

    Vec3 lightPos = mOwner->getLight()->getTransform().getPosition();
    float lightPwr = mOwner->getLight()->getLightPwr();
    Vec3 lightColor = mOwner->getLight()->getLightColor();

    float specularPwr = mOwner->getLight()->getSpecularPwr();
    Vec3 specularColor = mOwner->getLight()->getSpecularColor();

    //All light values to shader
    mShader->setVec3("lightPos", lightPos);
    mShader->setFloat("lightPwr", lightPwr);
    mShader->setVec3("lightColor", lightColor);
    mShader->setFloat("specularPwr", specularPwr);
    mShader->setVec3("specularColor", specularColor);

    mShader->setVec3("objectCol", objectCol);
    //    Vec3 camPos = mOwner->getCamera()->getTransform().getPosition();
    //    mShader->setVec3("cameraPos", camPos);
    //std::cout << camPos << std::endl;

    // Read our vertex data
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    // Load the matrix into the shader
    glUniformMatrix4fv(mModelMatrixUniform, 1, GL_TRUE, Model.constData());
    glUniformMatrix4fv(mViewMatrixUniform, 1, GL_TRUE, View.constData());
    glUniformMatrix4fv(mProjectionMatrixUniform, 1, GL_TRUE, Projection.constData());

    glDrawElements(GL_TRIANGLES, mNumberOfIndices, GL_UNSIGNED_INT, (GLvoid*)0);

    mOwner->checkForGLerrors();
}

double curvedPlane::xzFunction(double x, double z)
{
    double s = sin(x*0.5)*sin(x*0.5);
    double c = cos(z*0.5)*cos(z*0.5);
    return s + c;
}

double curvedPlane::derivedX(double x, double z)
{
    double s = sin(x*0.5);
    double c = cos(x*0.f);
    // double d = sin(5*x) / 5.f;
    return s*c;
}

double curvedPlane::derivedZ(double x, double z)
{
    double s = -sin(0.5*z);
    double c = cos(0.5*z);
    // double d = cos(5*z) / 5;
    return s*c;
}

//Vec3 curvedPlane::terrainNormal(double x, double z)
//{

//}

Vec3 curvedPlane::getNormal(double x, double z)
{
    double cx = derivedX(x, z);
    double cz = derivedZ(x, z);
    double cd = xzFunction(x, z) - cx*x - cz*z;

    Vec3 p0 = Vec3(x,       cx*x + cz*z + cd,       z);
    Vec3 p1 = Vec3(x + 1,   cx*(x+1) + cz*z + cd,  z);
    Vec3 p2 = Vec3(x,       cx*x + cz*(z+1.f) + cd, z + 1.f);

    Vec3 v1 = p1 - p0;
    Vec3 v2 = p2 - p0;
    Vec3 normal = (v2^v1).normalized();
    return normal;
}

Vec3 curvedPlane::getTriangleNormal(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 N = (a - b) ^ (b - c); //Cross-Product
    N.normalize();
    Vec3 n = N;
    return n;
}

Vec3 curvedPlane::getCornerNormal(int b)
{
    //    Vec3 a = mVertices[b].get_xyz();
    //    Vec3 sum;
    //    Vec3 points[7];
    //    Vec3 triNor[6];
    //    GLfloat counter = 0;

    //    bool left{false};
    //    bool right{false};
    //    bool bottom{false};
    //    bool top{false};

    //    points[3] = a;

    //    if(a.getX() == -5.f)
    //        left = true;
    //    if(a.getX() == 5.f)
    //        right = true;
    //    if(a.getZ() == -5.f || b <=49)
    //        bottom = true;
    //    if(a.getZ() == 5.f)
    //        top = true;

    //    if(!left)
    //    {
    //        points[2] = mVertices[b - 1].get_xyz();
    //        if(!top)
    //        {
    //            points[1] = mVertices[b + 50].get_xyz();
    //            points[0] = mVertices[b + 50 - 1].get_xyz();
    //            triNor[0] = getTriangleNormal(points[0], points[2], points[3]);
    //            triNor[1] = getTriangleNormal(points[0], points[1], points[3]);
    //            sum = sum + triNor[0];
    //            sum = sum + triNor[1];
    //            counter += 2;
    //        }
    //        if(!bottom)
    //        {
    //            points[5] = mVertices[b - 50].get_xyz();
    //            triNor[3] = getTriangleNormal(points[2], points[3], points[5]);
    //            sum = sum + triNor[3];
    //            counter ++;
    //        }
    //    }
    //    if(!right)
    //    {
    //        points[4] = mVertices[b + 1].get_xyz();
    //        if(!bottom)
    //        {
    //            points[5] = mVertices[b - 50].get_xyz();
    //            points[6] = mVertices[b - 50 + 1].get_xyz();
    //            triNor[4] = getTriangleNormal(points[6], points[3], points[5]);
    //            triNor[5] = getTriangleNormal(points[6], points[3], points[4]);
    //            sum = sum + triNor[4];
    //            sum = sum + triNor[5];
    //            counter += 2;
    //        }
    //        if(!top)
    //        {
    //            triNor[2] = getTriangleNormal(points[1], points[4], points[3]);
    //            sum = sum + triNor[2];
    //            counter++;
    //        }
    //    }
    //    sum.normalize();
    //    return sum;
}

Vec3* curvedPlane::getNormalsFromAverageFaces()
{
    std::vector<triangle> triangles;
    for(int i = 0; i < mNumberOfIndices; i+= 3)
    {
        triangles.push_back(triangle(index[i], index[i+1], index[i+2]));
    }

    Vec3* normals = new Vec3[mNumberOfVertices];
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        std::vector<triangle> sharing;
        for(int j = 0; j < triangles.size(); j++)
        {
            if(has_index(triangles[j], i))
                sharing.push_back(triangles[j]);
        }

        Vec3 sum;
        for(int j = 0; j < sharing.size(); j++)
        {
            sum = sum + normalFromTriangle(sharing[j]);
        }
        sum.normalize();
        normals[i] = sum;
    }
    return normals;
}

int curvedPlane::findTriangle(Vec3 point)
{

    int counter = 0;

    for(int i = 0; i < triangles.size() && i >= 0;)
    {
        counter++;

        Vec3 p0 = mVertices[triangles[i].n[0]].get_xyz();
        Vec3 p1 = mVertices[triangles[i].n[1]].get_xyz();
        Vec3 p2 = mVertices[triangles[i].n[2]].get_xyz();

        Vec3 normal = normalFromTriangle(triangles[0]);
        Vec3 Areal = point.barycentCoord(p0, p1, p2, normal);

        if(Areal.getX() >= 0 && Areal.getY() >= 0 && Areal.getZ() >= 0)
            return i;
        if(counter > 80)
        {
            std::cout << "Breakpoint here" << std::endl;
        }
        //i = 0, 2, 4, 6 ,8, osv
        if(i % 2 == 0)
        {
            if(Areal.getZ() < 0 && Areal.getX() >= 0 && Areal.getY() >= 0)
            {
                i++;
            }
            else if(Areal.getX() < 0 && Areal.getY() >= 0 && Areal.getZ() >= 0)
            {
               i -= (mWidth-1) * 2 - 1;
            }
            else if(Areal.getY() < 0 && Areal.getX() >= 0 && Areal.getZ() >=0)
            {
               i--;
            }
            else if(Areal.getX() < 0 && Areal.getY() < 0 && Areal.getZ() >= 0)
            {
                if(Areal.getX() < Areal.getZ())
                {
                    i -= (mWidth-1) * 2 - 1;
                }
                else
                {
                    i--;
                }
            }
            else if(Areal.getY() < 0 && Areal.getZ() < 0 && Areal.getX() >= 0)
            {
                if(Areal.getZ() < Areal.getY())
                {
                    i++;
                }
                else
                {
                    i--;
                }
            }
        }
        //i = 1, 3, 5, 7, 9, osv
        else
        {
            if(Areal.getX() < 0 &&Areal.getY() >= 0 && Areal.getZ() >= 0)
            {
                i--;
            }
            if(Areal.getY() < 0 && Areal.getX() >= 0 && Areal.getZ() >= 0)
            {
                i += (mWidth-1) * 2 - 1;
            }
            if(Areal.getZ() < 0 && Areal.getX() >= 0 && Areal.getY() >= 0)
            {
                i++;
            }
            if(Areal.getY() < 0 && Areal.getZ() < 0 && Areal.getX() >= 0)
            {
                if(Areal.getZ() < Areal.getY())
                {
                    i++;
                }
                else
                {
                    i += (mWidth-1) * 2 - 1;
                }
            }
            if(Areal.getX() < 0 && Areal.getY() < 0 && Areal.getZ() >= 0)
            {
                if(Areal.getX() < Areal.getY())
                {
                    i--;
                }
                else
                {
                    i += (mWidth-1) * 2 - 1;
                }
            }
        }
    }
}

bool curvedPlane::has_index(const triangle &t1, int i)
{
    for(int j = 0; j < 3; j++)
    {
        if(t1.n[j] == i)
            return true;
    }

    return false;
}

Vec3 curvedPlane::normalFromTriangle(const triangle &t)
{
    int i1 = t.n[0];
    int i2 = t.n[1];
    int i3 = t.n[2];

    Vec3 p0 = mVertices[i1].get_xyz();
    Vec3 p1 = mVertices[i2].get_xyz();
    Vec3 p2 = mVertices[i3].get_xyz();

    Vec3 U = p2 - p0;
    Vec3 V = p1 - p0;

    return (U^V).normalized();
}

void curvedPlane::topologicalSort()
{
    std::stack<int> topOrder;
    //Check which are visited
    bool* visited = new bool[triangles.size()];
    for(uint i = 0; i < triangles.size(); i++)
    {
        visited[i] = false;
    }
    for(uint i = 0; i < triangles.size(); i++)
    {
        if(!visited[i])
        {
            recursiveTopologicalSort(i, visited, topOrder);
        }
    }
    int k = 0;
    std::vector<triangle> sorted;

    while(!topOrder.empty())
    {
        sorted.push_back(triangles[topOrder.top()]);
        topOrder.pop();
    }

    for(int i = 0; i < sorted.size(); i++)
    {
        triangles[i] = sorted[i];
    }
}

void curvedPlane::recursiveTopologicalSort(int i, bool *visited, std::stack<int> &stack)
{
    visited[i] = true;
    if(i % 2 == 0)
    {
        if(triEdges[i].e[2] != -1)
        {
            if(!visited[triEdges[i].e[2]])
            {
                recursiveTopologicalSort(triEdges[i].e[2], visited, stack);
            }
        }
    }
    else
    {
        for(int j = 0; j < 2; j++)
        {
            if(triEdges[i].e[j] != -1)
            {
                if(!visited[triEdges[i].e[j]])
                {
                    recursiveTopologicalSort(triEdges[i].e[j], visited, stack);
                }
            }
        }
    }
    stack.push(i);
}

plane curvedPlane::getPlaneFromIndex(int i)
{
    triangle t = triangles[i];
    Vec3 p0 = mVertices[t.n[0]].get_xyz();
//    Vec3 p1 = mVertices[t.n[1]].get_xyz();
//    Vec3 p2 = mVertices[t.n[0]].get_xyz();
    Vec3 norm = normalFromTriangle(t);

    return plane(p0, norm);
}
