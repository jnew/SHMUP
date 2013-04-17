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
        Load("images/Seeker.png");
        SetPosition(x, y);
        hitPoints = 100;
        pixelsPerFrame = 0;
        score = 1000;
        break;
    }
    case 2: //tracker
    {
        Load("images/Seeker.png");
        SetPosition(x, y);
        hitPoints = 50;
        pixelsPerFrame = 5;
        score = 100;
        break;
    }
    case 3: //boss
    {
        Load("images/Boss.png");
        SetPosition(x, y);
        hitPoints = 500;
        pixelsPerFrame = 2;
        score = 500;
        break;
    }
    }
    startingHealth = hitPoints;
    velocity[0] = 0;
    velocity[1] = 0;
    isDestroyed = false;
    timeSinceHit.restart();
    fireClock = 0;
}

Enemy::~Enemy()
{

}

void Enemy::takeDamage(int damageNumber) //returns 1 if destroyed, 0 if not
{
    timeSinceHit.restart();
    switch(enemyType)
    {
    case 1: //dummy enemy
    {
        Load("images/SeekerDMG.png");
        break;
    }
    case 2: //tracker
    {
        Load("images/SeekerDMG.png");
        break;
    }
    case 3: //boss
    {
        Load("images/BossDMG.png");
        break;
    }
    }

    hitPoints -= damageNumber;
    if(hitPoints <= 0)
    {
        hitPoints = 0;
        isDestroyed = true;
    }
}

void Enemy::updatePosition()
{
    float xdist;
    float ydist;
    float hyp;

    float staticVelocity = pixelsPerFrame;

    if(enemyType != 2)
    {
        xdist = (destination[0] - sprite.getPosition().x);
        ydist = (destination[1] - sprite.getPosition().y);
        hyp = sqrt(xdist*xdist+ydist*ydist);
        velocity[0] = (xdist/hyp)*staticVelocity;
        //std::cerr << xdist << std::endl;
        velocity[1] = (ydist/hyp)*staticVelocity;
        if(abs(xdist) > 0)
            sprite.move(velocity[0], 0);
        if(abs(ydist) > 0)
            sprite.move(0, velocity[1]);
    }
    else
        sprite.move(velocity[0], velocity[1]);
}

void Enemy::setVelocity(float xvelocity, float yvelocity)
{
    this->velocity[0] = xvelocity;
    this->velocity[1] = yvelocity;
}

void Enemy::setDestination(float x, float y)
{
    destination[0] = x;
    destination[1] = y;
}

void Enemy::trackPlayer(float playerx, float playery)
{
    float xdist;
    float ydist;
    float hyp;

    float staticVelocity = pixelsPerFrame;

    xdist = playerx - sprite.getPosition().x;
    ydist = playery - sprite.getPosition().y;
    hyp = sqrt(xdist*xdist+ydist*ydist);
    velocity[0] = (xdist/hyp)*staticVelocity;
    velocity[1] = (ydist/hyp)*staticVelocity;
}

bool Enemy::fire()
{
    if(enemyType == 3 && fireClock >= 30)
    {
        fireClock = 0;
        return true;
    }
    else
    {
        fireClock++;
        return false;
    }
}

int Enemy::getScore()
{
    return score;
}

int Enemy::getHitPoints()
{
    return hitPoints;
}

int Enemy::getInitHealth()
{
    return startingHealth;
}

int Enemy::getType()
{
    return enemyType;
}

bool Enemy::destroyCheck()
{
    return isDestroyed;
}
