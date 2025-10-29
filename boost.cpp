#include "boost.h"
#include <iostream>

BoostNitro::BoostNitro(const vector& poz) : PowerUp(poz) {}

void BoostNitro::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a activat un boost de nitro\n";

    masina.acceleratie(vector(1.0, 1.0), 15.0f);
}
