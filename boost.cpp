#include "boost.h"
#include <iostream>

BoostNitro::BoostNitro(const vector& poz, sf::Texture& texturaPowerUp)
    : PowerUp(poz, texturaPowerUp){}

void BoostNitro::aplicaEfect(car& masina)
{
    masina.modifyPerformance(0.4f);
}

std::unique_ptr<PowerUp> BoostNitro::clone() const 
{
    return std::make_unique<BoostNitro>(*this);
}

void BoostNitro::doPrint() const
{
    std::cout << "NITRO BOOST (Perf++) la pozitia " << getPozitie();
}