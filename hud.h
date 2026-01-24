#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "car.h"

class HUD {
private:
    sf::Font font;
    
    // Fuel Bar
    sf::RectangleShape fuelFrame;
    sf::RectangleShape fuelFill;
    sf::Text fuelLabel;

    // Stats
    sf::Text speedText;
    sf::Text damageText;
    sf::Text lapText;

    // Message Log
    sf::Text logText;
    std::string currentMessage;
    float messageTimer;

    // Game Over UI
    sf::RectangleShape gameOverOverlay;
    sf::Text gameOverText;
    sf::Text statsText;
    sf::Text restartText;

public:
    HUD(float screenWidth, float screenHeight);
    
    void update(const car& playerCar, float dTime, int currentLap);
    void draw(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int collectedPowerUps, int finalLaps);
    void showMessage(const std::string& msg);
};
