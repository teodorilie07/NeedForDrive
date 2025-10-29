#pragma once
#include "vector.h"
#include "car.h"
#include <iostream>

class PowerUp 
{
private:
    vector pozitie;

public:
    PowerUp(const vector& poz);

    virtual ~PowerUp();

    const vector& getPozitie() const;

    friend std::istream& operator>>(std::istream& is, PowerUp& pu);

    virtual void aplicaEfect(car& masina) = 0;
};
