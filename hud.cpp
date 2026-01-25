#include "hud.h"
#include "resource_manager.h"
#include <cmath>
#include <iomanip>
#include <sstream>

HUD::HUD(float screenWidth, float screenHeight) : 
    font(ResourceManager<sf::Font>::getInstance().get("C:/Windows/Fonts/arial.ttf")),
    fuelLabel(font, "FUEL", 15),
    speedText(font),
    damageText(font),
    lapText(font),
    logText(font),
    messageTimer(0.0f),
    gameOverText(font, "GAME OVER", 50),
    statsText(font),
    restartText(font, "Press R to Restart or Q to Quit", 25),
    isMultiplayerMode(false),
    leaderboardLines{ sf::Text(font), sf::Text(font) },
    damageBoardLines{ sf::Text(font), sf::Text(font) }
{
     
    fuelFrame.setSize({200.f, 20.f});
    fuelFrame.setFillColor(sf::Color::Transparent);
    fuelFrame.setOutlineColor(sf::Color::Black);
    fuelFrame.setOutlineThickness(3.f);
    fuelFrame.setPosition({20.f, 45.f});

    fuelFill.setSize({200.f, 20.f});
    fuelFill.setFillColor(sf::Color::Green);
    fuelFill.setPosition({20.f, 45.f});

    fuelLabel.setFillColor(sf::Color::White);
    fuelLabel.setPosition({20.f, 30.f});

     
    speedText.setCharacterSize(20);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition({20.f, screenHeight - 60.f});
    
     
    lapText.setCharacterSize(20);
    lapText.setFillColor(sf::Color::White);
    lapText.setPosition({20.f, 5.f});

    damageText.setCharacterSize(20);
    damageText.setFillColor(sf::Color::Red);
    damageText.setPosition({20.f, screenHeight - 35.f});

     
    logText.setCharacterSize(24);
    logText.setFillColor(sf::Color::Yellow);
    logText.setPosition({screenWidth - 300.f, 20.f});

     
    gameOverOverlay.setSize({screenWidth, screenHeight});
    gameOverOverlay.setFillColor(sf::Color(0, 0, 0, 200));

    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect goBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin({goBounds.size.x / 2.f, goBounds.size.y / 2.f});
    gameOverText.setPosition({screenWidth / 2.f, screenHeight / 3.f});

    statsText.setCharacterSize(30);
    statsText.setFillColor(sf::Color::White);

    restartText.setFillColor(sf::Color::White);
    sf::FloatRect rBounds = restartText.getLocalBounds();
    restartText.setOrigin({rBounds.size.x / 2.f, rBounds.size.y / 2.f});
    restartText.setPosition({screenWidth / 2.f, screenHeight * 0.75f});

     
    leaderboardBg.setSize({180.f, 60.f});
    leaderboardBg.setFillColor(sf::Color(0, 0, 0, 150));
    leaderboardBg.setOutlineColor(sf::Color::White);
    leaderboardBg.setOutlineThickness(2.f);
    leaderboardBg.setPosition({screenWidth - 200.f, screenHeight - 140.f});  

    leaderboardLines[0].setCharacterSize(18);
    leaderboardLines[1].setCharacterSize(18);

     
    damageBoardBg.setSize({180.f, 60.f});
    damageBoardBg.setFillColor(sf::Color(0, 0, 0, 150));
    damageBoardBg.setOutlineColor(sf::Color::Red);
    damageBoardBg.setOutlineThickness(2.f);
    damageBoardBg.setPosition({screenWidth - 200.f, screenHeight - 70.f});  

    damageBoardLines[0].setCharacterSize(18);
    damageBoardLines[1].setCharacterSize(18);
}

void HUD::setMultiplayerMode(bool enabled) {
    isMultiplayerMode = enabled;
}

