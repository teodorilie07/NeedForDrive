#pragma once
#include "powerUp.h"

class BoostNitro : public PowerUp 
{
public:
   explicit  BoostNitro(const vector& poz);

    void aplicaEfect(car& masina) override;
};
