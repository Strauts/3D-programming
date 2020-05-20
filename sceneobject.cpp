#include "sceneobject.h"
#include "vertex.h"
#include "renderwindow.h"
#include "curvedplane.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(RenderWindow *Owner)
    : mOwner{Owner}
{}

SceneObject::~SceneObject()
{
    //std::cout << "SceneObject deleted\n";
}

void SceneObject::collisionCheck(SceneObject *other)
{}

Transform& SceneObject::getTransform()
{
    return transform;
}

Vertex *SceneObject::vertices() const
{
    return mVertices;
}

Vec3* SceneObject::normalAverage()
{
    // Find normals that share points
    Vec3* resultNormals = new Vec3[mNumberOfVertices];

    bool* complete = new bool[mNumberOfVertices]{false};

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        if(!complete[i])
        {
            complete[i] = true;

            std::vector<Vertex*> pointArray;
            std::vector<uint> pointIndex;
            std::vector<Vec3> unique;

            for(int j = 0; j < mNumberOfVertices; j++)
            {
                Vec3 vec1 = mVertices[j].get_xyz();
                Vec3 vec2 = mVertices[i].get_xyz();

                if(vec1 == vec2)
                {
                    bool add = true;

                    for(uint k = 0; k < pointArray.size(); k++)
                    {
                        if(pointArray[k]->get_normal() == mVertices[j].get_normal())
                            add = false;
                    }

                    pointArray.push_back(&mVertices[j]);
                    pointIndex.push_back(j);
                    if(add)
                        unique.push_back(mVertices[j].get_normal());

                    complete[j] = true;
                }
            }

            Vec3 sum;

            for(Vec3 norm : unique)
                sum = sum + norm;

            sum.normalize();

            for(uint ind : pointIndex)
                resultNormals[ind] = sum;
        }
    }

    return resultNormals;
}

void SceneObject::update(float deltaTime)
{
    switchSmooth();

    glBindVertexArray(mVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices * sizeof(Vertex), vertices(), GL_DYNAMIC_DRAW);
}

void SceneObject::switchSmooth()
{}

void SceneObject::setAvgNormalToggle(bool nT)
{
    normalToggle = nT;
}

bool SceneObject::getAvgNormalToggle()
{
    return normalToggle;
}
