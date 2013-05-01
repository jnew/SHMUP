#include "Game.h"

//intitalize window and window options, calls main loop
void Game::Start(void)
{
  if(gameState != Uninitialized)
    return;

  //make the window
  mainWindow.create(sf::VideoMode(1024,768,32),"Ace SPACE Man"); //playing field is 576 by 768
  mainWindow.setView(View);
  mainWindow.setKeyRepeatEnabled(false);
  mainWindow.setFramerateLimit(60);
  mainWindow.setMouseCursorVisible(false);
  playArea.setSize(sf::Vector2f(576,768));
  playArea.setFillColor(sf::Color::Blue);
  spawnArea.setSize(sf::Vector2f(576,400));
  spawnArea.setPosition(0,-400);
  spawnArea.setFillColor(sf::Color::Green);
  wholeArea.setSize(sf::Vector2f(1024,768));
  wholeArea.setFillColor(sf::Color::Black);
  //mainWindow.setVerticalSyncEnabled(true);
  gameState = Uninitialized;

  //load our main font
  uni05.loadFromFile("uni05_53.ttf");

  //testing optimization
  textures[0].loadFromFile("images/proj.png");
  textures[1].loadFromFile("images/bullet1.jpg");

  //load some sounds
  sf::SoundBuffer projSound;
  projSound.loadFromFile("sounds/se_plst00.wav");
  sounds[0].setBuffer(projSound);
  sounds[0].setVolume(20);

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
    if(image.loadFromFile("images/background0.png") != true)
    {
        return 0;
    }

    sf::Texture texture;
    if(texture.loadFromImage(image) != true)
    {
        return 0;
    }

    sf::String titleText("Ace SPACE Pilot");
    sf::Text title(titleText, uni05);
    title.setFont(uni05);
    title.setCharacterSize(40);
    title.setStyle(sf::Text::Bold);
    title.setColor(sf::Color::White);
    title.setPosition(288-(title.getGlobalBounds().width/2), 300);

    sf::Sprite sprite(texture);
    mainWindow.draw(wholeArea);
    mainWindow.draw(playArea);
    mainWindow.draw(spawnArea);
    background0.Draw(mainWindow);
    background1.Draw(mainWindow);
    background2.Draw(mainWindow);
    background3.Draw(mainWindow);
    mainWindow.draw(title);
    mainWindow.display();

    sf::String prompt("Press Enter to Begin");
    sf::Text text(prompt, uni05);
    text.setFont(uni05);
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setPosition(288-text.getGlobalBounds().width/2, 728);

    music[0].openFromFile("sounds/menu_bgm.ogg");
    music[0].play();

    music[1].openFromFile("sounds/stage_bgm.ogg");
    music[1].setVolume(30);

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
               music[0].stop();
               sounds[1].play();
               music[1].play();
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
            mainWindow.draw(wholeArea);
            mainWindow.draw(playArea);
            mainWindow.draw(spawnArea);
            background0.Draw(mainWindow);
            background1.Draw(mainWindow);
            background2.Draw(mainWindow);
            background3.Draw(mainWindow);
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
    float moveDistance = playerSpeed;
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameState == Playing)
    {
        player1.fireProjectile(textures[0]);
        if(sounds[0].getStatus() != sf::Sound::Playing)
            sounds[0].play();
    }

}

//update projectiles
void Game::UpdateProj()
{
    player1.updateProjectiles();
    std::list<Enemy>::iterator i = enemyList.begin();
    for(i = enemyList.begin(); i != enemyList.end(); i++)
    {
        (*i).updateProjectiles();
    }
}

//detect collisions with player or projectiles, calculate damage and flag destroyed things, update velocities
void Game::UpdateEnemies()
{
    std::list<Enemy>::iterator i;
    for(i = enemyList.begin(); i != enemyList.end(); ++i)
    {
      if(!enemyList.empty())
      {
          bool hit = player1.checkProjCollision((*i), scoreboard);
          if(sounds[3].getStatus() != sf::Sound::Playing && hit)
              sounds[3].play();
      }

      if((*i).getType() == 2)
          (*i).trackPlayer(player1);

      (*i).checkProjCollision(player1);

      (*i).updatePosition();

      (*i).fireProjectile(textures, 2);

      if((*i).sprite.getGlobalBounds().intersects(player1.sprite.getGlobalBounds()) && !(*i).destroyCheck())
      {
          player1.loseLife();
      }
    }        
}

//delete anything flagged for deletion
void Game::CleanUp()
{
    player1.cleanProjectiles();
    std::list<Enemy>::iterator i = enemyList.begin();
    for(i = enemyList.begin(); i != enemyList.end(); i++)
    {
        (*i).cleanProjectiles();
    }

    i = enemyList.begin();
    while(i != enemyList.end())
    {
        if((*i).destroyCheck())
            scoreboard.updateScore((*i).getScore());
        if((*i).destroyCheck() && (*i).projList.empty())
        {
            enemyList.erase(i);
            i = enemyList.begin();
        }
        else
            i++;
    }

    if(enemyList.empty() && gameState == Playing)
    {
        Enemy* newEnemy = new Enemy(3, 576/2, -100);
        newEnemy->setDestination(576/2, 200);
        enemyList.push_front(*newEnemy);

        Enemy* newEnemy2 = new Enemy(2, 0, -100);
        newEnemy2->setDestination(200, 300);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy3 = new Enemy(2, 576, -100);
        newEnemy3->setDestination(350, 300);
        enemyList.push_front(*newEnemy3);

        Enemy* newEnemy4 = new Enemy(2, 60, -100);
        newEnemy4->setDestination(200, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 536, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);

        Enemy* newEnemy6 = new Enemy(1, 30, 100);
        newEnemy6->setDestination(200, 300);
        enemyList.push_front(*newEnemy6);

        Enemy* newEnemy7 = new Enemy(1, 506, 100);
        newEnemy7->setDestination(350, 300);
        enemyList.push_front(*newEnemy7);
    }
}

