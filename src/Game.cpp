#include "Game.hpp"
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

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
    if(!backTexture.loadFromFile("assets/fruitBackGround.png")){
        cout << "erro ao carregar background" << endl; 
    }

    backSprite.setTexture(backTexture);

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

    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setFont(font);

    FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);
    gameOverText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f - 120.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(22);
    highScoreText.setFillColor(Color::Yellow);
    highScoreText.setString("High Score: 0");

    FloatRect hsBounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin(
        hsBounds.left + hsBounds.width / 2.f,
        hsBounds.top + hsBounds.height / 2.f);
    highScoreText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f - 20.f);

    playButton.setSize({200.f, 60.f});
    playButton.setFillColor(Color::Green);
    playButton.setOrigin(playButton.getSize() / 2.f);
    playButton.setPosition(window.getSize().x / 2.f, 300.f);

    playText.setFont(font);
    playText.setString("JOGAR");
    playText.setCharacterSize(28);
    playText.setFillColor(Color::Black);

    FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(
        playBounds.left + playBounds.width / 2.f,
        playBounds.top + playBounds.height / 2.f);
    playText.setPosition(playButton.getPosition());

    restartButton.setSize({220.f, 60.f});
    restartButton.setFillColor(Color::White);
    restartButton.setOrigin(restartButton.getSize() / 2.f);
    restartButton.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f + 80.f);

    restartText.setFont(font);
    restartText.setString("RESTART");
    restartText.setCharacterSize(24);
    restartText.setFillColor(Color::Black);

    FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setOrigin(
        restartBounds.left + restartBounds.width / 2.f,
        restartBounds.top + restartBounds.height / 2.f);
    restartText.setPosition(restartButton.getPosition());

    screenState = ScreenState::MENU;
}

void Game::resetGame()
{
    entities.clear();
    sliceEntities.clear();

    state.reset();

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
        if (event.type == Event::Closed)
            window.close();

        if (screenState == ScreenState::MENU)
        {
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                if (playButton.getGlobalBounds().contains(mouse))
                {
                    screenState = ScreenState::PLAYING;
                    resetGame();
                }
            }

            continue;
        }

        if (screenState == ScreenState::GAME_OVER)
        {
            if (event.type == Event::MouseButtonPressed)
            {
                Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                if (restartButton.getGlobalBounds().contains(mouse))
                {
                    screenState = ScreenState::PLAYING;
                    resetGame();
                }
            }

            continue;
        }

        if (event.type == Event::MouseMoved && Mouse::isButtonPressed(Mouse::Left))
            isMouseMovedPressed = true;
        else
            isMouseMovedPressed = false;

        if (isMouseMovedPressed &&
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

    if (screenState != ScreenState::PLAYING)
        return;

    if (clkSliceDlt.getElapsedTime().asSeconds() >= 0.005f &&
        !isMouseMovedPressed && sliceEntities.size() > 1)
    {
        sliceEntities.pop_back();
        clkSliceDlt.restart();
    }

    if (!sliceEntities.empty())
    {
        auto &s = sliceEntities.front();

        // PODE TER DADO ERRO NA QUEDA LIVRE!!!!!!!!!!!!!!!!!!!!
        for (auto &e : entities)
        {
            if (auto f = dynamic_cast<FruitEntity *>(e.get()))
            {
                if (f->getBounds().contains(s->getPosition()) && !f->wasSliced())
                {
                    f->setDead(s->getPosition());
                    state.addScore(10);
                }
            }

            if (auto b = dynamic_cast<BombEntity *>(e.get()))
            {
                if (b->getBounds().contains(s->getPosition()) && !b->isExploding())
                {
                    
                    b->setDead(s->getPosition());
                    
                    highScoreText.setString(
                        "High Score: " + std::to_string(state.getHighScore()));

                    FloatRect hsBounds = highScoreText.getLocalBounds();
                    highScoreText.setOrigin(
                        hsBounds.left + hsBounds.width / 2.f,
                        hsBounds.top + hsBounds.height / 2.f);
                    highScoreText.setPosition(
                        window.getSize().x / 2.f,
                        window.getSize().y / 2.f - 20.f);

                    screenState = ScreenState::GAME_OVER;
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
                                   {
                                       highScoreText.setString(
                                           "High Score: " + std::to_string(state.getHighScore()));

                                       FloatRect hsBounds = highScoreText.getLocalBounds();
                                       highScoreText.setOrigin(
                                           hsBounds.left + hsBounds.width / 2.f,
                                           hsBounds.top + hsBounds.height / 2.f);
                                       highScoreText.setPosition(
                                           window.getSize().x / 2.f,
                                           window.getSize().y / 2.f - 20.f);

                                       screenState = ScreenState::GAME_OVER;
                                   }
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
    window.draw((backSprite));

    if (screenState == ScreenState::MENU)
    {
        window.draw(playButton);
        window.draw(playText);
        window.display();
        return;
    }

    if (screenState == ScreenState::GAME_OVER)
    {
        for (auto &e : entities)
            e->render(window);

        for (auto &s : sliceEntities)
            s->render(window);

        window.draw(gameOverText);
        window.draw(restartButton);
        window.draw(restartText);
        window.draw(highScoreText);

        window.display();
        return;
    }

    for (auto &e : entities)
        e->render(window);

    for (auto &s : sliceEntities)
        s->render(window);

    window.draw(scoreText);
    window.draw(livesText);

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
