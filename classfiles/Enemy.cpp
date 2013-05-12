#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(int enemyType, float x, float y)
{
    pattern[0] = &Enemy::pattern0;
    pattern[1] = &Enemy::pattern1;
    pattern[2] = &Enemy::pattern2;
    this->enemyType = enemyType;
    loadToggle();
    SetPosition(x, y);
    switch(enemyType)
    {
    case 1: //fast shooter
    {
        Load("images/Shooter.png");
        center[0] = 50/2;
        center[1] = 48;
        hitPoints = 100;
        pixelsPerFrame = 5;
        score = 200;
        fires = true;
        patternNumber = 0;
        break;
    }
    case 2: //tracker
    {
        Load("images/Seeker.png");
        center[0] = 29/2;
        center[1] = 48;
        hitPoints = 150;
        pixelsPerFrame = 3;
        score = 100;
        fires = false;
        break;
    }
    case 3: //boss
    {
        Load("images/Boss.png");
        center[0] = 200/2;
        center[1] = 80/2;
        hitPoints = 2500;
        pixelsPerFrame = 2;
        score = 1000;
        fires = true;
        patternNumber = 2;
        break;
    }
    case 4: //circle shooter
    {
        Load("images/Shooter.png");
        center[0] = 50/2;
        center[1] = 48;
        hitPoints = 200;
        pixelsPerFrame = 2;
        score = 500;
        fires = true;
        patternNumber = 1;
        break;
    }
    }
    this->sprite.setOrigin(center[0],center[1]);
    rotation=0;
    startingHealth = hitPoints;
    velocity[0] = 0;
    velocity[1] = 0;
    isDestroyed = false;
    isScored = false;
    offScreen = false;
    fireDelay = 0;
    fireClock = 0;
    popped = 0;
    angle = 0;
}

Enemy::~Enemy()
{

}

void Enemy::fireProjectile(sf::Texture textureList[])
{
    if(fires && fire())
        (this->*pattern[patternNumber])(textureList);
    else if(enemyType == 2 && isDestroyed && !popped)
    {
        fireClock = 60;
        isDestroyed = false;
        popped = true;
        (this->*pattern[1])(textureList);
        isDestroyed = true;
    }
}

void Enemy::pattern0(sf::Texture textureList[])
{
    if(fireClock >= 50 && !isDestroyed)
    {
        float projVelocity[2];
        projVelocity[0] = -.5;
        projVelocity[1] = 4;
        for(int i = 0; i < 3; i++)
        {
        Projectile* newProj = new Projectile(projVelocity, sprite.getPosition().x, sprite.getPosition().y, textureList[1], 5, false);
        projList.push_front(*newProj);
        projVelocity[0] += .2;
        projVelocity[1] += .2;
        }
        fireClock = 0;
    }
    else
        fireClock++;
}

void Enemy::pattern1(sf::Texture textureList[])
{
    if(fireClock >= 60 && !isDestroyed)
    {
        float projVelocity[2];
        float magnitude = 3;
        angle = 3.14159/2;
        for(int i = 0; i < 10; i++)
        {
            projVelocity[0] = sin(angle)*magnitude;
            projVelocity[1] = cos(angle)*magnitude;
            Projectile* newProj = new Projectile(projVelocity, sprite.getPosition().x, sprite.getPosition().y, textureList[2], 5, false);
            projList.push_front(*newProj);
            angle += (3.14159)/5;
        }
        fireClock = 0;
    }
    else
        fireClock++;
}

void Enemy::pattern2(sf::Texture textureList[])
{
    if(fireClock >= 5 && !isDestroyed)
    {
        float projVelocity[2];
        float magnitude = 3;
        projVelocity[0] = sin(angle)*magnitude;
        projVelocity[1] = cos(angle)*magnitude;
        Projectile* newProj = new Projectile(projVelocity, sprite.getPosition().x, sprite.getPosition().y, textureList[1], 5, false);
        projList.push_front(*newProj);
        angle += (3.14159)/8;
        fireClock = 0;
    }
    else
        fireClock++;
}

void Enemy::updateProjectiles()
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

void Enemy::cleanProjectiles()
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

void Enemy::drawProjectiles(sf::RenderWindow &window)
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

bool Enemy::checkProjCollision(Player &player)
{
    bool hit = false;
    std::list<Projectile>::iterator j;
    for(j = projList.begin(); j != projList.end(); ++j)
    {
        if((*j).sprite.getGlobalBounds().intersects(sf::Rect<float>(player.sprite.getGlobalBounds().left+(15), player.sprite.getGlobalBounds().top+15,player.sprite.getGlobalBounds().width/4, player.sprite.getGlobalBounds().height/4)))
        {
            player.loseLife();
            (*j).offScreen = true;
            hit = true;
        }
    }
    return hit;
}

void Enemy::takeDamage(int damageNumber) //returns 1 if destroyed, 0 if not
{
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
    sprite.setRotation(rotation);
    if(sprite.getPosition().x < -200 || sprite.getPosition().x > 800 || sprite.getPosition().y < -400 || sprite.getPosition().y > 900)
    {
        offScreen = true;
        isDestroyed = true;
    }
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

void Enemy::setFireDelay(int delay)
{
    fireDelay = delay;
    //std::cerr << fireDelay << std::endl;
}

void Enemy::trackPlayer(Player &target)
{
    float xdist;
    float ydist;
    float hyp;

    float staticVelocity = pixelsPerFrame;

    xdist = target.sprite.getPosition().x - sprite.getPosition().x;
    ydist = target.sprite.getPosition().y - sprite.getPosition().y;
    hyp = sqrt(xdist*xdist+ydist*ydist);
    velocity[0] = (xdist/hyp)*staticVelocity;
    velocity[1] = (ydist/hyp)*staticVelocity;
    rotate(target);
}

bool Enemy::fire()
{
    if(fireDelay == 0)
    {
        return true;
    }
    else
    {
       //std::cerr << fireDelay << std::endl;
        fireDelay--;
        return false;
    }
}

void Enemy::drawEnemy(sf::RenderWindow &window)
{
    Draw(window);
}

void Enemy::rotate(Player &target)
{
    float xdist = target.sprite.getPosition().x - sprite.getPosition().x;
    float ydist = target.sprite.getPosition().y - sprite.getPosition().y;
    if(ydist > 0)
    {
        rotation = atan(xdist/ydist)* -57.296;
    }
    else
    {
        rotation = (atan(xdist/ydist)* -57.296) + 180;
    }
}

int Enemy::getScore()
{
    if(!isScored)
    {
        isScored = true;
        return score;
    }
    return 0;
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

bool Enemy::screenCheck()
{
    return offScreen;
}
