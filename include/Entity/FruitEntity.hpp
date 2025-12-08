#pragma once
#include "AimEntity.hpp"

class FruitEntity : public AimEntity
{
private:
    bool sliced;

public:
    FruitEntity(sf::Vector2f startPos, sf::Vector2u screenSize);
    bool isClicked(const sf::Vector2f &mousePos) const override;
    void setDead(sf::Vector2f pos);
    bool wasSliced() const;
};

