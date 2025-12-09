#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class AimEntity : public Entity
{
protected:
    sf::CircleShape shape;
    sf::Vector2f P0;
    sf::Vector2f P1;
    sf::Vector2f P2;
    float t;
    float speed;
    bool dead;

public:
    AimEntity(sf::Vector2f startPos, sf::Vector2u screenSize);

    void update(float dt) override;
    void draw(sf::RenderWindow &window) const override;
    void render(sf::RenderWindow &window) override;

    sf::FloatRect getBounds() const override;
    bool isDead() const override;
};