//draw projectiles
void Game::DrawProj()
{
    player1.drawProjectiles(mainWindow);
    std::list<Enemy>::iterator i = enemyList.begin();
    for(i = enemyList.begin(); i != enemyList.end(); i++)
    {
        (*i).drawProjectiles(mainWindow);
    }
}

//draw enemies
void Game::DrawEnemies()
{
    std::list<Enemy>::iterator i;
    for(i = enemyList.begin(); i != enemyList.end(); i++)
    {
        if(!(*i).destroyCheck())
            (*i).drawEnemy(mainWindow);
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
            {
                scoreboard.clear();
                gameState = Uninitialized;
            }
            if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
            {
                gameState = Exiting;
            }
            break;
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
            if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
            {
                gameState = Exiting;
            }
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
        background0.Load("images/background0.png");
        background1.Load("images/background1.png");
        background2.Load("images/background2.png");
        background3.Load("images/background3.png");
        background0.sprite.setTextureRect(sf::Rect<int>(0,(1000-768),576,768));
        background1.sprite.setTextureRect(sf::Rect<int>(0,(1000-768),576,768));
        background2.sprite.setTextureRect(sf::Rect<int>(0,(1000-768),576,768));
        background3.sprite.setTextureRect(sf::Rect<int>(0,(1000-768),576,768));
        bgMove1 = 0;
        bgMove2 = 0;
        bgMove3 = 0;
        player1.Load("images/Player.png");
        player1.SetPosition(576/2,700);
        player1.revive();
        scoreboard.updateFont(&uni05);
        scoreboard.updateLives(1);
        scoreboard.updatePower(5);
        scoreboard.updateScore(000000);
        scoreboard.updateTargetHP(0,0);
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
        CheckMovement(5, frameTime);
        if(player1.destroyCheck())
        {
            enemyList.clear();
            mainWindow.clear();
            player1.clearProjectiles();
            music[1].stop();
            sounds[4].play();
            scoreboard.updateLives(-1);
            gameState = GameOver;
            break;
        }
        UpdateProj();
        UpdateEnemies();
        if(bgMove1 > (1000-768))
            bgMove1 = 0;
        if(bgMove2 > (1000-768))
            bgMove2 = 0;
        if(bgMove3 > (1000-768))
            bgMove3 = 0;
        bgMove1 += 20*frameTime;
        bgMove2 += 50*frameTime;
        bgMove3 += 150*frameTime;
        background1.sprite.setTextureRect(sf::Rect<int>(0,(1000-768)-bgMove1,576,768));
        background2.sprite.setTextureRect(sf::Rect<int>(0,(1000-768)-bgMove2,576,768));
        background3.sprite.setTextureRect(sf::Rect<int>(0,(1000-768)-bgMove3,576,768));
        break;
    }
    }

    //clean up sprites to be deleted
    CleanUp();

    //draw game
    mainWindow.clear(sf::Color(0,0,0));
    mainWindow.draw(wholeArea);
    mainWindow.draw(playArea);
    background0.Draw(mainWindow);
    background1.Draw(mainWindow);
    background2.Draw(mainWindow);
    char fps[32];
    sprintf(fps,"%.2f\nESC to Quit\nP to Pause",(float(1)/frameTime));
    sf::String fpsString(fps);
    sf::Text text(fpsString, uni05);
    text.setCharacterSize(18);
    //text.setStyle(sf::Text::Bold);
    text.setFont(uni05);
    text.setColor(sf::Color::Yellow);
    text.setPosition(586, 700);
    mainWindow.draw(text);
    if(gameState == GameOver)
    {
        sf::String gameOver("GAME OVER");
        sf::Text endingMessage(gameOver, uni05);
        endingMessage.setCharacterSize(40);
        endingMessage.setFont(uni05);
        endingMessage.setStyle(sf::Text::Bold);
        endingMessage.setColor(sf::Color::Red);
        endingMessage.setPosition(288-(endingMessage.getGlobalBounds().width/2), 300);

        mainWindow.draw(endingMessage);
    }
    DrawProj();
    DrawEnemies();
    if(player1.destroyCheck() == false)
        player1.Draw(mainWindow);
    background3.Draw(mainWindow);
    mainWindow.draw(spawnArea);
    scoreboard.drawScoreboard(mainWindow, player1.sprite);
    mainWindow.display();
}


Game::GameState Game::gameState;
sf::RenderWindow Game::mainWindow;
Player Game::player1;
Background Game::background0;
Background Game::background1;
Background Game::background2;
Background Game::background3;
Scoreboard Game::scoreboard;
std::list<Enemy> Game::enemyList;
sf::Clock Game::frameClock;
sf::RectangleShape Game::playArea;
sf::RectangleShape Game::spawnArea;
sf::RectangleShape Game::wholeArea;
sf::View Game::View(sf::FloatRect(0, 0, 1024, 768));
sf::Sound Game::sounds[5];
sf::Music Game::music[2];
sf::Texture Game::textures[2];
float Game::bgMove1;
float Game::bgMove2;
float Game::bgMove3;
sf::Font Game::uni05;

