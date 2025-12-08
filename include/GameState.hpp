class GameState
{
private:
    int score;
    int lives;
    int highScore;

public:
    GameState();

    void reset();
    void addScore(int value);
    void loseLife();

    int getScore() const;
    int getLives() const;
    int getHighScore() const;

    bool isGameOver() const;

    void loadHighScore();
    void saveHighScore();
};
