#include "Game.h"

//intitalize window and window options, calls main loop
void Game::Start(void)
{
    if(gameState != Uninitialized)
    return;

    //set window icon
    icon.loadFromFile("images/Player.png");
    //make the window
    mainWindow.create(sf::VideoMode(1024,768,32),"Ace SPACE Pilot"); //playing field is 576 by 768
    mainWindow.setView(View);
    mainWindow.setIcon(40,40,icon.getPixelsPtr());
    mainWindow.setKeyRepeatEnabled(false);
    mainWindow.setFramerateLimit(60);
    mainWindow.setMouseCursorVisible(false);
    rightBound.setSize(sf::Vector2f(400,768));
    rightBound.setFillColor(sf::Color::Black);
    rightBound.setPosition(576,0);
    leftBound.setSize(sf::Vector2f(400,768));
    leftBound.setFillColor(sf::Color::Black);
    leftBound.setPosition(-400,0);
    bottomBound.setSize(sf::Vector2f(576,400));
    bottomBound.setFillColor(sf::Color::Black);
    bottomBound.setPosition(0,768);
    spawnArea.setSize(sf::Vector2f(676,400));
    spawnArea.setPosition(-50,-400);
    spawnArea.setFillColor(sf::Color::Black);
    wholeArea.setSize(sf::Vector2f(1024,768));
    wholeArea.setFillColor(sf::Color::Black);
    mainWindow.setVerticalSyncEnabled(true);
    gameState = Uninitialized;

    //load our main fonts
    uni05.loadFromFile("fonts/uni05_53.ttf");
    datagoth.loadFromFile("fonts/datagoth.otf");

    //load projectile textures
    textures[0].loadFromFile("images/proj.png");
    textures[1].loadFromFile("images/EnemyProj.png");
    textures[2].loadFromFile("images/EnemyProjSmall.png");

    //load sounds
    sf::SoundBuffer projSound;
    projSound.loadFromFile("sounds/se_plst00.wav");
    sounds[0].setBuffer(projSound);
    sounds[0].setVolume(20);

    sf::SoundBuffer okSound;
    okSound.loadFromFile("sounds/se_ok00.wav");
    sounds[1].setBuffer(okSound);
    sounds[1].setVolume(50);

    sf::SoundBuffer pauseSound;
    pauseSound.loadFromFile("sounds/se_select00.wav");
    sounds[2].setBuffer(pauseSound);

    sf::SoundBuffer hitSound;
    hitSound.loadFromFile("sounds/se_damage00.wav");
    sounds[3].setBuffer(hitSound);
    sounds[3].setVolume(20);

    sf::SoundBuffer endSound;
    endSound.loadFromFile("sounds/se_playerdead.wav");
    sounds[4].setBuffer(endSound);
    sounds[4].setVolume(50);

    sf::SoundBuffer killSound;
    killSound.loadFromFile("sounds/destroy.ogg");
    sounds[5].setBuffer(killSound);
    sounds[5].setVolume(50);

	//loads in story
    std::ifstream storyFile("storytime.txt");
    std::string storyString((std::istreambuf_iterator<char>(storyFile)), std::istreambuf_iterator<char>());
    story = wordWrap(storyString,48);

    if(mainWindow.setActive(true))
    {
        while(gameState != Exiting)
        {
            GameLoop();
        }
    }
  
    mainWindow.setActive(false);

    if(mainWindow.isOpen())
        mainWindow.close();
}

