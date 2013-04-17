#ifndef PLAYER_H
#define PLAYER_H

#include "VisibleObject.h"
#include "Projectile.h"
#include <list>

class Player : public VisibleObject
{
public:
    Player();
    ~Player();
    bool destroyCheck();
    void loseLife();
    void revive();
private:
    sf::Texture thisWontWork;
    float projVelocity[2];
    bool isDestroyed;
};

#endif // PLAYER_H
