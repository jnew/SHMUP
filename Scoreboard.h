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
    void updateTargetHP(unsigned int, unsigned int);
    void updateFont(sf::Font *);
    void drawScoreboard(sf::RenderWindow&, sf::Sprite);
    void clear();
private:
    sf::Text score;
    sf::Text power;
    sf::Text lives;
    sf::Text targetHP;
    sf::Sprite lifeSprite;
    sf::Sprite healthRemaining;
    sf::Image healthSprite;
    sf::Texture healthBar;
    char powerString[32];
    char livesString[32];
    char scoreString[32];
    char healthString[32];
    unsigned int totalScore;
    unsigned int totalLives;
    unsigned int currentPower;
    unsigned int currentHealth;
};

#endif // SCOREBOARD_H
