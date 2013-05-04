#include "Scoreboard.h"

Scoreboard::Scoreboard()
{
    //576 by 768 so board is 448 by 768
    score.setPosition(596, 10);
    lives.setPosition(646, 105);
    power.setPosition(596, 40);
    lifeSprite.setPosition(596, 100);
    targetHP.setPosition(596, 160);
    healthBar.setPosition(10,10);
    healthBar2.setPosition(10,10);
    healthBar3.setPosition(10,10);
    healthBar.setSize(sf::Vector2f(0,0));
    healthBar2.setSize(sf::Vector2f(0,0));
    healthBar3.setSize(sf::Vector2f(0,0));
    healthBar.setFillColor(sf::Color::Green);
    healthBar2.setFillColor(sf::Color::Yellow);
    healthBar3.setFillColor(sf::Color::Red);

    totalScore = 0;
    sprintf(scoreString,"Score: %06d", totalScore);
    this->score.setString(scoreString);
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
    //window.draw(targetHP);
    window.draw(healthBar3);
    window.draw(healthBar2);
    window.draw(healthBar);
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

bool Scoreboard::updateScore(unsigned int score)
{
    if(score > 0)
    {
        totalScore += score;
        sprintf(scoreString,"Score: %06d", totalScore);
        this->score.setString(scoreString);
        return 1;
    }
    else
        return 0;
}

void Scoreboard::updateTargetHP(unsigned int newHealth, unsigned int initHealth)
{
    if(newHealth < 0)
        currentHealth = 0;
    else
        currentHealth = newHealth;
    sprintf(healthString,"Target Health: %04d", currentHealth);
    this->targetHP.setString(healthString);

    float barOneW, barTwoW, barThreeW;
    if(newHealth > 10000)
    {
        barOneW = newHealth-10000;
        barTwoW = 5000;
        barThreeW = 5000;
    }
    else if(newHealth > 5000)
    {
        barOneW = 0;
        barTwoW = newHealth-5000;
        barThreeW = 5000;
    }
    else if(newHealth > 0)
    {
        barOneW = 0;
        barTwoW = 0;
        barThreeW = newHealth;
    }
    else
    {
        barOneW = 0;
        barTwoW = 0;
        barThreeW = 0;
    }
    healthBar.setSize(sf::Vector2f((barOneW/float(5000))*556, 5));
    healthBar2.setSize(sf::Vector2f((barTwoW/float(5000))*556, 5));
    healthBar3.setSize(sf::Vector2f((barThreeW/float(5000))*556, 5));
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