//show splash screen (BLOCKING)
bool Game::ShowSplashScreen()
{
    sf::String titleText("Ace SPACE Pilot");
    sf::Text title(titleText, datagoth);
    title.setCharacterSize(50);
    //title.setStyle(sf::Text::Bold);
    title.setColor(sf::Color::White);
    title.setPosition(288-(title.getGlobalBounds().width/2), 280);

    sf::String prompt("Press Enter to Begin");
    sf::Text text(prompt, datagoth);
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setPosition(288-text.getGlobalBounds().width/2, 725);

    sf::String fpsString("\nESC to Quit\nP to Pause\nSPACE to Fire\nWASD to Move");
    sf::Text text2(fpsString, datagoth);
    text2.setCharacterSize(20);
    text2.setColor(sf::Color::White);
    text2.setPosition(596, 650);

    music[0].openFromFile("sounds/menu_bgm.ogg");
	music[0].setVolume(60);
    music[0].play();

    music[1].openFromFile("sounds/stage_bgm.ogg");
    music[1].setVolume(50);

    sf::Event event;
    while(true)
    {
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
               sounds[1].play();
                return 1;
           }
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) || event.type == sf::Event::Closed )
           {
             return 0;
           }
        }

        //update paralaxed backgrounds
        if(bgMove1 > (2500-768))
            bgMove1 = 0;
        bgMove1 += 2;
        background1.sprite.setTextureRect(sf::Rect<int>(0,(2500-768)-bgMove1,576,768));

        mainWindow.draw(wholeArea);
        mainWindow.draw(spawnArea);
        background0.Draw(mainWindow);
        background1.Draw(mainWindow);
        mainWindow.draw(rightBound);
        mainWindow.draw(leftBound);
        mainWindow.draw(bottomBound);
        mainWindow.draw(title);
        mainWindow.draw(text);
        mainWindow.draw(text2);
        scoreboard.drawScoreboard(mainWindow, player1.sprite);
        mainWindow.display();
    }
}

//wraps long text to fit textbox
std::string Game::wordWrap(std::string str, unsigned int size)
{
    unsigned start = 0;
    while(1)
    {
        if(start+size > str.length())
            break;

        unsigned thispos = str.find_last_of('\n', start+size);
        if(thispos < size+start)
            start = thispos;
        thispos = str.find_last_of(' ', size+start);
        str.replace(thispos,1,"\n"); // makes last space in a line a newline char
        start = thispos;
    }

    return str;
}

//checks for wasd and moves player accordingly
void Game::CheckMovement(float playerSpeed, float frameTime)
{
    float moveDistance = playerSpeed;
    if((sf::Keyboard::isKeyPressed((sf::Keyboard::W)) ||  sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        moveDistance = moveDistance/(sqrt(2));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1.sprite.getPosition().y > (20))
    {
        player1.sprite.move(0.f, -moveDistance);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player1.sprite.getPosition().y < (748))
    {
        player1.sprite.move(0.f, moveDistance);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.sprite.getPosition().x > (20))
    {
        player1.sprite.move(-moveDistance, 0.f);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.sprite.getPosition().x < (556))
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

      (*i).fireProjectile(textures);

      if((*i).sprite.getGlobalBounds().intersects(player1.sprite.getGlobalBounds()) && !(*i).destroyCheck())
      {
          player1.loseLife();
      }
    }        
}

//delete anything flagged for deletion and spawn next wave if necessary
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
        if((*i).destroyCheck() && !(*i).screenCheck())
        {
            if(scoreboard.updateScore((*i).getScore()))
                sounds[5].play();
        }
        if((*i).destroyCheck() && (*i).projList.empty())
        {
            enemyList.erase(i);
            i = enemyList.begin();
        }
        else
            i++;
    }

//    bool newWave = 0;
//    for(i = enemyList.begin(); i != enemyList.end(); i++)
//    {
//        if(!(*i).destroyCheck() )
//        {
//            newWave = 0;
//            break;
//        }
//        else
//            newWave = 1;
//    }

//    if(newWave && gameState == Playing)
//    {
//        Enemy* newEnemy = new Enemy(3, 576/2, -100);
//        newEnemy->setDestination(576/2, 100);
//        enemyList.push_front(*newEnemy);

//        Enemy* newEnemy2 = new Enemy(2, 0, -100);
//        newEnemy2->setDestination(200, 300);
//        enemyList.push_front(*newEnemy2);

//        Enemy* newEnemy3 = new Enemy(4, 576, -100);
//        newEnemy3->setDestination(450, 200);
//        enemyList.push_front(*newEnemy3);

