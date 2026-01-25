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
    bool isMultiplayer;
    int targetLaps;
    int lastLap[2];  
    int cachedPowerUps[2];
    int cachedLaps[2];
    std::string winnerName;

    void processEvents();
    void update(float dTime);
    void render();
    void resetGame(bool multiplayer, int laps);

public:
    Game();
    void run();
};