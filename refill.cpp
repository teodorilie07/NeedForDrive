#include "refill.h"
#include <iostream>

RefillCombustibil::RefillCombustibil(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void RefillCombustibil::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o canistra de combustibil\n";
    masina.adaugaCombustibil(50);
}

std::unique_ptr<PowerUp> RefillCombustibil::clone() const 
{
    return std::make_unique<RefillCombustibil>(*this);
}

void RefillCombustibil::doPrint() const
{
    std::cout << "Refill Combustibil (Fuel+) la pozitia " << getPozitie();
}