#include "kit.h"
#include <iostream>

KitReparatie::KitReparatie(const vector& poz) : PowerUp(poz) {}

void KitReparatie::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat un kit de reparatie\n";
    masina.aplicaDamage(-1);
}
