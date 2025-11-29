#include "Entity/AimEntity.hpp"
#include <cmath>
#include <random>

// Função auxiliar interna
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
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    // Inicializa flag de morte
    dead = false;

    // --- 1. Ponto Inicial (P0) ---
    P0 = startPos;
    P0.y = screenSize.y + shape.getRadius(); // Nasce logo abaixo da tela

    // --- 2. Ponto Final (P2) ---
    float margin = 50.f;
    float minX = margin;
    float maxX = screenSize.x - margin;
    float endX;

    // Garante que o destino X esteja longe da origem X
    do {
        endX = randomFloat(minX, maxX);
    } while (std::abs(endX - P0.x) < 220.f);

    P2.x = endX;
    P2.y = screenSize.y + shape.getRadius(); // Cai na mesma altura que nasceu

    // --- 3. Ponto de Controle (P1 - O Pico) ---
    P1.x = (P0.x + P2.x) / 2.f;

    // Altura aleatória entre 15% (alto) e 60% (médio) da tela
    float maxPeakY = screenSize.y * 0.25f; 
    float minPeakY = screenSize.y * 0.50f;
    float targetPeakY = randomFloat(maxPeakY, minPeakY);

    P1.y = 2.f * targetPeakY - P0.y;

    // --- 4. Velocidade baseada em Duração ---
    t = 0.f;
    float duration = randomFloat(3.f, 7.f); 
    speed = 1.0f / duration;

    // Define a posição inicial imediatamente para evitar que apareça em (0,0)
    // por 1 frame antes do primeiro update.
    shape.setPosition(P0);
}

void AimEntity::update(float dt)
{
    // Incrementa t
    t += dt * speed;

    // Verifica se chegou ao final da curva
    if (t >= 1.0f)
    {
        t = 1.0f;
        dead = true; // Marca para destruição natural
    }

    // Cálculo da Curva de Bézier Quadrática
    float u = 1.f - t;
    float uu = u * u;
    float tt = t * t;

    sf::Vector2f currentPos = (uu * P0) + (2.f * u * t * P1) + (tt * P2);

    shape.setPosition(currentPos);
}

void AimEntity::draw(sf::RenderWindow &window) const
{
    window.draw(shape);
}

void AimEntity::render(sf::RenderWindow &window)
{
    draw(window);
}

bool AimEntity::isClicked(const sf::Vector2f &mousePos) const
{
    float dx = shape.getPosition().x - mousePos.x;
    float dy = shape.getPosition().y - mousePos.y;
    return (dx * dx + dy * dy) <= (shape.getRadius() * shape.getRadius());
}

sf::FloatRect AimEntity::getBounds() const
{
    return shape.getGlobalBounds();
}

bool AimEntity::isDead() const
{
    return dead;
}