#ifndef ENEMY_H
#define ENEMY_H

#include "octahedron.h"

class BezierCurve;

class Enemy : public Octahedron
{
public:
    Enemy(RenderWindow* Owner, Vec3 Pos1, Vec3 Pos2, Vec3 Pos3, Vec3 Pos4, int n);
    void createObject() override;
    void drawObject() override;
    void update(float deltaTime) override;
    Vec3 getNormal();

private:
    BezierCurve* mBCurve;
    bool forward {true};
    float Time {0.f};
};

#endif // ENEMY_H
