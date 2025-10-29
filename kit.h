#pragma once
#include "powerUp.h"


claitReparatie : public PowerUp 
{
pulic:
   explicit  KitReparatie(const vector& poz);

    void aplicaEfect(car& masina) override;
};

