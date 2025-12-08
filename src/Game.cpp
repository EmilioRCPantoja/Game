#include "Game.hpp"
#include <algorithm>
#include <random>
#include <cmath>

using namespace sf;

static float randomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

Game::Game()
    : window(sf::VideoMode(900, 550), "Game")
{
    window.setFramerateLimit(60);

    spawnTimer = 0.f;
    nextSpawnDelay = 0.5f;
    isMouseMovedPressed = false;

    font.loadFromFile("assets/Montserrat-Regular.ttf");

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10.f, 10.f);

    livesText.setFont(font);
    livesText.setCharacterSize(20);
    livesText.setFillColor(Color::Red);
    livesText.setPosition(10.f, 40.f);

    gameOver = false;

    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setFont(font);

    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.left + bounds.width / 2.f,
                           bounds.top + bounds.height / 2.f);

    gameOverText.setPosition(window.getSize().x / 2.f,
                             window.getSize().y / 2.f);

    for (int i = 0; i < MAX_ENTITIES; i++)
        spawnEntity();

    sliceEntity();
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
        if (gameOver)
        {
            if (event.type == sf::Event::Closed)
                window.close();

            continue;
        }

        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseMoved && Mouse::isButtonPressed(Mouse::Left))
            isMouseMovedPressed = true;
        else
            isMouseMovedPressed = false;

        if (!gameOver && isMouseMovedPressed &&
            clkSliceSp.getElapsedTime().asSeconds() >= 0.005f &&
            sliceEntities.size() <= MAX_SLICEENTITIES)
        {
            sliceEntity();
            clkSliceSp.restart();
        }
    }
}

void Game::update()
{
    static Clock clock;
    float dt = clock.restart().asSeconds();

    if (gameOver)
    {
        scoreText.setString("Score: " + std::to_string(state.getScore()));
        livesText.setString("Lives: " + std::to_string(state.getLives()));
        return;
    }

    if (clkSliceDlt.getElapsedTime().asSeconds() >= 0.005f &&
        !isMouseMovedPressed && sliceEntities.size() > 1)
    {
        sliceEntities.pop_back();
        clkSliceDlt.restart();
    }

    for (auto &s : sliceEntities)
    {
        if (s == sliceEntities.front())
        {
            for (auto &e : entities)
            {
                if (auto f = dynamic_cast<FruitEntity *>(e.get()))
                {
                    if (f->getBounds().contains(s->getPosition()))
                    {
                        f->setDead(s->getPosition());
                        state.addScore(10);
                    }
                }

                if (auto b = dynamic_cast<BombEntity *>(e.get()))
                {
                    if (b->getBounds().contains(s->getPosition()))
                    {
                        b->setDead(s->getPosition());
                        state.loseLife();

                        if (state.isGameOver())
                        {
                            gameOver = true;
                        }
                    }
                }
            }
        }
    }

    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
                       [&](const std::unique_ptr<AimEntity> &e)
                       {
                           if (!e->isDead())
                               return false;

                           if (auto f = dynamic_cast<FruitEntity *>(e.get()))
                           {
                               if (!f->wasSliced())
                               {
                                   state.loseLife();

                                   if (state.isGameOver())
                                       gameOver = true;
                               }
                           }

                           return true;
                       }),
        entities.end());

    for (auto &e : entities)
        e->update(dt);

    if (entities.size() < MAX_ENTITIES)
    {
        spawnTimer += dt;
        if (spawnTimer >= nextSpawnDelay)
        {
            spawnEntity();
            spawnTimer = 0.f;
            nextSpawnDelay = randomFloat(0.5f, 2.f);
        }
    }

    if (isMouseMovedPressed)
    {
        Vector2i originpixel = Mouse::getPosition(window);
        Vector2f mousepos = window.mapPixelToCoords(originpixel);

        float maxDistance = sliceEntities.empty() ? 20.f : sliceEntities[0]->getRad() * 2.f;

        for (size_t i = 0; i < sliceEntities.size(); ++i)
        {
            SliceEntity *currentSlice = sliceEntities[i].get();

            if (i == 0)
                currentSlice->setPosition(mousepos);
            else
            {
                SliceEntity *prevSlice = sliceEntities[i - 1].get();

                Vector2f posAnterior = prevSlice->getPosition();
                Vector2f posAtual = currentSlice->getPosition();

                float dx = posAtual.x - posAnterior.x;
                float dy = posAtual.y - posAnterior.y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist > maxDistance)
                {
                    dx /= dist;
                    dy /= dist;

                    currentSlice->setPosition(
                        posAnterior.x + dx * maxDistance,
                        posAnterior.y + dy * maxDistance);
                }
            }
        }
    }

    scoreText.setString("Score: " + std::to_string(state.getScore()));
    livesText.setString("Lives: " + std::to_string(state.getLives()));
}

void Game::render()
{
    window.clear();

    for (auto &e : entities)
        e->render(window);

    for (auto &s : sliceEntities)
        s->render(window);

    window.draw(scoreText);
    window.draw(livesText);

    if (gameOver)
        window.draw(gameOverText);

    window.display();
}

void Game::spawnEntity()
{
    Vector2u size = window.getSize();
    float x = randomFloat(50.f, size.x - 50.f);
    Vector2f startPos(x, (float)size.y);

    if (randomFloat(0.f, 1.f) < 0.2f)
        entities.push_back(std::make_unique<BombEntity>(startPos, size));
    else
        entities.push_back(std::make_unique<FruitEntity>(startPos, size));
}

void Game::sliceEntity()
{
    sliceEntities.push_back(std::make_unique<SliceEntity>());
}
