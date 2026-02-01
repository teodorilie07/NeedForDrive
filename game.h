#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "circuit.h"
#include "checkpoint.h"
#include "hud.h"
#include "menu.h"
#include "resource_manager.h"
#include "achievement.h"
#include "leaderboard.h"
#include "ghost.h"

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
    AchievementManager achievementManager;
    Leaderboard leaderboard;
    GhostManager ghostManager;

    bool isGameOver;
    bool isMultiplayer;
    GameMode currentMode; // New member

    bool isEnteringName; 
    std::string playerNameInput;
    sf::Text nameInputText;
    sf::Text namePromptText;
    sf::RectangleShape nameInputBox;

    int targetLaps;
    float currentLapTime; // Track lap time for ghost
    
    int lastLap[2];  
    int cachedPowerUps[2];
    int cachedLaps[2];
    std::string winnerName;

    void processEvents();
    void update(float dTime);
    void render();
    void resetGame(bool multiplayer, int laps, GameMode mode = GameMode::Standard);

public:
    Game();
    void run();
};