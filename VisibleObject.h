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
    sf::Sprite sprite;
private:
    sf::Texture texture;
    std::string filePath;
    bool isLoaded;
};

#endif // VISIBLEOBJECT_H
