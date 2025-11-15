#pragma once
#include "vector.h"
#include <iostream>
#include <SFML/Graphics.hpp>

class obstacol
{
private:
    vector pozitie;
    const float rad;
    sf::Sprite m_sprite;

public:
    obstacol(const vector& poz, float r, sf::Texture& texturaObstacol);
    ~obstacol();
    const vector& getPozitie() const;
    float getRaza() const;
    friend std::ostream& operator<<(std::ostream& os, const obstacol& obs);
    void draw(sf::RenderWindow& window) const;

};
