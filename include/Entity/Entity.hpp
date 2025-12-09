#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;

    virtual bool isClicked(const sf::Vector2f &mousePos) const = 0;
    virtual sf::FloatRect getBounds() const = 0;

    virtual bool isDead() const { return false; }
};
