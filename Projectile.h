#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "VisibleObject.h"
#include "SFML/Graphics.hpp"
#include <iostream>

class Projectile: public VisibleObject
{
public:
    Projectile();
    Projectile(float[], float, float, sf::Texture &, int, bool);
    ~Projectile();
    void setVelocity(float[]);
    void updatePosition();
    bool offScreen;
    bool isEnemy;
    int power;
private:
    float initialVelocity[2];
    float velocity[2];
};

#endif // PROJECTILE_H
