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

enum class MenuState {
    MainMenu,
    SPLaps,
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
    std::vector<sf::Text> spLapsItems;
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
    
     
    int getSelectedLapsSP() const;
    int getSelectedLapsMP() const;
};
