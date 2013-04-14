#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(int enemyType, float x, float y)
{
    this->enemyType = enemyType;
    switch(enemyType)
    {
    case 1: //dummy enemy
    {
        Load("images/Sprite-0002.png");
        SetPosition(x, y);
        hitPoints = 100;
        pixelsPerSecond = 0;
        break;
    }
    case 2: //tracker
    {
        Load("images/Sprite-0002.png");
        SetPosition(x, y);
        hitPoints = 50;
        pixelsPerSecond = 250;
        break;
    }
    }
    velocity[0] = 0;
    velocity[1] = 0;
    isDestroyed = false;
    timeSinceHit.restart();
}

Enemy::~Enemy()
{

}

bool Enemy::takeDamage(int damageNumber) //returns 1 if destroyed, 0 if not
{
    timeSinceHit.restart();
    Load("images/Sprite_flash.png");
    hitPoints -= damageNumber;
    if(hitPoints <= 0)
    {
        isDestroyed = true;
        return 1;
    }
    else
        return 0;
}

void Enemy::updatePosition()
{
    sprite.move(int(velocity[0]), int(velocity[1]));
}

void Enemy::setVelocity(float xvelocity, float yvelocity)
{
    this->velocity[0] = xvelocity;
    this->velocity[1] = yvelocity;
}

void Enemy::trackPlayer(float frameTime, float playerx, float playery)
{
    float xdist;
    float ydist;
    float hyp;

    float staticVelocity = pixelsPerSecond*frameTime;

    xdist = playerx - sprite.getPosition().x;
    ydist = playery - sprite.getPosition().y;
    hyp = sqrt(xdist*xdist+ydist*ydist);
    velocity[0] = (xdist/hyp)*staticVelocity;
    velocity[1] = (ydist/hyp)*staticVelocity;

}
