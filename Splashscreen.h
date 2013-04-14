#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Game.h"
#include <ctime>
#include <stdio.h>

class Splashscreen
{
public:
    bool Show(sf::RenderWindow& window);
};

#endif // SPLASHSCREEN_H
