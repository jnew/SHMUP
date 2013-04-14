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
private:
    int hitPoints;
    int enemyType;
};

#endif // ENEMY_H
