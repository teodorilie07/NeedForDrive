#pragma once
#include "powerUp.h"

class RefillCombustibil : public PowerUp 
{
public:
    explicit RefillCombustibil(const vector& poz, sf::Texture& texturaPowerUp);
    void aplicaEfect(car& masina) override;
    std::unique_ptr<PowerUp> clone() const override;

private:
    void doPrint() const override;
};
