#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "car.h"

class HUD {
private:
    sf::Font font;
    
     
    sf::RectangleShape fuelFrame;
    sf::RectangleShape fuelFill;
    sf::Text fuelLabel;

     
    sf::Text speedText;
    sf::Text damageText;
    sf::Text lapText;

     
    sf::Text logText;
    std::string currentMessage;
    float messageTimer;

     
    sf::RectangleShape gameOverOverlay;
    sf::Text gameOverText;
    sf::Text statsText;
    sf::Text restartText;

     
    bool isMultiplayerMode;
    sf::RectangleShape leaderboardBg;
    sf::Text leaderboardLines[2];  

     
    sf::RectangleShape damageBoardBg;
    sf::Text damageBoardLines[2]; 

public:
    HUD(float screenWidth, float screenHeight);
    
    void setMultiplayerMode(bool enabled);
    void update(const car& playerCar, float dTime, int currentLapP1, int currentLapP2, int cpIndexP1, int cpIndexP2, int damageP2 = 0);
    void draw(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int collectedPowerUps, int finalLaps, const std::string& winnerName = "");
    void showMessage(const std::string& msg);
};
