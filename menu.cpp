#include "menu.h"
#include "resource_manager.h"

Menu::Menu(float width, float height) : active(false), selectedItem(0), pauseText(font) {
    try {
        font = ResourceManager<sf::Font>::getInstance().get("C:/Windows/Fonts/arial.ttf");
    } catch (...) {
        try {
            font = ResourceManager<sf::Font>::getInstance().get("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        } catch (...) {
            std::cerr << "Nu s-a putut incarca fontul!\n";
        }
    }

    overlay.setSize(sf::Vector2f(width, height));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    pauseText.setString("PAUSE");
    pauseText.setCharacterSize(60);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setStyle(sf::Text::Bold);
    
    sf::FloatRect textRect = pauseText.getLocalBounds();
    pauseText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    pauseText.setPosition(sf::Vector2f(width / 2.0f, height / 4.0f));

    std::vector<std::string> options = {"Resume", "Restart", "Quit"};
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(font);
        text.setString(options[i]);
        text.setCharacterSize(40);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        text.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f + i * 60.0f));
        
        menuItems.push_back(text);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    if (!active) return;

    window.draw(overlay);
    window.draw(pauseText);
    for (const auto& item : menuItems) {
        window.draw(item);
    }
}

void Menu::moveUp() {
    if (!active) return;
    if (selectedItem - 1 >= 0) {
        menuItems[selectedItem].setFillColor(sf::Color::White);
        selectedItem--;
        menuItems[selectedItem].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (!active) return;
    if (selectedItem + 1 < static_cast<int>(menuItems.size())) {
        menuItems[selectedItem].setFillColor(sf::Color::White);
        selectedItem++;
        menuItems[selectedItem].setFillColor(sf::Color::Red);
    }
}

int Menu::getPressedItem() const {
    return selectedItem;
}

bool Menu::isActive() const {
    return active;
}

void Menu::toggle() {
    active = !active;
    if (active) resetSelection();
}

void Menu::resetSelection() {
    selectedItem = 0;
    for (auto& item : menuItems) item.setFillColor(sf::Color::White);
    menuItems[0].setFillColor(sf::Color::Red);
}
