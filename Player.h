#ifndef PLAYER_H
#define PLAYER_H

#include "VisibleObject.h"

class Player : public VisibleObject
{
public:
    Player();
    ~Player();
private:
    float xpos;
    float ypos;
};

#endif // PLAYER_H
