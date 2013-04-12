#include "Projectile.h"

Projectile::Projectile()
{
    velocity = 5;
}

Projectile::Projectile(float velocity, float x, float y)
{
    Load("images/proj.png");
    this->velocity = velocity;
    SetPosition(x, y);
}

Projectile::~Projectile()
{
}

void Projectile::updatePosition()
{
    sprite.move(velocity, 0.f);
    //std::cerr << "position updated" << std::endl;
}

void Projectile::setOrigin(float x, float y)
{

}

void Projectile::setVelocity(float velocity)
{
    this->velocity = velocity;
}
