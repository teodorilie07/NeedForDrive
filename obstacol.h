#pragma once
#include "vector.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "car.h"

class obstacol {
private:
    vector pozitie;
    float latime;
    float lungime;
    float rotatie;
    sf::Sprite m_sprite;

public:
    obstacol(const vector& pos, float lat, float lung, float rot, sf::Texture& texturaObstacol);
    virtual ~obstacol();

    const vector& getPozitie() const;

    void draw(sf::RenderWindow& window) const;
    bool verificaColiziune(car& masina) const;

    friend std::ostream& operator<<(std::ostream& os, const obstacol& obs);
};