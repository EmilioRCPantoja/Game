#include "Entity/BombEntity.hpp"
#include <cmath>

BombEntity::BombEntity(sf::Vector2f startPos, sf::Vector2u screenSize)
    : AimEntity(startPos, screenSize)
{
    shape.setFillColor(sf::Color::Blue);
}

bool BombEntity::isClicked(const sf::Vector2f &mousePos) const
{
    float dx = shape.getPosition().x - mousePos.x;
    float dy = shape.getPosition().y - mousePos.y;
    return (dx * dx + dy * dy) <= (shape.getRadius() * shape.getRadius());
}

void BombEntity::setDead(sf::Vector2f boundPos)
{
    if (shape.getGlobalBounds().contains(boundPos))
        dead = true;
}
