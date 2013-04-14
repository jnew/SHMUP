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
        break;
    }
    }
    isDestroyed = false;
}

Enemy::~Enemy()
{

}

bool Enemy::takeDamage(int damageNumber) //returns 1 if destroyed, 0 if not
{
    hitPoints -= damageNumber;
    if(hitPoints <= 0)
    {
        isDestroyed = true;
        return 1;
    }
    else
        return 0;
}
