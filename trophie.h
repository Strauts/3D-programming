#ifndef TROPHIE_H
#define TROPHIE_H

#include "triangle.h"
#include "pyramid.h"

class trophie : public Pyramid/*Triangle*/
{
public:
    trophie(RenderWindow* Owner, Vec3 Position, Quaternion Rotation);
    ~trophie();
    void update(float deltaTime) override;

};

#endif // TROPHIE_H
