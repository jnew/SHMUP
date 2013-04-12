#include "Splashscreen.h"

void Splashscreen::Show(sf::RenderWindow &window)
{
    sf::Image image;
    if(image.loadFromFile("images/SplashScreen.png") != true)
    {
        return;
    }

    sf::Image image2;
    if(image2.loadFromFile("images/SplashScreen2.png") != true)
    {
        return;
    }

    sf::Texture text;
    if(text.loadFromImage(image) != true)
    {
        return;
    }

    sf::Texture text2;
    if(text2.loadFromImage(image2) != true)
    {
        return;
    }

    sf::Sprite sprite(text);
    sf::Sprite sprite2(text2);
    window.draw(sprite);
    window.display();

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
           if((event.type == (sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) || event.type == sf::Event::Closed )
           {
             return;
           }
        }
        if(((clock()-start)/(double) CLOCKS_PER_SEC) >= 0.5)
        {
            if(spriteCount % 2 == 0)
                window.draw(sprite2);
            else
                window.draw(sprite);
            window.display();
            spriteCount++;
            blinkCounter = 1;
        }
    }
}
