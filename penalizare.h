#pragma once
#include "powerUp.h"

class PenalizareMotor : public PowerUp 
{
public:
    explicit PenalizareMotor(const vector& poz, sf::Texture& texturaPowerUp);
    void aplicaEfect(car& masina) override;
    std::unique_ptr<PowerUp> clone() const override;

private:
    void doPrint() const override;
};
