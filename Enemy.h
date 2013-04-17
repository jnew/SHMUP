#ifndef ENEMY_H
#define ENEMY_H

#include "VisibleObject.h"
#include <cmath>

class Enemy : public VisibleObject
{
public:
    Enemy();
    Enemy(int, float, float);
    ~Enemy();
    int getScore();
    int getType();
    int getHitPoints();
    int getInitHealth();
    void takeDamage(int);
    void updatePosition();
    void setDestination(float, float);
    void setVelocity(float, float);
    void trackPlayer(float, float);
    bool fire();
    bool destroyCheck();
    sf::Clock timeSinceHit;
private:
    bool isDestroyed;
    int enemyType;
    int score;
    int fireClock;
    int hitPoints;
    int startingHealth;
    float pixelsPerFrame;
    float velocity[2];
    float destination[2];
};

#endif // ENEMY_H
