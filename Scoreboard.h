#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <iostream>
#include <list>

class Scoreboard
{
public:
    Scoreboard();
    Scoreboard(unsigned int, unsigned int, unsigned int);
    ~Scoreboard();
    void updateScore(unsigned int);
    void updateLives(unsigned int);
    void updatePower(unsigned int);
    void drawScoreboard(sf::RenderWindow&, sf::Sprite);
    void clear();
private:
    sf::Text score;
    sf::Text power;
    sf::Text lives;
    sf::Sprite lifeSprite;
    sf::Texture lifeTexture;
    unsigned int totalScore;
    unsigned int totalLives;
    unsigned int currentPower;
};

#endif // SCOREBOARD_H
