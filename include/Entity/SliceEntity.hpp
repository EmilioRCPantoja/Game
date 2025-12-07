#pragma once
#include<SFML/Graphics.hpp>
#include "Entity.hpp"

using namespace sf;

class SliceEntity : public Entity{
    private:
    CircleShape shape;
    float radius;


    public:
    SliceEntity();

    void update(float dt) override;
    void draw(sf::RenderWindow &window) const override;
    void render(sf::RenderWindow &window) override;
    void setPosition();
    void setPosition(const Vector2f pos);
    void setPosition(float x, float y);
    Vector2f getPosition();
    void setRad(float radius);
    float getRad();
    FloatRect getBounds();
    bool isClicked(const sf::Vector2f &mousePos) const override;
    sf::FloatRect getBounds() const override;
    bool isDead() const override;

};