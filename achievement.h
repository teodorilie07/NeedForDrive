#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <SFML/Graphics.hpp>

class AchievementManager {
private:
    struct Achievement {
        std::string name;
        std::string description;
        bool unlocked;
        float displayTimer; 
    };

    std::map<std::string, Achievement> achievements;
    std::set<std::string> unlockedKeys; 
    
    std::string activeNotification;
    float notificationTimer;
    
    sf::Font font;
    sf::RectangleShape notifBox;
    sf::Text notifTitle;
    sf::Text notifName;

public:
    AchievementManager();
    
    void check(bool isMultiplayer, int laps, int powerUps, double fuel, int damage, int maxDamage);
    void update(float dTime);
    void draw(sf::RenderWindow& window);
    
private:
    void unlock(const std::string& key);
};
