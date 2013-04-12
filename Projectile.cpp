#include "Projectile.h"

Projectile::Projectile()
{
    this->velocity[0] = 10;
    this->velocity[1] = 0;
}

Projectile::Projectile(float velocity[2], float x, float y)
{
    Load("images/proj.png");
    this->velocity[0] = velocity[0];
    this->velocity[1] = velocity[1];
    SetPosition(x, y);
}

Projectile::~Projectile()
{
}

void Projectile::updatePosition()
{
    sprite.move(velocity[0], velocity[1]);
    //std::cerr << "position updated" << std::endl;
}

void Projectile::setOrigin(float x, float y)
{

}

void Projectile::setVelocity(float velocity[2])
{
    this->velocity[0] = velocity[0];
    this->velocity[1] = velocity[1];
}
