#include "Game.h"

//intitalize window and window options, calls main loop
void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  mainWindow.create(sf::VideoMode(1024,768,32),"SHMUP Super Alpha"); //playing field is 576 by 768
  mainWindow.setView(View);
  mainWindow.setKeyRepeatEnabled(false);
  mainWindow.setFramerateLimit(0);
  mainWindow.setMouseCursorVisible(false);
  playArea.setSize(sf::Vector2f(576,768));
  playArea.setFillColor(sf::Color::Blue);
  //mainWindow.setVerticalSyncEnabled(true);
  gameState = Uninitialized;

  //load some sounds
  sf::SoundBuffer projSound;
  projSound.loadFromFile("sounds/se_plst00.wav");
  sounds[0].setBuffer(projSound);

  sf::SoundBuffer okSound;
  okSound.loadFromFile("sounds/se_ok00.wav");
  sounds[1].setBuffer(okSound);

  sf::SoundBuffer pauseSound;
  pauseSound.loadFromFile("sounds/se_select00.wav");
  sounds[2].setBuffer(pauseSound);

  sf::SoundBuffer hitSound;
  hitSound.loadFromFile("sounds/se_damage00.wav");
  sounds[3].setBuffer(hitSound);

  sf::SoundBuffer endSound;
  endSound.loadFromFile("sounds/se_playerdead.wav");
  sounds[4].setBuffer(endSound);

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
    sf::Image image;
    if(image.loadFromFile("images/Background.png") != true)
    {
        return 0;
    }

    sf::Texture texture;
    if(texture.loadFromImage(image) != true)
    {
        return 0;
    }

    char titleText[32];
    sprintf(titleText,"SHMUP: THE GAME");
    sf::Text title(titleText);
    title.setCharacterSize(40);
    title.setStyle(sf::Text::Bold);
    title.setColor(sf::Color::White);
    title.setPosition(288-(title.getGlobalBounds().width/2), 300);

    sf::Sprite sprite(texture);
    mainWindow.draw(Game::playArea);
    mainWindow.draw(title);
    mainWindow.display();

    char prompt[32];
    sprintf(prompt,"Press Enter to Begin");
    sf::Text text(prompt);
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setPosition(288-text.getGlobalBounds().width/2, 728);

    sf::Event event;
    bool blinkCounter = 1;
    int spriteCount = 0;
    int start;
    while(true)
    {
        if(blinkCounter)
        {
            start = clock();
            blinkCounter = 0;
        }
        while(mainWindow.pollEvent(event))
        {
           if(event.type == sf::Event::Resized)
           {
                //std::cerr << "resized" << std::endl;
                float aspectRatio = float(event.size.width)/float(event.size.height);
                float newHeight = (1024.f*event.size.height)/event.size.width;
                float newWidth = (768.f*event.size.width)/event.size.height;
                if(aspectRatio > (4.f/3.f))
                {
                    float displace = (newWidth - 1024.f)/(-2.f);
                    View = sf::View(sf::FloatRect(displace, 0, newWidth, 768));
                }
                else if(aspectRatio < (4.f/3.f))
                {
                    float displace = (newHeight - 768.f)/(-2.f);
                    View = sf::View(sf::FloatRect(0, displace, 1024, newHeight));
                }
                mainWindow.setView(View);
           }
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)))
           {
               sounds[1].play();
                return 1;
           }
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) || event.type == sf::Event::Closed )
           {
             return 0;
           }
        }
        if(((clock()-start)/(double) CLOCKS_PER_SEC) >= 0.5)
        {
            mainWindow.clear();
            mainWindow.draw(Game::playArea);
            //window.draw(sprite);
            mainWindow.draw(title);
            if(spriteCount % 2 == 0)
            {
                mainWindow.draw(text);
            }
            mainWindow.display();
            spriteCount++;
            blinkCounter = 1;
        }
    }
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
        if(sounds[0].getStatus() != sf::Sound::Playing)
            sounds[0].play();
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
                if(sounds[3].getStatus() != sf::Sound::Playing)
                    sounds[3].play();
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
        //gotta handle resize shit here
        if(currentEvent.type == sf::Event::Resized)
        {
            //std::cerr << "resized" << std::endl;
            float aspectRatio = float(currentEvent.size.width)/float(currentEvent.size.height);
            float newHeight = (1024.f*currentEvent.size.height)/currentEvent.size.width;
            float newWidth = (768.f*currentEvent.size.width)/currentEvent.size.height;
            if(aspectRatio > (4.f/3.f))
            {
                float displace = (newWidth - 1024.f)/(-2.f);
                View = sf::View(sf::FloatRect(displace, 0, newWidth, 768));
            }
            else if(aspectRatio < (4.f/3.f))
            {
                float displace = (newHeight - 768.f)/(-2.f);
                View = sf::View(sf::FloatRect(0, displace, 1024, newHeight));
            }
            mainWindow.setView(View);
        }
        //broken fullscreen stuff, do not touch
        else if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::F))
        {
//            mainWindow.close();
//            std::vector<sf::VideoMode> FullscreenModes = sf::VideoMode::getFullscreenModes();
//            mainWindow.create(FullscreenModes[0],"SHMUP Super Alpha", sf::Style::Fullscreen);
        }

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
                sounds[2].play();
                gameState = Paused;
            }
            break;
        }
        case Paused:
        {
            if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::P))
            {
                sounds[2].play();
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
            sounds[4].play();
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
sf::View Game::View(sf::FloatRect(0, 0, 1024, 768));
sf::Sound Game::sounds[5];

