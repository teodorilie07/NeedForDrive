#pragma once
#include "powerUp.h"

class RefillCombustibil : public PowerUp 
{
public:
    explicit RefillCombustibil(const vector& poz);
    void aplicaEfect(car& masina) override;
};
