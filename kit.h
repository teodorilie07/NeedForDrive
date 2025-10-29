#pragma once
#include "powerUp.h"


class KitReparatie : public PowerUp 
{
public:
   explicit  KitReparatie(const vector& poz);

    void aplicaEfect(car& masina) override;
};

