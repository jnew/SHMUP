#include "Splashscreen.h"

bool Splashscreen::Show(sf::RenderWindow &window)
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
    window.draw(Game::playArea);
    window.draw(title);
    window.display();

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
        while(window.pollEvent(event))
        {
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)))
           {
             return 1;
           }
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) || event.type == sf::Event::Closed )
           {
             return 0;
           }
        }
        if(((clock()-start)/(double) CLOCKS_PER_SEC) >= 0.5)
        {
            window.clear();
            window.draw(Game::playArea);
            //window.draw(sprite);
            window.draw(title);
            if(spriteCount % 2 == 0)
            {
                window.draw(text);
            }
            window.display();
            spriteCount++;
            blinkCounter = 1;
        }
    }
}
