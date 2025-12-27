#pragma once
#include "powerUp.h"

class RefillCombustibil : public PowerUp 
{
private:
    int cantitate;
public:
    RefillCombustibil(const vector& poz, sf::Texture& texturaPowerUp, int cantitate = 100);
    void aplicaEfect(car& masina) override;
    std::unique_ptr<PowerUp> clone() const override;
    void doPrint() const override;
};
