#ifndef ENEMY_H
#define ENEMY_H

#include "VisibleObject.h"

class Enemy : public VisibleObject
{
public:
    Enemy();
    Enemy(int, float, float);
    ~Enemy();
    bool takeDamage(int);
    bool isDestroyed;
    sf::Clock timeSinceHit;
    void updatePosition();
    void setVelocity(float, float);
    void trackPlayer(float, float, float);
    int enemyType;
private:
    float velocity[2];
    int hitPoints;
    float pixelsPerSecond;
};

#endif // ENEMY_H
