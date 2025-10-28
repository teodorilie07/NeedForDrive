#pragma once
#include "vector.h"
#include <iostream>

class obstacol
{
private:
    vector pozitie;
    const float rad;

public:
    obstacol(const vector& poz, float r);
    ~obstacol();
    const vector& getPozitie() const;
    float getRaza() const;
    friend std::ostream& operator<<(std::ostream& os, const obstacol& obs);

};
