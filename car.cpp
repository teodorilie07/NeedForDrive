#include "car.h"
#include <iostream>
#include <cmath>

car::car(const std::string& nume, const vector& poz, int combInit, int consum):
    nume(nume),
    pozitie(poz),
    viteza(0, 0),
    fuel(combInit),
    consum(consum),
    damage(0),
    damageMax(3),
    performanta(1.0),
    unghi(0.0)
{
    std::cout << "Init construct: masina condusa de " << nume << "a intrat pe circuit.\n";
}

car::car(const car& other):
    nume(other.nume),
    pozitie(other.pozitie),
    viteza(other.viteza),
    fuel(other.fuel),
    consum(other.consum),
    damage(other.damage),
    damageMax(other.damageMax),
    performanta(other.performanta),
    unghi(other.unghi)
{
    std::cout << "Cpy construct: o copie a "<< nume << "a fost creata.\n";
}

car& car::operator=(const car& other)
{
    std::cout << "Asignment operator: Masina lui" << this->nume << " a devenit o copie a masinii lui " << other.nume <<"\n";
    if (this !=&other)
    {
        this->nume = other.nume;
        this->pozitie = other.pozitie;
        this->viteza = other.viteza;
        this->fuel = other.fuel;
        this->damage = other.damage;
        this->performanta = other.performanta;
        this->unghi = other.unghi;
    }
    return *this;
}

car::~car()
{
    std::cout << "Destructor masina lui" << nume << " a fost scoasa din cursa.\n";
}

void car::uptState(float dTime)
{
    pozitie = vector(
            pozitie.getx() + viteza.getx() * dTime,
            pozitie.gety() + viteza.gety() * dTime
        );
}

void car::acceleratie(float forta)
{
    if (fuel > 0)
    {
        float unghiRad = unghi * (3.14159f / 180.0f);
        vector directie(std::sin(unghiRad), -std::cos(unghiRad));

        float fortaAplicata = forta * performanta;

        viteza = vector(
            viteza.getx() + directie.getx() * fortaAplicata,
            viteza.gety() + directie.gety() * fortaAplicata
        );

        fuel = fuel - consum * 0.2;
        if (fuel < 0)
            fuel = 0;
    }
    else
    {
        std::cout << "[INFO] " << nume << " nu mai are combustibil si nu mai poate accelera.\n";
        brake();
    }
}

void car::brake()
{
    viteza = vector(0, 0);
    std::cout << "[INFO] " << nume << "a franat brusc!\n";

}

const std::string& car::getNume() const
{
    return nume;
}

const vector& car::getPozitie() const
{
    return pozitie;
}

std::ostream& operator<<(std::ostream& os, const car& car)
{
    os << "nume: " << car.nume << " | pozitia " << car.pozitie << "| combustibil " << car.fuel<< "% " << "| damage " << car.damage << "/" << car.damageMax;
    return os;

}

void car::aplicaDamage(int valoare)
{
    this->damage += valoare;
    if(this->damage < 0)
    {
        this->damage = 0;
    }
    std::cout <<"[INFO] " << this->nume << "a suferit damage!";
    std::cout << "Nivel actual: " << this->damage << "/" << this->damageMax << "\n";
}

bool car::eliminata() const
{
    return this->damage >= this->damageMax;
}

void car::adaugaCombustibil(int cantitate)
{
    fuel += cantitate;
    if (fuel > 100)
    {
        fuel = 100;
    }
    std::cout << "[INFO] " << nume << " a primit combustibil! Nivel actual: " << fuel << "%\n";
}

void car::penalizareMotor(float penalizare)
{
    performanta -= penalizare;
    if (performanta < 0.2f)
    {
        performanta = 0.2f;
    }
    std::cout << "[DEBUFF] Motorul lui " << nume << " a fost avariat Performanta redusa la " << static_cast<int>(performanta * 100) << "%\n";
}

void car::roteste(float grade)
{
    this->unghi += grade;
}

float car::getUnghi() const
{
    return this->unghi;
}

void car::aplicaFrictiune(float factor)
{
    viteza = vector(
        viteza.getx() * factor,
        viteza.gety() * factor
    );
}
