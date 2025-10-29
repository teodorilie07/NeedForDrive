#pragma once
#include "powerUp.h"

class PenalizareMotor : public PowerUp 
{
public:
    explicit PenalizareMotor(const vector& poz);
    void aplicaEfect(car& masina) override;
};