//        Enemy* newEnemy4 = new Enemy(4, 60, -100);
//        newEnemy4->setDestination(100, 200);
//        enemyList.push_front(*newEnemy4);

//        Enemy* newEnemy5 = new Enemy(2, 536, -100);
//        newEnemy5->setDestination(350, 300);
//        enemyList.push_front(*newEnemy5);

//        Enemy* newEnemy6 = new Enemy(1, 600, 100);
//        newEnemy6->setDestination(-500, 400);
//        enemyList.push_front(*newEnemy6);

//        Enemy* newEnemy7 = new Enemy(1, 675, 100);
//        enemyList.push_front(*newEnemy7);
//        newEnemy7->500(-setDestination, 400);

//        Enemy* newEnemy8 = new Enemy(1, 750, 100);
//        newEnemy8->setDestination(-500, 400);
//        enemyList.push_front(*newEnemy8);
//    }
}

void Game::CreateEnemy(unsigned int number, int startX, int startY, int destX, int destY, unsigned int delayFire)
{
    Enemy* newEnemy = new Enemy(number, startX, startY);
    newEnemy->setDestination(destX, destY);
    if(delayFire != 0)
        newEnemy->setFireDelay(delayFire);
    enemyList.push_front(*newEnemy);
}

void Game::Spawn(unsigned int frameCounter)
{
    if(frameCounter == 1)
    {
        Enemy* newEnemy4 = new Enemy(2, 200, -100);
        newEnemy4->setDestination(200, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 376, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 180)
    {
        Enemy* newEnemy4 = new Enemy(1, -100, 100);
        newEnemy4->setDestination(800, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 476, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 240)
    {
        Enemy* newEnemy4 = new Enemy(1, 676, 100);
        newEnemy4->setDestination(-200, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 100, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 320)
    {
        Enemy* newEnemy2 = new Enemy(4, 576/2, -100);
        newEnemy2->setDestination(576/2, 400);
        newEnemy2->setFireDelay(320);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy6 = new Enemy(1, 600, 100);
        newEnemy6->setDestination(-500, 400);
        enemyList.push_front(*newEnemy6);

        Enemy* newEnemy7 = new Enemy(1, 675, 100);
        newEnemy7->setDestination(-500, 400);
        enemyList.push_front(*newEnemy7);

        Enemy* newEnemy8 = new Enemy(1, 750, 100);
        newEnemy8->setDestination(-500, 400);
        enemyList.push_front(*newEnemy8);
    }
    else if(frameCounter == 440)
    {
        Enemy* newEnemy2 = new Enemy(4, 576/4, -100);
        newEnemy2->setDestination(576/4, 300);
        newEnemy2->setFireDelay(200);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy6 = new Enemy(4, 576*3/4, -100);
        newEnemy6->setDestination(576*3/4, 300);
        newEnemy6->setFireDelay(200);
        enemyList.push_front(*newEnemy6);

        Enemy* newEnemy7 = new Enemy(4, 526, -100);
        newEnemy7->setDestination(526, 200);
        newEnemy7->setFireDelay(200);
        enemyList.push_front(*newEnemy7);

        Enemy* newEnemy8 = new Enemy(4, 50, -100);
        newEnemy8->setDestination(50, 200);
        newEnemy8->setFireDelay(200);
        enemyList.push_front(*newEnemy8);

        Enemy* newEnemy3 = new Enemy(4, 576/2, -100);
        newEnemy6->setDestination(576/2, 100);
        newEnemy6->setFireDelay(200);
        enemyList.push_front(*newEnemy6);
    }
    else if(frameCounter == 1000)
    {
        Enemy* newEnemy2 = new Enemy(3, (576/4)-50, 800);
        newEnemy2->setDestination(576/4, 100);
        newEnemy2->setFireDelay(240);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy6 = new Enemy(3, (576*3/4)+50, 800);
        newEnemy6->setDestination(576*3/4, 100);
        newEnemy6->setFireDelay(240);
        enemyList.push_front(*newEnemy6);
    }
    else if(frameCounter == 1100)
    {
        Enemy* newEnemy2 = new Enemy(3, 576/2, 800);
        newEnemy2->setDestination(576/2, 200);
        newEnemy2->setFireDelay(140);
        enemyList.push_front(*newEnemy2);
    }
    else if(frameCounter == 1800)
    {
        Enemy* newEnemy4 = new Enemy(2, 200, -100);
        newEnemy4->setDestination(200, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 376, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    
    else if(frameCounter == 2000)
    {
        Enemy* newEnemy8 = new Enemy(4, 50, -100);
        newEnemy8->setDestination(50, 100);
        newEnemy8->setFireDelay(240);
        enemyList.push_front(*newEnemy8);
        
        Enemy* newEnemy6 = new Enemy(4, 526, -100);
        newEnemy6->setDestination(526, 200);
        newEnemy6->setFireDelay(240);
        enemyList.push_front(*newEnemy6);
    }

    else if(frameCounter == 2120)
    {
        Enemy* newEnemy2 = new Enemy(4, 576/4, -100);
        newEnemy2->setDestination(576/4, 250);
        newEnemy2->setFireDelay(120);
        enemyList.push_front(*newEnemy2);

        Enemy* newEnemy6 = new Enemy(4, 576*3/4, -100);
        newEnemy6->setDestination(576*3/4, 250);
        newEnemy6->setFireDelay(120);
        enemyList.push_front(*newEnemy6);
    }
    else if(frameCounter == 2250)
    {
        Enemy* newEnemy4 = new Enemy(2, 200, -100);
        newEnemy4->setDestination(200, 300);
        enemyList.push_front(*newEnemy4);

        Enemy* newEnemy5 = new Enemy(2, 376, -100);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 2450)
    {
        Enemy* newEnemy8 = new Enemy(1, 750, 100);
        newEnemy8->setDestination(-500, 400);
        enemyList.push_front(*newEnemy8);
    }
    else if(frameCounter == 2530)
    {
        Enemy* newEnemy5 = new Enemy(2, 576/4, -376);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 2540)
    {
        Enemy* newEnemy5 = new Enemy(2, 576/2, -376);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 2550)
    {
        Enemy* newEnemy5 = new Enemy(2, 576*3/4, -376);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);
    }
    else if(frameCounter == 2700)
    {

        Enemy* newEnemy3 = new Enemy(4, 576/4, -100);
        newEnemy3->setDestination(576/4, 250);
        newEnemy3->setFireDelay(200);
        enemyList.push_front(*newEnemy3);

        Enemy* newEnemy4 = new Enemy(4, 576*3/4, -100);
        newEnemy4->setDestination(576*3/4, 250);
        newEnemy4->setFireDelay(200);
        enemyList.push_front(*newEnemy4);
    }
    else if(frameCounter == 2790)
    {

        Enemy* newEnemy3 = new Enemy(4, 576/6, -100);
        newEnemy3->setDestination(576/6, 150);
        newEnemy3->setFireDelay(110);
        enemyList.push_front(*newEnemy3);

        Enemy* newEnemy4 = new Enemy(4, 576*5/6, -100);
        newEnemy4->setDestination(576*5/6, 150);
        newEnemy4->setFireDelay(110);
        enemyList.push_front(*newEnemy4);
    }
    else if(frameCounter == 3000)
    {
        Enemy* newEnemy2 = new Enemy(3, 576/2, -100);
        newEnemy2->setDestination(576/2, 100);
        newEnemy2->setFireDelay(140);
        enemyList.push_front(*newEnemy2);
    }
    /*else if(frameCounter == 3300)
    {
        Enemy* newEnemy5 = new Enemy(2, 576/2, -376);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);

        Enemy* newEnemy6 = new Enemy(2, 576/4, -376);
        newEnemy6->setDestination(350, 300);
        enemyList.push_front(*newEnemy6);

        Enemy* newEnemy7 = new Enemy(2, 576*3/4, -376);
        newEnemy7->setDestination(350, 300);
        enemyList.push_front(*newEnemy7);
    }
    else if(frameCounter == 3440)
    {
        Enemy* newEnemy5 = new Enemy(2, 576/2, -376);
        newEnemy5->setDestination(350, 300);
        enemyList.push_front(*newEnemy5);

        Enemy* newEnemy6 = new Enemy(2, 576/4, -376);
        newEnemy6->setDestination(350, 300);
        enemyList.push_front(*newEnemy6);

        Enemy* newEnemy7 = new Enemy(2, 576*3/4, -376);
        newEnemy7->setDestination(350, 300);
        enemyList.push_front(*newEnemy7);
    }*/
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
        //gotta handle resize things here
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
            //mainWindow.close();
            //std::vector<sf::VideoMode> FullscreenModes = sf::VideoMode::getFullscreenModes();
            //mainWindow.create(FullscreenModes[0],"SHMUP Super Alpha", sf::Style::Fullscreen);
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
                music[1].pause();
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
                music[1].play();
                gameState = Playing;
            }
            break;
        }
	case Intro:
	{
	    if(currentEvent.type == (sf::Event::KeyPressed) && currentEvent.key.code == (sf::Keyboard::Return))
            {
		storyScreen++;
		sounds[1].play();
		unsigned pos = 0;
		for(int i = 0; i < 10; i++)
		{
			pos = story.find_first_of('\n',pos+1);
		}
		buffer.assign(story,0,pos);
		story.erase(0,pos);

            }
            if(currentEvent.type == sf::Event::Closed || (currentEvent.type == (sf::Event::KeyPressed) && (currentEvent.key.code == sf::Keyboard::Escape)))
            {
                gameState = Exiting;
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
        frameCounter = 0;

        background0.Load("images/acespacebg0.png");
        background1.Load("images/acespacebg1.png");
        background0.sprite.setTextureRect(sf::Rect<int>(0,(2500-768),576,768));
        background1.sprite.setTextureRect(sf::Rect<int>(0,(2500-768),576,768));
        bgMove0 = 0;
        bgMove1 = 0;

        player1.Load("images/Player.png");
        player1.SetPosition(576/2,700);
        player1.revive();

        scoreboard.updatePower(0);
        scoreboard.updateLives(0);
        scoreboard.updateScore(-1);
        scoreboard.updateTargetHP(0,0);

        scoreboard.updateFont(&datagoth);
        scoreboard.clear();
    }
    case ShowingSplash:
    {
        if(!ShowSplashScreen())
            gameState = Exiting;
        else
        {
           gameState = Intro;
		unsigned pos = 0;
		for(int i = 0; i < 10; i++)
		{
			pos = story.find_first_of('\n',pos+1);
		}
		buffer.assign(story,0,pos);
		story.erase(0,pos);
	}
        break;
    }
    case Intro:
    {

	    if(storyScreen == 5) 
	    {
        music[0].stop();
		music[1].play();    
		gameState = Playing;

		scoreboard.updateLives(1);
		scoreboard.updatePower(5);
		scoreboard.updateScore(000000);
        scoreboard.updateTargetHP(0,0);

		frameClock.restart();
	    }

        //update paralaxed backgrounds
        if(bgMove0 > (2500-768))
            bgMove0 = 0;
        bgMove0 += 1;
        background0.sprite.setTextureRect(sf::Rect<int>(0,(2500-768)-bgMove0,576,768));

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

        frameCounter++;
        //spawn new enemies based on framecounter
        Spawn(frameCounter);

        //update paralaxed backgrounds
        if(bgMove0 > (2500-768))
            bgMove0 = 0;
        bgMove0 += 1;
        background0.sprite.setTextureRect(sf::Rect<int>(0,(2500-768)-bgMove0,576,768));
        break;
    }
    }

    if(bgMove1 > (2500-768))
        bgMove1 = 0;
    bgMove1 += 3;
    background1.sprite.setTextureRect(sf::Rect<int>(0,(2500-768)-bgMove1,576,768));

    //clean up sprites to be deleted
    CleanUp();

    //draw game
    mainWindow.clear(sf::Color(0,0,0));
    mainWindow.draw(wholeArea);
    background0.Draw(mainWindow);

    char fps[60];
    sprintf(fps,"%.2f\nESC to Quit\nP to Pause\nSPACE to Fire\nWASD to Move",(float(1)/frameTime));
    sf::String fpsString(fps);
    sf::Text text(fpsString, datagoth);
    text.setCharacterSize(20);
    text.setColor(sf::Color::White);
    text.setPosition(596, 650);

    sf::String sidebar(buffer+"\n\nPress Enter");
    sf::Text textBox(sidebar,datagoth);

    if(gameState == GameOver)
    {
        sf::String gameOver("GAME OVER");
        sf::Text endingMessage(gameOver, datagoth);
        endingMessage.setCharacterSize(50);
        //endingMessage.setStyle(sf::Text::Bold);
        endingMessage.setColor(sf::Color::Red);
        endingMessage.setPosition(288-(endingMessage.getGlobalBounds().width/2), 300);

        sf::String authors("Developed by:\nJohn New\nDevon Gardner\nJon Forcht\nDanny Krulick\n\nMusic By:\nEdward Blakeley\nAlexander Skeppstedt\n\n\nPress Enter to Restart");
        sf::Text credits(authors, datagoth);
        credits.setCharacterSize(20);
        credits.setColor(sf::Color::White);
        credits.setPosition(288-(credits.getGlobalBounds().width/2), 500);
        mainWindow.draw(credits);
        mainWindow.draw(endingMessage);
    }
    if(enemyList.empty() && frameCounter > 3300)
    {
        sf::String gameOver("Good Job!");
        sf::Text endingMessage(gameOver, datagoth);
        endingMessage.setCharacterSize(50);
        //endingMessage.setStyle(sf::Text::Bold);
        endingMessage.setColor(sf::Color::Red);
        endingMessage.setPosition(288-(endingMessage.getGlobalBounds().width/2), 300);

        sf::String authors("The rest of the game\nis under development!\n\nDeveloped by:\nJohn New\nDevon Gardner\nJon Forcht\nDanny Krulick\n\nMusic By:\nEdward Blakeley\nAlexander Skeppstedt\n\nPress Esc to Exit");
        sf::Text credits(authors, datagoth);
        credits.setCharacterSize(20);
        credits.setColor(sf::Color::White);
        credits.setPosition(288-(credits.getGlobalBounds().width/2), 400);
        mainWindow.draw(credits);
        mainWindow.draw(endingMessage);
    }

    DrawProj();
    DrawEnemies();

    if(player1.destroyCheck() == false)
        player1.Draw(mainWindow);

    background1.Draw(mainWindow);

    mainWindow.draw(spawnArea);
    mainWindow.draw(rightBound);
    mainWindow.draw(leftBound);
    mainWindow.draw(bottomBound);
    mainWindow.draw(text);
    if(gameState == Intro && storyScreen < 5)
    {
    	textBox.setCharacterSize(20);
    	textBox.setColor(sf::Color::White);
    	textBox.setPosition(596, 250);
	mainWindow.draw(textBox);
    }
    scoreboard.drawScoreboard(mainWindow, player1.sprite);

    //finally, render the frame
    mainWindow.display();
}

//static object and variable construction
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
sf::RectangleShape Game::rightBound;
sf::RectangleShape Game::spawnArea;
sf::RectangleShape Game::wholeArea;
sf::RectangleShape Game::leftBound;
sf::RectangleShape Game::bottomBound;
sf::View Game::View(sf::FloatRect(0, 0, 1024, 768));
sf::Sound Game::sounds[6];
sf::Music Game::music[2];
sf::Texture Game::textures[3];
std::string Game::story;
std::string Game::buffer;
int Game::storyScreen;
float Game::bgMove0;
float Game::bgMove1;
float Game::bgMove2;
unsigned int Game::frameCounter = 0;
sf::Image Game::icon;
sf::Font Game::uni05;
sf::Font Game::datagoth;

