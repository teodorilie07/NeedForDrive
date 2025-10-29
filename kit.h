#pragma once
#include "powerUp.h"


class KitReparatie : public PowerUp 
{
public:
    KitReparatie(const vector& poz);

    void aplicaEfect(car& masina) override;
};

