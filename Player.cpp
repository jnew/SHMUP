#include "Player.h"

Player::Player()
{
    isDestroyed = false;
    sprite.setOrigin(38/2, 40/2);
}

Player::~Player()
{
}

void Player::fireProjectile(sf::Texture & texture)
{
    projVelocity[0] = 0;
    projVelocity[1] = -30;
    Projectile* newProj = new Projectile(projVelocity, sprite.getPosition().x+12, sprite.getPosition().y, texture, 5, false);
    projList.push_front(*newProj);
    Projectile* newProj2 = new Projectile(projVelocity, sprite.getPosition().x-12, sprite.getPosition().y, texture, 5, false);
    projList.push_front(*newProj2);
}

void Player::updateProjectiles()
{
    std::list<Projectile>::iterator i;
    for(i = projList.begin(); i != projList.end(); ++i)
    {
      if(!projList.empty())
      {
        (*i).updatePosition();
        if((*i).sprite.getPosition().x < 0 || (*i).sprite.getPosition().x > 576 || (*i).sprite.getPosition().y > 768 || (*i).sprite.getPosition().y < 0)
        {
            (*i).offScreen = true;
        }
      }
    }
}

void Player::cleanProjectiles()
{
    std::list<Projectile>::iterator j;
    j = projList.begin();
    while(j != projList.end())
    {
            if((*j).offScreen == true)
            {
                projList.erase(j);
                //std::cerr << "projectile erased" << std::endl;
                j = projList.begin();
            }
            else
                j++;
    }
}

void Player::drawProjectiles(sf::RenderWindow &window)
{
    if(!projList.empty())
    {
        std::list<Projectile>::iterator i;
        for(i = projList.begin(); i != projList.end(); i++)
        {
            (*i).Draw(window);
        }
    }
}

bool Player::checkProjCollision(Enemy &enemy, Scoreboard &scoreboard)
{
    bool hit = false;
    std::list<Projectile>::iterator j;
    for(j = projList.begin(); j != projList.end(); ++j)
    {
        if((*j).sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds()) && !enemy.destroyCheck())
        {
            enemy.takeDamage((*j).power);
            (*j).offScreen = true;
            hit = true;
            scoreboard.updateTargetHP(enemy.getHitPoints(), enemy.getInitHealth());
        }
    }
    return hit;
}

bool Player::destroyCheck()
{
    return isDestroyed;
}

void Player::loseLife()
{
    isDestroyed = true;
}

void Player::revive()
{
    isDestroyed = false;
}

