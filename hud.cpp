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
    restartText(font, "Press R to Restart or Q to Quit", 25)
{
    // Fuel Bar Setup (Top Left)
    fuelFrame.setSize({200.f, 20.f});
    fuelFrame.setFillColor(sf::Color::Transparent);
    fuelFrame.setOutlineColor(sf::Color::Black);
    fuelFrame.setOutlineThickness(3.f);
    fuelFrame.setPosition({20.f, 20.f});

    fuelFill.setSize({200.f, 20.f});
    fuelFill.setFillColor(sf::Color::Green);
    fuelFill.setPosition({20.f, 20.f});

    fuelLabel.setFillColor(sf::Color::White);
    fuelLabel.setPosition({20.f, 5.f});

    // Stats (Bottom Left)
    speedText.setCharacterSize(20);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition({20.f, screenHeight - 60.f});
    
    // Lap Text (Next to Speed)
    lapText.setCharacterSize(20);
    lapText.setFillColor(sf::Color::White);
    lapText.setPosition({250.f, screenHeight - 60.f});

    damageText.setCharacterSize(20);
    damageText.setFillColor(sf::Color::Red);
    damageText.setPosition({20.f, screenHeight - 35.f});

    // Log (Top Right)
    logText.setCharacterSize(24);
    logText.setFillColor(sf::Color::Yellow);
    logText.setPosition({screenWidth - 300.f, 20.f});

    // Game Over UI
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
}

void HUD::update(const car& playerCar, float dTime, int currentLap) {
    // Update Fuel Bar
    double maxFuel = playerCar.getMaxFuel();
    double currentFuel = playerCar.getFuel();
    float fuelPercentage = static_cast<float>(currentFuel / maxFuel);
    
    // Clamp between 0 and 1
    if (fuelPercentage < 0.f) fuelPercentage = 0.f;
    if (fuelPercentage > 1.f) fuelPercentage = 1.f;

    fuelFill.setSize({200.f * fuelPercentage, 20.f});
    
    // Color changes based on fuel level
    if (fuelPercentage < 0.25f) {
        fuelFill.setFillColor(sf::Color::Red);
    } else if (fuelPercentage < 0.5f) {
        fuelFill.setFillColor(sf::Color::Yellow);
    } else {
        fuelFill.setFillColor(sf::Color::Green);
    }

    // Update Speed
    vector vel = playerCar.getViteza();
    float speedPx = std::sqrt(vel.getx() * vel.getx() + vel.gety() * vel.gety());
    float speedKmh = speedPx * 0.36f; // Scaling increased 10x
    std::stringstream ssSpeed;
    ssSpeed << "Speed: " << std::fixed << std::setprecision(0) << speedKmh << " km/h";
    speedText.setString(ssSpeed.str());
    
    // Update Laps
    std::stringstream ssLap;
    ssLap << "Laps: " << currentLap;
    lapText.setString(ssLap.str());

    // Update Damage
    int dmg = playerCar.getDamage();
    int maxDmg = playerCar.getDamageMax();
    std::stringstream ssDmg;
    ssDmg << "Damage: " << dmg << " / " << maxDmg;
    damageText.setString(ssDmg.str());

    // Update Message Log Timer
    if (messageTimer > 0.f) {
        messageTimer -= dTime;
        // Fade out effect
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
    // Draw fill first, then frame
    window.draw(fuelFill);
    window.draw(fuelFrame);
    window.draw(fuelLabel);
    
    window.draw(speedText);
    window.draw(lapText);
    window.draw(damageText);
    
    if (messageTimer > 0.f) {
        window.draw(logText);
    }
}

void HUD::drawGameOver(sf::RenderWindow& window, int collectedPowerUps, int finalLaps) {
    window.draw(gameOverOverlay);
    window.draw(gameOverText);

    std::stringstream ss;
    ss << "Laps Completed: " << finalLaps << "\nPower-Ups Collected: " << collectedPowerUps;
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
    messageTimer = 3.0f; // Show for 3 seconds
}
