#pragma once
#include <string>
#include "vector.h"

class car
{
private:
    std::string nume;
    vector pozitie;
    vector viteza;
    int fuel;//nivel de combustibil
    const int consum;//consum de combustibil

public:
    car(std::string nume, const vector& poz, int fuelInit, int consumMediu);
    car(const car& other);
    ~car();
    void uptState(float dtime);
    void acceleratie(const vector& directie, float factor);
    void brake();
    const std::string& getNume() const;
    const vector& getPozitie() const;
    friend std::ostream& operator<<(std::ostream& os, const car& car);
};
