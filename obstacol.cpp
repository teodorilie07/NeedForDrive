#include "obstacol.h"
#include "vector.h"
#include <SFML/Graphics.hpp>

obstacol::obstacol(const vector& pos, float r, sf::Texture& texturaObstacol) :
pozitie(pos), rad(r), m_sprite(texturaObstacol)
{
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(sf::Vector2f(bounds.size.x * 0.5f, bounds.size.y * 0.5f));
    m_sprite.setPosition(toSfmlVector(this->pozitie));
}

    const vector& obstacol::getPozitie() const
    {
        return pozitie;
    }

    float obstacol::getRaza() const
    {
        return rad;
    }

    std::ostream& operator<<(std::ostream& os, const obstacol& obs)
    {
        os << "Obstacol [Pozitie:  " << obs.pozitie << ", Raza: " << obs.rad << "]\n";
        return os;
    }

    obstacol::~obstacol()
    {
        std::cout << "destructor: Obstacolul de la pozitia " << pozitie << " a fost eliminat\n";
    }

void obstacol::draw(sf::RenderWindow& window) const
    {
        window.draw(m_sprite);
    }
