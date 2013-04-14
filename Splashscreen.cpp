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

    sf::Sprite sprite(texture);
    window.draw(sprite);
    window.display();

    char prompt[16];
    sprintf(prompt,"Press Enter to Begin");
    sf::Text text(prompt);
    text.setCharacterSize(24);
    text.setColor(sf::Color::White);
    text.setPosition(10, 600);

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
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)) || event.type == sf::Event::Closed )
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
            if(spriteCount % 2 == 0)
            {
                window.draw(sprite);
                window.draw(text);
            }
            else
                window.draw(sprite);
            window.display();
            spriteCount++;
            blinkCounter = 1;
        }
    }
}
