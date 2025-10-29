#include "penalizare.h"
#include <iostream>

PenalizareMotor::PenalizareMotor(const vector& poz) : PowerUp(poz) {}

void PenalizareMotor::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o capcana de motor!\n";
    masina.penalizareMotor(0.2f);
}
