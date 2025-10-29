#include "car.h"
#include <iostream>

car::car(std::string nume, const vector& poz, int combInit, int consum):
    nume(nume), pozitie(poz), viteza(0, 0), fuel(combInit), consum(consum), damage(0), damageMax(3)
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
    damageMax(other.damageMax)
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

void car::acceleratie(const vector& directie, float factor)
{
    if (fuel > 0)
    {
        viteza = vector(
            viteza.getx() + directie.getx() * factor,
            viteza.gety() + directie.gety() * factor
        );
        fuel -=consum;
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

