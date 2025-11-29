#include "Game.hpp"
#include "Entity/AimEntity.hpp"
#include <iostream>
#include <algorithm>
#include <random>

using namespace sf;

static float randomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

Game::Game()
    : window(sf::VideoMode(900, 550), "Game", sf::Style::Close | sf::Style::Titlebar)
{
    window.setFramerateLimit(60);

    spawnTimer = 0.f;
    nextSpawnDelay = 0.5f; // Delay inicial curto

    // Inicializa com algumas entidades ou deixe vazio para o timer preencher
    for (int i = 0; i < MAX_AIMENTITIES; i++){
        spawnEntity();
    };

    for( int i = 0; i < MAX_SLICEENTITIES; i++){
        sliceEntity();
    };

}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f clickPos((float)event.mouseButton.x, (float)event.mouseButton.y);

            // Remove se clicar
            entities.erase(
                std::remove_if(entities.begin(), entities.end(),
                               [&](const std::unique_ptr<AimEntity> &e)
                               {
                                   return e->isClicked(clickPos);
                               }),
                entities.end());
        }

         
    }
}

void Game::update()
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    // 1. Atualiza Posição
    for (auto &e : entities)
    {
        e->update(dt);
    }

    // 2. Remove entidades mortas (que completaram o trajeto)
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
                       [](const std::unique_ptr<AimEntity> &e)
                       {
                           return e->isDead();
                       }),
        entities.end());

    // 3. SPAWN COM DELAY
    // Se há espaço para mais entidades...
    if (entities.size() < MAX_AIMENTITIES)
    {
        spawnTimer += dt; // Conta o tempo

        // Só cria se o tempo passou do delay estipulado
        if (spawnTimer >= nextSpawnDelay)
        {
            spawnEntity();
            
            spawnTimer = 0.f; // Reseta timer
            // Define o próximo delay aleatório (ex: entre 0.5s e 1.5s)
            nextSpawnDelay = randomFloat(0.5f, 2.f); 
        }
    }

    if (Mouse::isButtonPressed(Mouse::Left))
            {
                
                Vector2i originpixel = Mouse::getPosition(window);
                Vector2f mousepos = window.mapPixelToCoords(originpixel);

                float maxDistance = sliceEntities.empty() ? 20.f : sliceEntities[0]->getRad() * 2.f;

                for(size_t i = 0; i < sliceEntities.size(); ++i)
        {
            SliceEntity* currentSlice = sliceEntities[i].get();

            if (i == 0)
            {
                currentSlice->setPosition(mousepos);
            }
            else
            {
            
                SliceEntity* prevSlice = sliceEntities[i-1].get();

                sf::Vector2f posAnterior = prevSlice->getPosition();
                sf::Vector2f posAtual = currentSlice->getPosition();
                
                float dx = posAtual.x - posAnterior.x;
                float dy = posAtual.y - posAnterior.y;

                float dist = std::sqrt(dx*dx + dy*dy); 

                
                if (dist > maxDistance)
                {
                
                    dx /= dist;
                    dy /= dist;

            
                    float novoX = posAnterior.x + dx * maxDistance;
                    float novoY = posAnterior.y + dy * maxDistance;
                    
                    currentSlice->setPosition(novoX, novoY);
                }
                
            }
        }
            
            }

   
}

void Game::render()
{
    window.clear();
    for (auto &e : entities)
        e->render(window);

    for (auto &s:sliceEntities)
        s->render(window);

    window.display();
}

void Game::spawnEntity()
{
    sf::Vector2u size = window.getSize();
    float x = randomFloat(50.f, size.x - 50.f);
    // Y é tratado no AimEntity, mas passamos size para referência
    
    sf::Vector2f startPos(x, (float)size.y);
    entities.push_back(std::make_unique<AimEntity>(startPos, size));
}



void Game::sliceEntity(){
    sliceEntities.push_back(make_unique<SliceEntity>());
}