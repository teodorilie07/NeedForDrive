#pragma once
#include "vector.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class obstacol
{
private:
    vector pozitie;
    const float latime;
    const float lungime;
    const float rotatie;
    sf::Sprite m_sprite;

public:
    obstacol(const vector& poz, float lat, float lung, float rot, sf::Texture& texturaObstacol);
    ~obstacol();
    const vector& getPozitie() const;
    float getLat() const;
    float getLung() const;
    float getRotatie() const;
    friend std::ostream& operator<<(std::ostream& os, const obstacol& obs);
    void draw(sf::RenderWindow& window) const;

};
