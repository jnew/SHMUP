#include "Game.h"

//intitalize window and window options, calls main loop
void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  mainWindow.create(sf::VideoMode(1024,768,32),"SHMUP Super Alpha"); //playing field is 576 by 768
  mainWindow.setKeyRepeatEnabled(false);
  mainWindow.setFramerateLimit(0);
  mainWindow.setMouseCursorVisible(false);
  playArea.setSize(sf::Vector2f(576,768));
  playArea.setFillColor(sf::Color::Blue);
  //mainWindow.setVerticalSyncEnabled(true);
  gameState = Uninitialized;

  while(gameState != Exiting)
  {
    GameLoop();
  }

  if(mainWindow.isOpen())
    mainWindow.close();
}

//shows splash screen (BLOCKING)
bool Game::ShowSplashScreen()
{
    Splashscreen splash;
    return splash.Show(mainWindow);
}

//checks for wasd and moves player accordingly
void Game::CheckMovement(float playerSpeed, float frameTime)
{
    unsigned int Time = projClock.getElapsedTime().asMilliseconds();
    float moveDistance = playerSpeed*frameTime;
    if((sf::Keyboard::isKeyPressed((sf::Keyboard::W)) ||  sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        moveDistance = moveDistance/(sqrt(2));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.sprite.getPosition().y > (0))
    {
        player1.sprite.move(0.f, -moveDistance);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player1.sprite.getPosition().y < (728))
    {
        player1.sprite.move(0.f, moveDistance);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.sprite.getPosition().x > (0))
    {
        player1.sprite.move(-moveDistance, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.sprite.getPosition().x < (536))
    {
        player1.sprite.move(moveDistance, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == Playing && Time >= (frameTime))
    {
        float velocity[] = {0,-20};
        std::string filePath = "images/PlayerProj.png";
        Projectile* newProj = new Projectile(velocity, player1.sprite.getPosition().x+15, player1.sprite.getPosition().y, filePath, 5);
        projList.push_front(*newProj);
        std::cerr << "projectile made" << std::endl;
        projClock.restart();
    }
}

//update projectiles
void Game::UpdateProj()
{
    std::list<Projectile>::iterator i;
    for(i = projList.begin(); i != projList.end(); ++i)
    {
      if(!projList.empty())
      {
        (*i).updatePosition();
        if((*i).sprite.getPosition().x > 576 || (*i).sprite.getPosition().y > 768 || (*i).sprite.getPosition().y < 0)
        {
            (*i).offScreen = true;
        }
      }
    }
}

//detect collisions with player or projectiles, calculate damage and flag destroyed things, update velocities
void Game::UpdateEnemies(float frameTime)
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
      if((*i).enemyType == 2)
          (*i).trackPlayer(frameTime, player1.sprite.getPosition().x, player1.sprite.getPosition().y);
      (*i).updatePosition();
      if((*i).sprite.getGlobalBounds().intersects(player1.sprite.getGlobalBounds()))
      {
          player1.isDestroyed = true;
      }
    }
}

//delete anything flagged for deletion
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

//draw projectiles
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

//draw enemies
void Game::DrawEnemies()
{
    if(!enemyList.empty())
    {
        std::list<Enemy>::iterator i;
        for(i = enemyList.begin(); i != enemyList.end(); i++)
        {
            if(projList.empty() || (*i).timeSinceHit.getElapsedTime().asMilliseconds() > 50)
            {
                (*i).Load("images/Sprite-0002.png");
            }
            (*i).Draw(mainWindow);
        }
    }
}

//MAIN GAME LOOP
void Game::GameLoop()
{
    //check time of last frame
    float frameTime = frameClock.getElapsedTime().asSeconds();
    frameClock.restart();

    //event loop
    sf::Event currentEvent;
    while(mainWindow.pollEvent(currentEvent))
    {
        switch(gameState)
        {
        case GameOver:
        {
            if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::Return))
                gameState = Uninitialized;
        }
        case Playing:
        {
            if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
            {
                gameState = Exiting;
            }
            if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::P))
            {
                gameState = Paused;
            }
            break;
        }
        case Paused:
        {
            if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::P))
            {
                gameState = Playing;
            }
            break;
        }
        }
    }

    //non event-triggered state stuff (every frame updates)
    switch(gameState)
    {
    case Uninitialized:
    {
        mainWindow.clear();
        background.Load("images/Background.png");
        player1.Load("images/player.png");
        player1.SetPosition(268,700);
        player1.isDestroyed = false;

        Enemy* newEnemy = new Enemy(2, 124, 100);
        enemyList.push_front(*newEnemy);

        Enemy* newEnemy2 = new Enemy(2, 268, 100);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy3 = new Enemy(2, 412, 100);
        enemyList.push_front(*newEnemy3);
    }
    case ShowingSplash:
    {
        if(!ShowSplashScreen())
            gameState = Exiting;
        else
        {
            gameState = Playing;
            frameClock.restart();
        }
        break;
    }
    case Playing:
    {
        //check movement
        CheckMovement(300, frameTime);
        if(player1.isDestroyed)
        {
            projList.clear();
            enemyList.clear();
            mainWindow.clear();
            gameState = GameOver;
            break;
        }
        if(!projList.empty())
            UpdateProj();
        UpdateEnemies(frameTime);
        break;
    }
    }

    //clean up sprites to be deleted
    CleanUp();

    //draw game
    mainWindow.clear(sf::Color(0,0,0));
    mainWindow.draw(playArea);
    if(gameState == Playing)
    {
        char fps[32];
        sprintf(fps,"%.2f\nESC to Quit\nP to Pause",(1/frameTime));
        sf::Text text(fps);
        text.setCharacterSize(18);
        //text.setStyle(sf::Text::Bold);
        text.setColor(sf::Color::Yellow);
        text.setPosition(5, 0);

        mainWindow.draw(text);
    }
    else if(gameState == GameOver)
    {
        char gameOver[16];
        sprintf(gameOver,"GAME OVER");
        sf::Text endingMessage(gameOver);
        endingMessage.setCharacterSize(30);
        endingMessage.setStyle(sf::Text::Bold);
        endingMessage.setColor(sf::Color::Red);
        endingMessage.setPosition(288-(endingMessage.getGlobalBounds().width/2), 300);

        mainWindow.draw(endingMessage);
    }
    DrawProj();
    DrawEnemies();
    if(player1.isDestroyed == false)
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
sf::RectangleShape Game::playArea;
