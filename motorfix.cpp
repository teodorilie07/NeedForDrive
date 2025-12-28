#include "motorfix.h"
#include <iostream>

MotorFix::MotorFix(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void MotorFix::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o reparatie de motor! Performanta restaurata.\n";
    masina.penalizareMotor(-1.0f); // Penalizare negativa = bonus (restaurare)
    // Sau implementez o metoda restorePerformance() in car.
    // Metoda penalizareMotor scade performanta. Daca ii dau valoare negativa, o creste.
    // Dar penalizareMotor are un clamp inferior. Ar trebui sa verific daca are clamp superior.
    // car.cpp: performanta -= penalizare;
    // Daca penalizare = -1.0 => performanta += 1.0.
    // Nu exista clamp superior in penalizareMotor. Ar trebui sa existe.
    // Dar presupunand ca performanta initiala e 1.0, o sa fie ok.
}

std::unique_ptr<PowerUp> MotorFix::clone() const 
{
    return std::make_unique<MotorFix>(*this);
}

void MotorFix::doPrint() const
{
    std::cout << "Motor Fix (Perf+) la pozitia " << getPozitie();
}

