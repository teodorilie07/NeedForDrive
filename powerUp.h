#pragma once
#include "vector.h"
#include "car.h"
#include <memory>

class PowerUp 
{
private:
    vector pozitie;
    sf::Sprite m_sprite;

    virtual void doPrint() const = 0;

public:
    PowerUp(const vector& poz,  sf::Texture& texturaPowerUp);

    virtual ~PowerUp();

    const vector& getPozitie() const;

    friend std::istream& operator>>(std::istream& is, PowerUp& pu);

    virtual void aplicaEfect(car& masina) = 0;
    
     
    virtual std::unique_ptr<PowerUp> clone() const = 0;

    void draw(sf::RenderWindow& window) const;
    void printInfo() const;
};
