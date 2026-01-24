#include "penalizare.h"
#include <iostream>

PenalizareMotor::PenalizareMotor(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void PenalizareMotor::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o capcana de motor!\n";
    masina.modifyPerformance(-0.2f);
}

std::unique_ptr<PowerUp> PenalizareMotor::clone() const 
{
    return std::make_unique<PenalizareMotor>(*this);
}

void PenalizareMotor::doPrint() const
{
    std::cout << "Penalizare Motor (Slow) la pozitia " << getPozitie();
}
