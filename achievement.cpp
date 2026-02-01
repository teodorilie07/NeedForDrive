#include "achievement.h"
#include "erori.h"
#include <iostream>
#include "resource_manager.h"

AchievementManager::AchievementManager() 
    : notificationTimer(0.0f),
      font(ResourceManager<sf::Font>::getInstance().get("C:/Windows/Fonts/arial.ttf")),
      notifTitle(font, "ACHIEVEMENT UNLOCKED!", 14),
      notifName(font, "", 18)
{
    // Laps
    achievements["lap_2"] = { "Bun Venit", "Parcurge 2 ture.", false, 0.0f };
    achievements["lap_6"] = { "Senior", "Parcurge 6 ture.", false, 0.0f };
    achievements["lap_10"] = { "Campion", "Parcurge 10 ture.", false, 0.0f };
    achievements["lap_50"] = { "Novice", "Parcurge 50 ture.", false, 0.0f };

    // Powerups
    achievements["pwr_10"] = { "Colector", "Colecteaza 10 power-up-uri.", false, 0.0f };
    achievements["pwr_40"] = { "Afacerist", "Colecteaza 40 power-up-uri.", false, 0.0f };
    achievements["pwr_50"] = { "Expert", "Colecteaza 50 power-up-uri.", false, 0.0f };
    achievements["pwr_100"] = { "Stapanul", "Colecteaza 100 power-up-uri.", false, 0.0f };

    // Events
    achievements["no_fuel"] = { "Pe vapori", "Ramas fara combustibil.", false, 0.0f };
    achievements["wrecked"] = { "Dauna Totala", "Retired la prima iesire din cursa.", false, 0.0f };

    notifBox.setSize(sf::Vector2f(300.f, 80.f));
    notifBox.setFillColor(sf::Color(50, 50, 50, 200));
    notifBox.setOutlineColor(sf::Color::White);
    notifBox.setOutlineThickness(2.f);
    notifBox.setPosition({1024.f - 320.f, 20.f}); // Top right

    notifTitle.setFillColor(sf::Color::Yellow);
    notifTitle.setPosition({1024.f - 310.f, 30.f});

    notifName.setFillColor(sf::Color::White);
    notifName.setPosition({1024.f - 310.f, 55.f});
}

void AchievementManager::check(bool isMultiplayer, int laps, int powerUps, double fuel, int damage, int maxDamage) {
    if (isMultiplayer) return;

    // Laps
    if (laps >= 2) unlock("lap_2");
    if (laps >= 6) unlock("lap_6");
    if (laps >= 10) unlock("lap_10");
    if (laps >= 50) unlock("lap_50");

    // Powerups
    if (powerUps >= 10) unlock("pwr_10");
    if (powerUps >= 40) unlock("pwr_40");
    if (powerUps >= 50) unlock("pwr_50");
    if (powerUps >= 100) unlock("pwr_100");

    // Fuel
    if (fuel <= 0.001) unlock("no_fuel"); // close enough to 0

    // Damage (Retired)
    if (damage >= maxDamage) unlock("wrecked");
}

void AchievementManager::unlock(const std::string& key) {
    if (unlockedKeys.find(key) != unlockedKeys.end()) return; // Already unlocked

    unlockedKeys.insert(key);
    achievements[key].unlocked = true;
    
    // Queue notification
    activeNotification = key;
    notificationTimer = 3.0f; // Show for 3 seconds
    
    std::cout << "[ACHIEVEMENT] " << achievements[key].name << ": " << achievements[key].description << "\n";
}

void AchievementManager::update(float dTime) {
    if (notificationTimer > 0.0f) {
        notificationTimer -= dTime;
        if (notificationTimer < 0.0f) notificationTimer = 0.0f;
    }
}

void AchievementManager::draw(sf::RenderWindow& window) {
    if (notificationTimer > 0.0f && !activeNotification.empty()) {
        notifName.setString(achievements[activeNotification].name);
        window.draw(notifBox);
        window.draw(notifTitle);
        window.draw(notifName);
    }
}
