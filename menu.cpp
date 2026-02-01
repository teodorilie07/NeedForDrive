#include "menu.h"
#include "resource_manager.h"

Menu::Menu(float width, float height) 
    : currentState(MenuState::MainMenu), selectedItem(0),
      font(ResourceManager<sf::Font>::getInstance().get("C:/Windows/Fonts/arial.ttf")),
      titleText(font), width(width), height(height)
{
    overlay.setSize({width, height});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));

    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({width / 2.0f, height / 5.0f});

    auto initMenu = [&](std::vector<sf::Text>& items, const std::vector<std::string>& labels) {
        for (size_t i = 0; i < labels.size(); ++i) {
            sf::Text text(font, labels[i], 40);
            text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            text.setPosition({width / 2.0f, height / 2.0f + i * 60.0f});
            items.push_back(text);
        }
    };

    initMenu(mainMenuItems, {"Singleplayer", "Multiplayer", "Exit"});
    initMenu(spModesItems, {"Standard Infinite", "Ghost Mode", "Fuel Efficiency", "Back"});
    initMenu(mpLapsItems, {"2 Laps", "4 Laps", "8 Laps", "10 Laps", "Back"});
    initMenu(pauseMenuItems, {"Resume", "Restart", "Main Menu"});
}

void Menu::updateMenuVisuals(std::vector<sf::Text>& items) {
    for (size_t i = 0; i < items.size(); ++i) {
        items[i].setFillColor(i == static_cast<size_t>(selectedItem) ? sf::Color::Red : sf::Color::White);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    if (currentState == MenuState::Inactive) return;

    window.draw(overlay);
    
    std::string title = "NEED FOR DRIVE";
    if (currentState == MenuState::SPModes) title = "SELECT MODE (Infinite)";
    else if (currentState == MenuState::MPLaps) title = "SELECT LAPS (MP)";
    else if (currentState == MenuState::Pause) title = "PAUSE";

    titleText.setString(title);
    sf::FloatRect bounds = titleText.getLocalBounds();
    titleText.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    titleText.setPosition({width / 2.0f, height / 5.0f});
    window.draw(titleText);

    const std::vector<sf::Text>* currentItems = nullptr;
    if (currentState == MenuState::MainMenu) currentItems = &mainMenuItems;
    else if (currentState == MenuState::SPModes) currentItems = &spModesItems;
    else if (currentState == MenuState::MPLaps) currentItems = &mpLapsItems;
    else if (currentState == MenuState::Pause) currentItems = &pauseMenuItems;

    if (currentItems) {
        for (const auto& item : *currentItems) {
            window.draw(item);
        }
    }
}

void Menu::moveUp() {
    int size = 0;
    if (currentState == MenuState::MainMenu) size = mainMenuItems.size();
    else if (currentState == MenuState::SPModes) size = spModesItems.size();
    else if (currentState == MenuState::MPLaps) size = mpLapsItems.size();
    else if (currentState == MenuState::Pause) size = pauseMenuItems.size();

    if (selectedItem - 1 >= 0) {
        selectedItem--;
    } else {
        selectedItem = size - 1;
    }

    if (currentState == MenuState::MainMenu) updateMenuVisuals(mainMenuItems);
    else if (currentState == MenuState::SPModes) updateMenuVisuals(spModesItems);
    else if (currentState == MenuState::MPLaps) updateMenuVisuals(mpLapsItems);
    else if (currentState == MenuState::Pause) updateMenuVisuals(pauseMenuItems);
}

void Menu::moveDown() {
    int size = 0;
    if (currentState == MenuState::MainMenu) size = mainMenuItems.size();
    else if (currentState == MenuState::SPModes) size = spModesItems.size();
    else if (currentState == MenuState::MPLaps) size = mpLapsItems.size();
    else if (currentState == MenuState::Pause) size = pauseMenuItems.size();

    if (selectedItem + 1 < size) {
        selectedItem++;
    } else {
        selectedItem = 0;
    }

    if (currentState == MenuState::MainMenu) updateMenuVisuals(mainMenuItems);
    else if (currentState == MenuState::SPModes) updateMenuVisuals(spModesItems);
    else if (currentState == MenuState::MPLaps) updateMenuVisuals(mpLapsItems);
    else if (currentState == MenuState::Pause) updateMenuVisuals(pauseMenuItems);
}

int Menu::getPressedItem() const {
    return selectedItem;
}

bool Menu::isActive() const {
    return currentState != MenuState::Inactive;
}

void Menu::setState(MenuState state) {
    currentState = state;
    selectedItem = 0;
     
    if (currentState == MenuState::MainMenu) updateMenuVisuals(mainMenuItems);
    else if (currentState == MenuState::SPModes) updateMenuVisuals(spModesItems);
    else if (currentState == MenuState::MPLaps) updateMenuVisuals(mpLapsItems);
    else if (currentState == MenuState::Pause) updateMenuVisuals(pauseMenuItems);
}

MenuState Menu::getState() const {
    return currentState;
}

GameMode Menu::getSelectedGameMode() const {
    switch (selectedItem) {
        case 0: return GameMode::Standard;
        case 1: return GameMode::Ghost;
        case 2: return GameMode::FuelEfficiency;
        default: return GameMode::Standard;
    }
}

int Menu::getSelectedLapsMP() const {
    switch (selectedItem) {
        case 0: return 2;
        case 1: return 4;
        case 2: return 8;
        case 3: return 10;
        default: return 0;  
    }
}