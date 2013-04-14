#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "VisibleObject.h"

class Projectile: public VisibleObject
{
public:
    Projectile();
    Projectile(float[], float, float, char *filePath, int);
    ~Projectile();
    void setVelocity(float[]);
    void updatePosition();
    bool offScreen;
    int power;
private:
    float velocity[2];
};

#endif // PROJECTILE_H
