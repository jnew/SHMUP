#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "VisibleObject.h"
#include <iostream>

class Projectile: public VisibleObject
{
public:
    Projectile();
    Projectile(float, float, float);
    ~Projectile();
    void setOrigin(float, float);
    void setVelocity(float);
    void updatePosition();
    bool offScreen;
private:
    float velocity;
};

#endif // PROJECTILE_H
