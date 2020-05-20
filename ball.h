#ifndef BALL_H
#define BALL_H

#include "octahedron.h"
#include "renderwindow.h"
#include "startmenudialog.h"
#include "mainwindow.h"

class Vec3;
class Shader;
class Transform;

class Ball : public Octahedron
{
public:
    Ball(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    ~Ball(){}

    void update(float deltaTime) override;
    void switchSmooth() override{}
    void collisionCheck(SceneObject* Object) override;

    void addVelocity(Vec3 v);
    Vec3 getVelocity();
    void setVelocity(Vec3 v);
    int trohpiesCollected = 0;
    int enemyHits = 0;

private:
    float mGravity = -9.81f;

    Vec3 mVelocity;
    Vec3 lastFramePos;
    RenderWindow *mRenderWindow;
    startMenuDialog *end;
    MainWindow *textUpdate;
};

#endif // BALL_H
