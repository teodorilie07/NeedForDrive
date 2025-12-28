#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Menu {
private:
    bool active;
    int selectedItem;
    sf::Font font;
    std::vector<sf::Text> menuItems;
    sf::Text pauseText;
    sf::RectangleShape overlay;

public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getPressedItem() const;
    bool isActive() const;
    void toggle();
    void resetSelection();
};