void HUD::update(const car& playerCar, float dTime, int currentLapP1, int currentLapP2, int cpIndexP1, int cpIndexP2, int damageP2) {
     
    double maxFuel = playerCar.getMaxFuel();
    double currentFuel = playerCar.getFuel();
    float fuelPercentage = static_cast<float>(currentFuel / maxFuel);
    
     
    if (fuelPercentage < 0.f) fuelPercentage = 0.f;
    if (fuelPercentage > 1.f) fuelPercentage = 1.f;

    fuelFill.setSize({200.f * fuelPercentage, 20.f});
    
     
    if (fuelPercentage < 0.25f) {
        fuelFill.setFillColor(sf::Color::Red);
    } else if (fuelPercentage < 0.5f) {
        fuelFill.setFillColor(sf::Color::Yellow);
    } else {
        fuelFill.setFillColor(sf::Color::Green);
    }

     
    vector vel = playerCar.getViteza();
    float speedPx = std::sqrt(vel.getx() * vel.getx() + vel.gety() * vel.gety());
    float speedKmh = speedPx * 0.36f; 
    std::stringstream ssSpeed;
    ssSpeed << "Speed: " << std::fixed << std::setprecision(0) << speedKmh << " km/h";
    speedText.setString(ssSpeed.str());
    
    int dmgP1 = playerCar.getDamage();
    int maxDmg = playerCar.getDamageMax();  

     
    if (!isMultiplayerMode) {
        std::stringstream ssLap;
        ssLap << "Laps: " << currentLapP1;
        lapText.setString(ssLap.str());

        std::stringstream ssDmg;
        ssDmg << "Damage: " << dmgP1 << " / " << maxDmg;
        damageText.setString(ssDmg.str());
    } else {
         
        bool p1Ahead = false;
        if (currentLapP1 > currentLapP2) p1Ahead = true;
        else if (currentLapP1 < currentLapP2) p1Ahead = false;
        else { 
            if (cpIndexP1 >= cpIndexP2) p1Ahead = true;
            else p1Ahead = false;
        }

        std::stringstream ss1, ss2;
        sf::Color col1 = sf::Color::White;
        sf::Color col2 = sf::Color(100, 100, 255); 

        if (p1Ahead) {
            ss1 << "1. P1 (Lap " << currentLapP1 << ")";
            ss2 << "2. P2 (Lap " << currentLapP2 << ")";
            leaderboardLines[0].setString(ss1.str());
            leaderboardLines[0].setFillColor(col1);
            
            leaderboardLines[1].setString(ss2.str());
            leaderboardLines[1].setFillColor(col2);
        } else {
            ss1 << "1. P2 (Lap " << currentLapP2 << ")";
            ss2 << "2. P1 (Lap " << currentLapP1 << ")";
            leaderboardLines[0].setString(ss1.str());
            leaderboardLines[0].setFillColor(col2);
            
            leaderboardLines[1].setString(ss2.str());
            leaderboardLines[1].setFillColor(col1);
        }
        
        leaderboardLines[0].setPosition({leaderboardBg.getPosition().x + 10.f, leaderboardBg.getPosition().y + 5.f});
        leaderboardLines[1].setPosition({leaderboardBg.getPosition().x + 10.f, leaderboardBg.getPosition().y + 30.f});

         
        std::stringstream dss1, dss2;
        dss1 << "P1 HP: " << (maxDmg - dmgP1);
        dss2 << "P2 HP: " << (maxDmg - damageP2);
        
        damageBoardLines[0].setString(dss1.str());
        damageBoardLines[0].setFillColor(col1);
        damageBoardLines[0].setPosition({damageBoardBg.getPosition().x + 10.f, damageBoardBg.getPosition().y + 5.f});

        damageBoardLines[1].setString(dss2.str());
        damageBoardLines[1].setFillColor(col2);
        damageBoardLines[1].setPosition({damageBoardBg.getPosition().x + 10.f, damageBoardBg.getPosition().y + 30.f});
    }

     
    if (messageTimer > 0.f) {
        messageTimer -= dTime;
         
        std::uint8_t alpha = 255;
        if (messageTimer < 1.0f) {
            alpha = static_cast<std::uint8_t>(255.f * messageTimer);
        }
        logText.setFillColor(sf::Color(255, 255, 0, alpha));
    } else {
        logText.setString("");
    }
}

void HUD::draw(sf::RenderWindow& window) {
     
    window.draw(fuelFill);
    window.draw(fuelFrame);
    window.draw(fuelLabel);
    
    window.draw(speedText);
    
    if (!isMultiplayerMode) {
        window.draw(lapText);
        window.draw(damageText);
    } else {
        window.draw(leaderboardBg);
        window.draw(leaderboardLines[0]);
        window.draw(leaderboardLines[1]);

        window.draw(damageBoardBg);
        window.draw(damageBoardLines[0]);
        window.draw(damageBoardLines[1]);
    }
    
    if (messageTimer > 0.f) {
        window.draw(logText);
    }
}

void HUD::drawGameOver(sf::RenderWindow& window, int collectedPowerUps, int finalLaps, const std::string& winnerName) {
    window.draw(gameOverOverlay);
    window.draw(gameOverText);

    std::stringstream ss;
    if (!winnerName.empty()) {
        ss << "WINNER: " << winnerName << "\n\n";
    }
    
    if (!isMultiplayerMode) {
        ss << "Laps Completed: " << finalLaps << "\nPower-Ups Collected: " << collectedPowerUps;
    }
    statsText.setString(ss.str());
    
    sf::FloatRect bounds = statsText.getLocalBounds();
    statsText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    statsText.setPosition({static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f});

    window.draw(statsText);
    window.draw(restartText);
}

void HUD::showMessage(const std::string& msg) {
    currentMessage = msg;
    logText.setString(currentMessage);
    logText.setFillColor(sf::Color::Yellow);
    messageTimer = 3.0f;  
}
