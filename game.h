#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "circuit.h"
#include "checkpoint.h"
#include "hud.h"
#include "menu.h"
#include "resource_manager.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Clock clock;

    circuit gameCircuit;
    CheckpointManager checkpointManager;
    HUD hud;
    Menu menu;

    bool isGameOver;
    int lastLap;
    int cachedPowerUps;
    int cachedLaps;

    void processEvents();
    void update(float dTime);
    void render();
    void resetGame();

public:
    Game();
    void run();
};