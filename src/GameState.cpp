#include "GameState.hpp"
#include <fstream>

GameState::GameState()
{
    loadHighScore();
    reset();
}

void GameState::reset()
{
    score = 0;
    lives = 3;
}

void GameState::addScore(int value)
{
    score += value;

    if (score > highScore)
    {
        highScore = score;
        saveHighScore();
    }
}

void GameState::loseLife()
{
    lives--;
}

int GameState::getScore() const
{
    return score;
}

int GameState::getLives() const
{
    return lives;
}

int GameState::getHighScore() const
{
    return highScore;
}

bool GameState::isGameOver() const
{
    return lives <= 0;
}

void GameState::loadHighScore()
{
    std::ifstream file("highscore.txt");

    if (file.is_open())
        file >> highScore;
    else
        highScore = 0;
}

void GameState::saveHighScore()
{
    std::ofstream file("highscore.txt");
    file << highScore;
}
