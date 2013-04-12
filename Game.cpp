#include "Game.h"

void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  mainWindow.create(sf::VideoMode(800,600,32),"SHMUP Super Alpha");
  mainWindow.setKeyRepeatEnabled(false);
  //mainWindow.setFramerateLimit(60);
  mainWindow.setVerticalSyncEnabled(true);
  gameState = Game::Uninitialized;

  while(!IsExiting())
  {
    GameLoop();
  }

  mainWindow.close();
}

bool Game::IsExiting()
{
  if(gameState == Game::Exiting)
    return true;
  else
    return false;
}

void Game::ShowSplashScreen()
{
    Splashscreen splash;
    splash.Show(mainWindow);
}

void Game::CheckMovement(float playerSpeed)
{
    unsigned int Time = clock.getElapsedTime().asMilliseconds();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.sprite.getPosition().y > (playerSpeed-1))
    {
        player1.sprite.move(0.f, -playerSpeed);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player1.sprite.getPosition().y < (560-(playerSpeed-1)))
    {
        player1.sprite.move(0.f, playerSpeed);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.sprite.getPosition().x > (playerSpeed-1))
    {
        player1.sprite.move(-playerSpeed, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.sprite.getPosition().x < (760-(playerSpeed-1)))
    {
        player1.sprite.move(playerSpeed, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == Playing && Time > 100)
    {
        Projectile* newProj = new Projectile(10, player1.sprite.getPosition().x, player1.sprite.getPosition().y);
        projList.push_front(*newProj);
        std::cerr << "projectile made" << std::endl;
        clock.restart();
    }
}

void Game::UpdateProj()
{
    std::list<Projectile>::iterator i;
    for(i = projList.begin(); i != projList.end(); ++i)
    {
      if(!projList.empty())
      {
        (*i).updatePosition();
        if((*i).sprite.getPosition().x > 800)
        {
            (*i).offScreen = true;
        }
      }
    }
}

void Game::DrawProj()
{
    if(!projList.empty())
    {
    std::list<Projectile>::iterator i;
    for(i = projList.begin(); i != projList.end(); i++)
    {
            if((*i).sprite.getPosition().x > 800)
            {
                projList.erase(i);
                std::cerr << "erased" << std::endl;
                break;
            }
            else
                (*i).Draw(mainWindow);
    }
    }
}

void Game::GameLoop()
{
  //check movement
  CheckMovement(6);

  //event loop
  sf::Event currentEvent;
  while(mainWindow.pollEvent(currentEvent))
  {
    switch(gameState)
    {
        case Game::Uninitialized:
        {
            gameState = ShowingSplash;
            break;
        }
        case Game::ShowingSplash:
        {
            ShowSplashScreen();
            //after splash is done, load player and background
            background.Load("images/Background.png");
            player1.Load("images/player.png");
            player1.SetPosition(30,280);
            gameState = Playing;
            break;
        }
        case Game::Playing:
        {
            if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
            {
                gameState = Game::Exiting;
            }
            break;
        }
    }
  }

    if(!projList.empty())
    {
      UpdateProj();
    }
    else
        std::cerr << "EMPTY" << std::endl;

  //draw game
  //mainWindow.clear(sf::Color(0,0,0));
  background.Draw(mainWindow);
  DrawProj();
  player1.Draw(mainWindow);
  mainWindow.display();

}


Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
Player Game::player1;
Background Game::background;
std::list<Projectile> Game::projList;
sf::Clock Game::clock;
