#include "Projectile.h"

Projectile::Projectile()
{
    this->velocity[0] = 10;
    this->velocity[1] = 0;
    offScreen = false;
}

Projectile::Projectile(float velocity[2], float x, float y, std::string filePath, int power)
{
    Load(filePath);
    this->velocity[0] = velocity[0];
    this->velocity[1] = velocity[1];
    SetPosition(x, y);
    offScreen = false;
    this->power = power;
}

Projectile::~Projectile()
{
}

void Projectile::updatePosition()
{
    sprite.move(int(velocity[0]), int(velocity[1]));
}

void Projectile::setVelocity(float velocity[2])
{
    this->velocity[0] = velocity[0];
    this->velocity[1] = velocity[1];
}
