#pragma once
#include "AimEntity.hpp"

class BombEntity : public AimEntity
{
public:
    BombEntity(sf::Vector2f startPos, sf::Vector2u screenSize);
    bool isClicked(const sf::Vector2f &mousePos) const override;
    void setDead(sf::Vector2f boundPos);
};
