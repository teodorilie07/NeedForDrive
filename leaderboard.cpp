#include "leaderboard.h"
#include "resource_manager.h"
#include <iomanip>
#include <sstream>

Leaderboard::Leaderboard() : 
    font(ResourceManager<sf::Font>::getInstance().get("assets/arial.ttf")),
    titleText(font)
{
    titleText.setString("HIGH SCORES (INFINITE)");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    
    load();
}

void Leaderboard::load() {
    entries.clear();
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name;
        int laps;
        while (file >> name >> laps) {
            entries.push_back({name, laps});
        }
        file.close();
    }
    // Sort descending
    std::sort(entries.begin(), entries.end(), std::greater<Entry>());
}

void Leaderboard::save() {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& entry : entries) {
            file << entry.name << " " << entry.laps << "\n";
        }
        file.close();
    }
}

void Leaderboard::addEntry(const std::string& name, int laps) {
    entries.push_back({name, laps});
    std::sort(entries.begin(), entries.end(), std::greater<Entry>());
    // Keep top 10
    if (entries.size() > 10) {
        entries.resize(10);
    }
    save();
}

void Leaderboard::draw(sf::RenderWindow& window, float width, float height) {
    sf::RectangleShape bg({400.f, 500.f});
    bg.setFillColor(sf::Color(0, 0, 0, 220));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(2.f);
    bg.setOrigin({200.f, 250.f});
    bg.setPosition({width / 2.f, height / 2.f});
    
    window.draw(bg);
    
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText.setPosition({width / 2.f, height / 2.f - 220.f});
    window.draw(titleText);
    
    float startY = height / 2.f - 160.f;
    for (size_t i = 0; i < entries.size(); ++i) {
        std::stringstream ss;
        ss << (i + 1) << ". " << std::setw(10) << std::left << entries[i].name << " - " << entries[i].laps << " laps";
        
        sf::Text line(font, ss.str(), 20);
        line.setFillColor(sf::Color::White);
        sf::FloatRect lb = line.getLocalBounds();
        line.setOrigin({lb.size.x / 2.f, lb.size.y / 2.f});
        line.setPosition({width / 2.f, startY + i * 30.f});
        
        window.draw(line);
    }
}
