#ifndef PLAYER_H
#define PLAYER_H
class Enemy;

#include "SFML/Graphics.hpp"
#include "VisibleObject.h"
#include "Enemy.h"
#include "Scoreboard.h"
#include "Projectile.h"
#include <list>

class Player : public VisibleObject
{
public:
    Player();
    ~Player();

    void fireProjectile(sf::Texture &);
    void updateProjectiles();
    void cleanProjectiles();
    void drawProjectiles(sf::RenderWindow &);
    void clearProjectiles();
    bool checkProjCollision(Enemy &, Scoreboard &);

    bool destroyCheck();
    void loseLife();
    void revive();
    std::list<Projectile> projList;
private:
    float projVelocity[2];
    bool isDestroyed;
};

#endif // PLAYER_H
