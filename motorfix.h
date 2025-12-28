#pragma once
#include "powerUp.h"

class MotorFix : public PowerUp 
{
public:
    MotorFix(const vector& poz, sf::Texture& texturaPowerUp);
    void aplicaEfect(car& masina) override;
    std::unique_ptr<PowerUp> clone() const override;
    void doPrint() const override;
};