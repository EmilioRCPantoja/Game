#include "Entity/SliceEntity.hpp"

SliceEntity::SliceEntity()
{
    shape.setFillColor(Color::White);
    shape.setRadius(1.f);
    radius = 1.f;
}

void SliceEntity::setPosition(){}

void SliceEntity::setPosition(const Vector2f pos)
{
    shape.setPosition(pos);
}

void SliceEntity::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

Vector2f SliceEntity::getPosition()
{
    return shape.getPosition();
}

void SliceEntity::setRad(float r)
{
    radius = r;
    shape.setRadius(r);
}

float SliceEntity::getRad()
{
    return radius;
}

void SliceEntity::update(float){}

void SliceEntity::draw(RenderWindow &window) const
{
    window.draw(shape);
}

void SliceEntity::render(RenderWindow &window)
{
    draw(window);
}

bool SliceEntity::isClicked(const sf::Vector2f &mousePos) const
{
    float dx = shape.getPosition().x - mousePos.x;
    float dy = shape.getPosition().y - mousePos.y;
    return (dx * dx + dy * dy) <= (shape.getRadius() * shape.getRadius());
}

sf::FloatRect SliceEntity::getBounds() const
{
    return shape.getGlobalBounds();
}

bool SliceEntity::isDead() const
{
    return false;
}
