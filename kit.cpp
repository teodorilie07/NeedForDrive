#include "kit.h"
#include <iostream>

KitReparatie::KitReparatie(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void KitReparatie::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat un kit de reparatie\n";
    masina.modifyDamage(-1);
}

std::unique_ptr<PowerUp> KitReparatie::clone() const 
{
    return std::make_unique<KitReparatie>(*this);
}

void KitReparatie::doPrint() const
{
    std::cout << "Kit Reparatie (HP+) la pozitia " << getPozitie();
}
