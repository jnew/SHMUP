#include "Scoreboard.h"

Scoreboard::Scoreboard()
{
    //576 by 768 so board is 448 by 768
    score.setPosition(586, 10);
    lives.setPosition(636, 110);
    power.setPosition(586, 40);
    lifeSprite.setPosition(586, 100);
    targetHP.setPosition(586, 160);
    healthRemaining.setTexture(healthBar);
    healthRemaining.setPosition(10, 10);
}

Scoreboard::Scoreboard(unsigned int score, unsigned int lives, unsigned int power)
{
    lifeSprite.setPosition(596, 100);
    currentHealth = 0;

    sprintf(scoreString,"Score: %06d", score);
    this->score.setString(scoreString);

    sprintf(livesString,"x %d", lives);
    this->lives.setString(livesString);

    sprintf(powerString,"Power: %02d", power);
    this->power.setString(powerString);

    sprintf(healthString,"Target Health: %04d", currentHealth);
    this->targetHP.setString(healthString);

    this->score.setPosition(596, 10);
    this->lives.setPosition(636, 110);
    this->power.setPosition(596, 60);
    this->targetHP.setPosition(596, 160);

    this->score.setCharacterSize(25);
    this->lives.setCharacterSize(25);
    this->power.setCharacterSize(25);
    this->targetHP.setCharacterSize(25);
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
    window.draw(targetHP);
    window.draw(healthRemaining);
}

void Scoreboard::updateLives(unsigned int lives)
{
    totalLives += lives;
    sprintf(livesString,"x %d", totalLives);
    this->lives.setString(livesString);
}

void Scoreboard::updatePower(unsigned int power)
{
    currentPower = power;
    sprintf(powerString,"Power: %02d", currentPower);
    this->power.setString(powerString);
}

void Scoreboard::updateScore(unsigned int score)
{
    totalScore += score;
    sprintf(scoreString,"Score: %06d", totalScore);
    this->score.setString(scoreString);
}

void Scoreboard::updateTargetHP(unsigned int newHealth, unsigned int initHealth)
{
    if(newHealth < 0)
        currentHealth = 0;
    else
        currentHealth = newHealth;
    sprintf(healthString,"Target Health: %04d", currentHealth);
    this->targetHP.setString(healthString);
    healthRemaining.setTextureRect(sf::Rect<int>(0,0,(float(newHealth)/float(5000))*500, 5));
}

void Scoreboard::updateFont(sf::Font* newFont)
{
    this->score.setFont(*newFont);
    this->lives.setFont(*newFont);
    this->power.setFont(*newFont);
    this->targetHP.setFont(*newFont);
}

void Scoreboard::clear()
{
    totalScore = 0;
    totalLives = 0;
    currentPower = 0;
    currentHealth = 0;
}
