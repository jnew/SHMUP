#include "Projectile.h"

Projectile::Projectile()
{
    this->velocity[0] = 10;
    this->velocity[1] = 0;
    offScreen = false;
}

Projectile::Projectile(float velocity[2], float x, float y, sf::Texture &texture, int power, bool isEnemy)
{
    sprite.setTexture(texture);
    loadToggle();
    SetPosition(x, y);

    initialVelocity[0] = velocity[0];
    initialVelocity[1] = velocity[1];
    this->velocity[0] = initialVelocity[0];
    this->velocity[1] = initialVelocity[1];

    offScreen = false;
    this->power = power;
    this->isEnemy = isEnemy;
    sprite.setOrigin(sprite.getLocalBounds().width/2,sprite.getLocalBounds().height/2);
}

Projectile::~Projectile()
{
}

void Projectile::updatePosition()
{
    this->velocity[0] = initialVelocity[0];
    this->velocity[1] = initialVelocity[1];
    sprite.move(velocity[0], velocity[1]);
    //std::cerr << "updated!" << std::endl;
}

void Projectile::setVelocity(float velocity[2])
{
    this->velocity[0] = velocity[0];
    this->velocity[1] = velocity[1];
}
