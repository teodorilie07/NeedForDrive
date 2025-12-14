#include "powerUp.h"
#include <SFML/Graphics.hpp>

PowerUp::PowerUp(const vector& poz, sf::Texture& texturaPowerUp) :
pozitie(poz),
m_sprite(texturaPowerUp)
{
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(sf::Vector2f(bounds.size.x * 0.5f, bounds.size.y * 0.5f));
    m_sprite.setPosition(toSfmlVector(this->pozitie));
}

PowerUp::~PowerUp() 
{
     std::cout << "Un power-up a fost eliminat.\n";
}

const vector& PowerUp::getPozitie() const 
{
    return pozitie;
}

std::istream& operator>>(std::istream& is, PowerUp& pu)
{
    float x, y;
    is >> x >> y;
    pu.pozitie = vector(x, y);
    return is;
}

void PowerUp::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

void PowerUp::printInfo() const
{
    std::cout << "[INFO PowerUp] ";
    doPrint();
    std::cout << "\n";
}
