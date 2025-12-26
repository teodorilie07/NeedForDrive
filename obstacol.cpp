#include "obstacol.h"
#include <iostream>


obstacol::obstacol(const vector& pos, float lat, float lung, float rot, sf::Texture& texturaObstacol)
    : pozitie(pos), latime(lat), lungime(lung), rotatie(rot), m_sprite(texturaObstacol)
{
    const auto bounds = m_sprite.getLocalBounds();

    m_sprite.setOrigin(bounds.position + bounds.size / 2.f);
    m_sprite.setPosition(toSfmlVector(pozitie));

    m_sprite.setRotation(sf::degrees(rotatie));

    if (bounds.size.x > 0 && bounds.size.y > 0)
    {
        m_sprite.setScale({latime / bounds.size.x, lungime / bounds.size.y});
    }
}

obstacol::~obstacol()
{
    std::cout << "Destructor obstacol pozitie: " << pozitie << "\n";
}

const vector& obstacol::getPozitie() const
{
    return pozitie;
}

float obstacol::getLat() const
{
    return latime;
}

float obstacol::getLung() const
{
    return lungime;
}

float obstacol::getRotatie() const
{
    return rotatie;
}

void obstacol::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

std::ostream& operator<<(std::ostream& os, const obstacol& obs)
{
    os << "Obstacol [Poz: " << obs.pozitie
       << ", L: " << obs.latime
       << ", l: " << obs.lungime
       << ", Rot: " << obs.rotatie << "]";
    return os;
}