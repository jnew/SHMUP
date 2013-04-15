#include "Scoreboard.h"

Scoreboard::Scoreboard()
{
    //576 by 768 so board is 448 by 768
    score.setPosition(586, 10);
    lives.setPosition(636, 110);
    power.setPosition(586, 40);
    lifeSprite.setPosition(586, 100);
}

Scoreboard::Scoreboard(unsigned int score, unsigned int lives, unsigned int power)
{
    lifeSprite.setPosition(596, 100);

    char scoreString[32];
    sprintf(scoreString,"Score: %06d", score);
    this->score.setString(scoreString);

    char livesString[32];
    sprintf(livesString,"x %d", lives);
    this->lives.setString(livesString);

    char powerString[32];
    sprintf(powerString,"Power: %02d", power);
    this->power.setString(powerString);

    this->score.setPosition(596, 10);
    this->lives.setPosition(636, 110);
    this->power.setPosition(596, 60);

    this->score.setCharacterSize(25);
    this->lives.setCharacterSize(25);
    this->power.setCharacterSize(25);
}

Scoreboard::~Scoreboard()
{

}

void Scoreboard::drawScoreboard(sf::RenderWindow &window, sf::Sprite player)
{
    window.draw(score);
    lifeSprite.setTexture(*player.getTexture());
    window.draw(lifeSprite);
    window.draw(lives);
    window.draw(power);
}

void Scoreboard::updateLives(unsigned int lives)
{
    totalLives += lives;
    char livesString[32];
    sprintf(livesString,"x %d", totalLives);
    this->lives.setString(livesString);
}

void Scoreboard::updatePower(unsigned int power)
{
    currentPower = power;
    char powerString[32];
    sprintf(powerString,"Power: %02d", currentPower);
    this->power.setString(powerString);
}

void Scoreboard::updateScore(unsigned int score)
{
    totalScore += score;
    char scoreString[32];
    sprintf(scoreString,"Score: %06d", totalScore);
    this->score.setString(scoreString);
}

void Scoreboard::clear()
{
    totalScore = 0;
    totalLives = 0;
    currentPower = 0;
}
