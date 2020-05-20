#include "skybox.h"
//#include "vec3.h"
//#include "vec2.h"

SkyBox::SkyBox()
{
    mmVertices.insert( mmVertices.end(),
    {//Vertex data for front
     Vertex{Vec3(-1.f, -1.f, 1.f),    Vec3(0.f, 0.f, 1.0f),  Vec2(0.25f, 0.333f)},  //v0
     Vertex{Vec3( 1.f, -1.f, 1.f),    Vec3(0.f, 0.f, 1.0f),  Vec2(0.5f,  0.333f)},  //v1
     Vertex{Vec3(-1.f,  1.f, 1.f),    Vec3(0.f, 0.f, 1.0f),  Vec2(0.25f, 0.666f)},    //v2
     Vertex{Vec3( 1.f,  1.f, 1.f),    Vec3(0.f, 0.f, 1.0f),  Vec2(0.5f,  0.666f)},    //v3

     //Vertex data for right
     Vertex{Vec3(1.f, -1.f,  1.f),    Vec3(1.f, 0.f, 0.f),   Vec2(0.5f,  0.333f)},  //v4
     Vertex{Vec3(1.f, -1.f, -1.f),    Vec3(1.f, 0.f, 0.f),   Vec2(0.75f, 0.333f)},  //v5
     Vertex{Vec3(1.f,  1.f,  1.f),    Vec3(1.f, 0.f, 0.f),   Vec2(0.5f,  0.666f)},    //v6
     Vertex{Vec3(1.f,  1.f, -1.f),    Vec3(1.f, 0.f, 0.f),   Vec2(0.75f, 0.666f)},    //v7

     //Vertex data for back
     Vertex{Vec3( 1.f, -1.f, -1.f),   Vec3(0.f, 0.f, -1.f),  Vec2(0.75f, 0.333f)},  //v8
     Vertex{Vec3(-1.f, -1.f, -1.f),   Vec3(0.f, 0.f, -1.f),  Vec2(1.f,   0.333f)},  //v9
     Vertex{Vec3( 1.f,  1.f, -1.f),   Vec3(0.f, 0.f, -1.f),  Vec2(0.75f, 0.666f)},    //v10
     Vertex{Vec3(-1.f,  1.f, -1.f),   Vec3(0.f, 0.f, -1.f),  Vec2(1.f,   0.666f)},    //v11

     //Vertex data for left
     Vertex{Vec3(-1.f, -1.f, -1.f),   Vec3(-1.f, 0.f, 0.f),  Vec2(0.f, 0.333f)},    //v12
     Vertex{Vec3(-1.f, -1.f,  1.f),   Vec3(-1.f, 0.f, 0.f),  Vec2(0.25f, 0.333f)},  //v13
     Vertex{Vec3(-1.f,  1.f, -1.f),   Vec3(-1.f, 0.f, 0.f),  Vec2(0.f, 0.666f)},      //v14
     Vertex{Vec3(-1.f,  1.f,  1.f),   Vec3(-1.f, 0.f, 0.f),  Vec2(0.25f,   0.666f)},  //v15

     //Vertex data for bottom
     Vertex{Vec3(-1.f, -1.f, -1.f),   Vec3(0.f, -1.f, 0.f),  Vec2(0.25f, 0.f)},      //v16
     Vertex{Vec3( 1.f, -1.f, -1.f),   Vec3(0.f, -1.f, 0.f),  Vec2(0.5f,  0.f)},      //v17
     Vertex{Vec3(-1.f, -1.f,  1.f),   Vec3(0.f, -1.f, 0.f),  Vec2(0.25f, 0.333f)},  //v18
     Vertex{Vec3( 1.f, -1.f,  1.f),   Vec3(0.f, -1.f, 0.f),  Vec2(0.5f,  0.333f)},  //v19

     //Vertex data for top
     Vertex{Vec3(-1.f, 1.f,  1.f),    Vec3(0.f, 1.f, 0.f),   Vec2(0.25f, 0.666f)},    //v20
     Vertex{Vec3( 1.f, 1.f,  1.f),    Vec3(0.f, 1.f, 0.f),   Vec2(0.5f,  0.666f)},    //v21
     Vertex{Vec3(-1.f, 1.f, -1.f),    Vec3(0.f, 1.f, 0.f),   Vec2(0.25f, 0.999f)},      //v22
     Vertex{Vec3( 1.f, 1.f, -1.f),    Vec3(0.f, 1.f, 0.f),   Vec2(0.5f,  0.999f)}       //v23
                      });

    mmIndices.insert( mmIndices.end(),
    { 0,  1,  2,  3,  3,      //Face 0 - triangle strip (v0,  v1,  v2,  v3)
      4,  4,  5,  6,  7,  7,  //Face 1 - triangle strip (v4,  v5,  v6,  v7)
      8,  8,  9, 10, 11, 11,  //Face 2 - triangle strip (v8,  v9, v10,  v11)
      12, 12, 13, 14, 15, 15, //Face 3 - triangle strip (v12, v13, v14, v15)
      16, 16, 17, 18, 19, 19, //Face 4 - triangle strip (v16, v17, v18, v19)
      20, 20, 21, 22, 23      //Face 5 - triangle strip (v20, v21, v22, v23)
                     });
    mMatrix.identity();
};

void SkyBox::createObject()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mmVertices.size()*sizeof(Vertex), mmVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mmIndices.size() * sizeof(GLuint), mmIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SkyBox::drawObject()
{
    glUseProgram(/*mMaterial.*/mShader->getProgram());
    glBindVertexArray( mVAO );
    //mMaterial.mShader->transmitUniformData(&mMatrix, &mMaterial);
    glDrawElements(GL_TRIANGLE_STRIP, mmIndices.size(), GL_UNSIGNED_INT, nullptr);
}
