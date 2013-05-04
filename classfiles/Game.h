#ifndef GAME_H
#define GAME_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Background.h"
#include "Scoreboard.h"
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
  static sf::RectangleShape spawnArea;
  static sf::RectangleShape wholeArea;

private:
  static void GameLoop();
  static bool ShowSplashScreen();
  static void CheckMovement(float, float);
  static void UpdateProj();
  static void UpdateEnemies();
  static void DrawProj();
  static void DrawEnemies();
  static void CleanUp();

  enum GameState { Uninitialized, ShowingSplash, Paused, Playing, GameOver, Exiting };

  static GameState gameState;
  static sf::RenderWindow mainWindow;
  static sf::View View;
  static Player player1;
  static Background background0;
  static Background background1;
  static Background background2;
  static Background background3;
  static Scoreboard scoreboard;
  static std::list<Enemy> enemyList;
  static sf::Clock frameClock;
  static sf::Sound sounds[5];
  static sf::Music music[2];
  static sf::Texture textures[2];
  static float bgMove0;
  static float bgMove1;
  static float bgMove2;
  static sf::Font uni05;
};


#endif // GAME_H
