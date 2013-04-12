#include "Game.h"

void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  mainWindow.create(sf::VideoMode(800,600,32),"SHMUP Super Alpha");
  mainWindow.setKeyRepeatEnabled(false);
  mainWindow.setFramerateLimit(0);
  //mainWindow.setVerticalSyncEnabled(true);
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

void Game::CheckMovement(float playerSpeed, float frameTime)
{
    unsigned int Time = projClock.getElapsedTime().asMilliseconds();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.sprite.getPosition().y > (0))
    {
        player1.sprite.move(0.f, -playerSpeed*frameTime);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player1.sprite.getPosition().y < (560))
    {
        player1.sprite.move(0.f, playerSpeed*frameTime);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.sprite.getPosition().x > (0))
    {
        player1.sprite.move(-playerSpeed*frameTime, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.sprite.getPosition().x < (760))
    {
        player1.sprite.move(playerSpeed*frameTime, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == Playing && Time > 100)
    {
        float velocity[] = {10,2};
        Projectile* newProj = new Projectile(velocity, player1.sprite.getPosition().x, player1.sprite.getPosition().y);
        projList.push_front(*newProj);
        std::cerr << "projectile made" << std::endl;

        velocity[1] = -2;
        Projectile* newProj2 = new Projectile(velocity, player1.sprite.getPosition().x, player1.sprite.getPosition().y);
        projList.push_front(*newProj2);
        std::cerr << "projectile made" << std::endl;

        velocity[1] = 0;
        Projectile* newProj3 = new Projectile(velocity, player1.sprite.getPosition().x, player1.sprite.getPosition().y);
        projList.push_front(*newProj3);
        std::cerr << "projectile made" << std::endl;

        projClock.restart();
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
    float frameTime = frameClock.getElapsedTime().asSeconds();
    frameClock.restart();
    //check movement
    CheckMovement(400, frameTime);

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

    char fps[4];
    sprintf(fps,"%f",(1/frameTime));
    sf::Text text(fps);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::Red);

    //draw game
    mainWindow.clear(sf::Color(0,0,0));
    mainWindow.draw(text);
    //background.Draw(mainWindow);
    DrawProj();
    player1.Draw(mainWindow);
    mainWindow.display();

}


Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
Player Game::player1;
Background Game::background;
std::list<Projectile> Game::projList;
sf::Clock Game::projClock;
sf::Clock Game::frameClock;
