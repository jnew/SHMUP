#ifndef GAME_H
#define GAME_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include <stdio.h>
#include <string>
#include <ctime>
#include <iostream>
#include <list>


class Game
{

public:
  static void Start();
  static sf::RectangleShape playArea;

private:
  static void GameLoop();
  static bool ShowSplashScreen();
  static void CheckMovement(float, float);
  static void UpdateProj();
  static void UpdateEnemies(float);
  static void DrawProj();
  static void DrawEnemies();
  static void CleanUp();

  enum GameState { Uninitialized, ShowingSplash, Paused,
          ShowingMenu, Playing, GameOver, Exiting };

  static GameState gameState;
  static sf::RenderWindow mainWindow;
  static Player player1;
  static Background background;
  static std::list<Projectile> projList;
  static std::list<Enemy> enemyList;
  static sf::Clock projClock;
  static sf::Clock frameClock;
  static sf::View View;
  static sf::Sound sounds[5];
};


#endif // GAME_H
