#include "Player.h"

Player::Player()
{
    isDestroyed = false;
}

Player::~Player()
{
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

