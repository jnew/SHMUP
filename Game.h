#ifndef GAME_H
#define GAME_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Splashscreen.h"
#include "Background.h"
#include "Player.h"
#include "Projectile.h"
#include <ctime>
#include <iostream>
#include <list>


class Game
{

public:
  static void Start();

private:
  static bool IsExiting();
  static void GameLoop();
  static void ShowSplashScreen();
  static void CheckMovement(float);
  static void UpdateProj();
  static void DrawProj();

  enum GameState { Uninitialized, ShowingSplash, Paused,
          ShowingMenu, Playing, Exiting };

  static GameState gameState;
  static sf::RenderWindow mainWindow;
  static Player player1;
  static Background background;
  static std::list<Projectile> projList;
  static sf::Clock clock;
};


#endif // GAME_H
