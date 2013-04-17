#ifndef VISIBLEOBJECT_H
#define VISIBLEOBJECT_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <math.h>
#include <iostream>

class VisibleObject
{
public:
    VisibleObject();
    virtual ~VisibleObject();
    virtual void Load(std::string filePath);
    virtual void Draw(sf::RenderWindow & window);
    virtual void SetPosition(float x, float y);
    virtual void loadToggle();
    sf::Sprite sprite;
private:
    bool isLoaded;
    sf::Texture texture;
    std::string filePath;
};

#endif // VISIBLEOBJECT_H
