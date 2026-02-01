#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

enum class GameMode {
    Standard,
    Ghost,
    FuelEfficiency
};

enum class MenuState {
    MainMenu,
    SPModes, // Changed from SPLaps
    MPLaps,
    Pause,
    Inactive
};

class Menu {
private:
    MenuState currentState;
    int selectedItem;
    sf::Font font;
    sf::Text titleText;
    
     
    std::vector<sf::Text> mainMenuItems;
    std::vector<sf::Text> spModesItems; // Changed from spLapsItems
    std::vector<sf::Text> mpLapsItems;
    std::vector<sf::Text> pauseMenuItems;

    sf::RectangleShape overlay;
    float width, height;

    void updateMenuVisuals(std::vector<sf::Text>& items);

public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getPressedItem() const;
    
    bool isActive() const;
    void setState(MenuState state);
    MenuState getState() const;
    
    GameMode getSelectedGameMode() const; // New function
    int getSelectedLapsMP() const;
};
