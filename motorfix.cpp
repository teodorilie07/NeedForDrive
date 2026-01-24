#include "motorfix.h"
#include <iostream>

MotorFix::MotorFix(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void MotorFix::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o reparatie de motor! Performanta restaurata.\n";
    masina.modifyPerformance(1.0f); 
}

std::unique_ptr<PowerUp> MotorFix::clone() const 
{
    return std::make_unique<MotorFix>(*this);
}

void MotorFix::doPrint() const
{
    std::cout << "Motor Fix (Perf+) la pozitia " << getPozitie();
}

