#pragma once
#include <string>
#include "vector.h"

class car
{
private:
    std::string nume;
    vector pozitie;
    vector viteza;
    double fuel;//nivel de combustibil
    const int consum;//consum de combustibil
    int damage;
    const int damageMax;
    float performanta;
    float unghi;

public:
    car(const std::string& nume, const vector& poz, int fuelInit, int consumMediu);
    car(const car& other);
    car& operator=(const car& other);
    ~car();
    void uptState(float dtime);
    void acceleratie(float factor);
    void roteste(float grade);
    [[nodiscard]] float getUnghi() const;
    void aplicaFrictiune(float factor);
    void brake();
    [[nodiscard]] const std::string& getNume() const;
    [[nodiscard]] const vector& getPozitie() const;
    friend std::ostream& operator<<(std::ostream& os, const car& car);
    void aplicaDamage(int valoare);
    [[nodiscard]] bool eliminata() const;//daca trece de damage max
    void adaugaCombustibil(int cantitate);
    void penalizareMotor(float penalizare);
};
