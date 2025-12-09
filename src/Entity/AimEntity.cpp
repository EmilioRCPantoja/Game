#include "Entity/AimEntity.hpp"
#include <random>
#include <cmath>

static float randomFloat(float a, float b)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(a, b);
    return dist(gen);
}

AimEntity::AimEntity(sf::Vector2f startPos, sf::Vector2u screenSize)
{
    shape.setRadius(20.f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    dead = false;

    P0 = startPos;
    P0.y = screenSize.y + shape.getRadius();

    float margin = 50.f;
    float endX;

    do {
        endX = randomFloat(margin, screenSize.x - margin);
    } while (std::abs(endX - P0.x) < 200.f);

    P2.x = endX;
    P2.y = screenSize.y + shape.getRadius();

    P1.x = (P0.x + P2.x) / 2.f;

    float peakY = randomFloat(screenSize.y * 0.25f, screenSize.y * 0.5f);
    P1.y = 2.f * peakY - P0.y;

    t = 0.f;
    float duration = randomFloat(3.f, 6.f);
    speed = 1.f / duration;

    shape.setPosition(P0);
}

void AimEntity::update(float dt)
{
    t += dt * speed;

    if (t >= 1.f)
    {
        t = 1.f;
        dead = true;
    }

    float u = 1.f - t;
    float uu = u * u;
    float tt = t * t;

    sf::Vector2f pos = (uu * P0) + (2.f * u * t * P1) + (tt * P2);
    shape.setPosition(pos);
}

void AimEntity::draw(sf::RenderWindow &window) const
{
    window.draw(shape);
}

void AimEntity::render(sf::RenderWindow &window)
{
    draw(window);
}

sf::FloatRect AimEntity::getBounds() const
{
    return shape.getGlobalBounds();
}

bool AimEntity::isDead() const
{
    return dead;
}
