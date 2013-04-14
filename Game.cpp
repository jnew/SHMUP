#include "Game.h"

void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  mainWindow.create(sf::VideoMode(480,640,32),"SHMUP Super Alpha");
  mainWindow.setKeyRepeatEnabled(false);
  mainWindow.setFramerateLimit(0);
  //mainWindow.setVerticalSyncEnabled(true);
  gameState = Game::Uninitialized;

  while(!IsExiting())
  {
    GameLoop();
  }
  if(mainWindow.isOpen())
    mainWindow.close();
}

bool Game::IsExiting()
{
  if(gameState == Game::Exiting)
    return true;
  else
    return false;
}

bool Game::ShowSplashScreen()
{
    Splashscreen splash;
    return splash.Show(mainWindow);
}

void Game::CheckMovement(float playerSpeed, float frameTime)
{
    unsigned int Time = projClock.getElapsedTime().asMilliseconds();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.sprite.getPosition().y > (0))
    {
        player1.sprite.move(0.f, -playerSpeed*frameTime);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player1.sprite.getPosition().y < (600))
    {
        player1.sprite.move(0.f, playerSpeed*frameTime);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.sprite.getPosition().x > (0))
    {
        player1.sprite.move(-playerSpeed*frameTime, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.sprite.getPosition().x < (440))
    {
        player1.sprite.move(playerSpeed*frameTime, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == Playing && Time > (1000/60))
    {
        float velocity[] = {0,-20};
        Projectile* newProj = new Projectile(velocity, player1.sprite.getPosition().x+15, player1.sprite.getPosition().y, "images/PlayerProj.png", 5);
        projList.push_front(*newProj);
        std::cerr << "projectile made" << std::endl;

//        velocity[1] = -2;
//        Projectile* newProj2 = new Projectile(velocity, player1.sprite.getPosition().x, player1.sprite.getPosition().y+15);
//        projList.push_front(*newProj2);
//        std::cerr << "projectile made" << std::endl;

//        velocity[1] = 0;
//        Projectile* newProj3 = new Projectile(velocity, player1.sprite.getPosition().x, player1.sprite.getPosition().y+15);
//        projList.push_front(*newProj3);
//        std::cerr << "projectile made" << std::endl;

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
        if((*i).sprite.getPosition().x > 800 || (*i).sprite.getPosition().y > 600 || (*i).sprite.getPosition().y < 0)
        {
            (*i).offScreen = true;
        }
      }
    }
}

void Game::UpdateEnemies()
{
    std::list<Enemy>::iterator i;
    std::list<Projectile>::iterator j;
    for(i = enemyList.begin(); i != enemyList.end(); ++i)
    {
      if(!projList.empty() && !enemyList.empty())
      {
        for(j = projList.begin(); j != projList.end(); ++j)
        {
            if((*i).sprite.getGlobalBounds().intersects((*j).sprite.getGlobalBounds()))
            {
                (*i).takeDamage((*j).power);
                (*j).offScreen = true;
            }
        }
      }
    }
}

void Game::CleanUp()
{
    std::list<Enemy>::iterator i;
    std::list<Projectile>::iterator j;
    j = projList.begin();
    while(j != projList.end())
    {
            if((*j).offScreen == true)
            {
                projList.erase(j);
                std::cerr << "projectile erased" << std::endl;
                j = projList.begin();
            }
            else
                j++;
    }
    i = enemyList.begin();
    while(i != enemyList.end())
    {
            if((*i).isDestroyed == true)
            {
                enemyList.erase(i);
                std::cerr << "enemy erased" << std::endl;
                i = enemyList.begin();
            }
            else
                i++;
    }
}

void Game::DrawProj()
{
    if(!projList.empty())
    {
        std::list<Projectile>::iterator i;
        for(i = projList.begin(); i != projList.end(); i++)
        {
            (*i).Draw(mainWindow);
        }
    }
}

void Game::DrawEnemies()
{
    if(!enemyList.empty())
    {
        std::list<Enemy>::iterator i;
        for(i = enemyList.begin(); i != enemyList.end(); i++)
        {
            (*i).Draw(mainWindow);
        }
    }
}

void Game::GameLoop()
{
    float frameTime = frameClock.getElapsedTime().asSeconds();
    frameClock.restart();
    //check movement
    if (gameState != Paused)
        CheckMovement(300, frameTime);

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
            };
            case Game::ShowingSplash:
            {
                if(!ShowSplashScreen())
                    gameState = Exiting;
                else
                    gameState = Playing;
                //after splash is done, load player and background
                background.Load("images/Background.png");
                player1.Load("images/player.png");
                player1.SetPosition(220,580);

                Enemy* newEnemy = new Enemy(1, 400, 200);
                enemyList.push_front(*newEnemy);

                Enemy* newEnemy2 = new Enemy(1, 400, 280);
                enemyList.push_front(*newEnemy2);

                Enemy* newEnemy3 = new Enemy(1, 400, 360);
                enemyList.push_front(*newEnemy3);

                break;
            };
            case Game::Playing:
            {
                if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
                {
                    gameState = Game::Exiting;
                }
                if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::P)))
                {
                    gameState = Game::Paused;
                }
                break;
            };
            case Game::Paused:
            {
                if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::P)))
                {
                    gameState = Game::Playing;
                }
                break;
            };
        }
    }

    if(!projList.empty() && gameState != Paused)
    {
      UpdateProj();
    }

    if(gameState != Paused)
        UpdateEnemies();

    CleanUp();

    //draw game
    mainWindow.clear(sf::Color(0,0,0));
    if(gameState == Playing)
    {
        char fps[16];
        sprintf(fps,"%.2f",(1/frameTime));
        sf::Text text(fps);
        text.setCharacterSize(24);
        text.setStyle(sf::Text::Bold);
        text.setColor(sf::Color::Yellow);
        text.setPosition(5, 0);

        char infoBuffer[32];
        sprintf(infoBuffer,"ESC to Quit\nP to Pause");
        sf::Text info(infoBuffer);
        info.setCharacterSize(24);
        //info.setStyle(sf::Text::Bold);
        info.setColor(sf::Color::Yellow);
        info.setPosition(5, 40);

        mainWindow.draw(text);
        mainWindow.draw(info);
    }
    //background.Draw(mainWindow);
    DrawProj();
    DrawEnemies();
    player1.Draw(mainWindow);
    mainWindow.display();
}


Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
Player Game::player1;
Background Game::background;
std::list<Projectile> Game::projList;
std::list<Enemy> Game::enemyList;
sf::Clock Game::projClock;
sf::Clock Game::frameClock;
