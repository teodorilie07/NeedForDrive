#pragma once
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

class Leaderboard {
private:
    struct Entry {
        std::string name;
        int laps;
        
        bool operator>(const Entry& other) const {
            return laps > other.laps;
        }
    };
    
    std::vector<Entry> entries;
    const std::string filename = "leaderboard.txt";
    
    sf::Font font;
    sf::Text titleText;
    std::vector<sf::Text> entryTexts;
    
public:
    Leaderboard();
    
    void load();
    void save();
    void addEntry(const std::string& name, int laps);
    
    void draw(sf::RenderWindow& window, float width, float height);
    
    const std::vector<Entry>& getEntries() const { return entries; }
};
