#include "refill.h"
#include <iostream>

RefillCombustibil::RefillCombustibil(const vector& poz) : PowerUp(poz) {}

void RefillCombustibil::aplicaEfect(car& masina) 
{
    std::cout << "[POWER-UP] " << masina.getNume() << " a colectat o canistra de combustibil\n";
    masina.adaugaCombustibil(50); // Adaugă 50% combustibil
}
