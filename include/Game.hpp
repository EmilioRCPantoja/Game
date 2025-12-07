#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Entity/AimEntity.hpp"
#include "Entity/SliceEntity.hpp"

using namespace std;
using namespace sf;

#define MAX_AIMENTITIES 5 

#define MAX_SLICEENTITIES 50

class Game
{
private:
    RenderWindow window;
    vector<unique_ptr<AimEntity>> entities;
    vector<unique_ptr<SliceEntity>> sliceEntities;
    Clock clkSliceSp;
    Clock clkSliceDlt;


    // Variáveis para controle de spawn com delay
    float spawnTimer;
    float nextSpawnDelay;
    bool isMouseMoved;

    void processEvents();
    void update();
    void render();
    void spawnEntity();
    void sliceEntity();

public:
    Game();
    void run();
};