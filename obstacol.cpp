#include "obstacol.h"

obstacol::obstacol(const vector& pos, float r) : pozitie(pos), rad(r) {}

    const vector& obstacol::getPozitie() const
    {
        return pozitie;
    }

    float obstacol::getRaza() const
    {
        return rad;
    }

    std::ostream& operator<<(std::ostream& os, const obstacol& obs)
    {
        os << "Obstacol [Pozitie:  " << obs.pozitie << ", Raza: " << obs.rad << "]\n";
        return os;
    }

    obstacol::~obstacol()
    {
        std::cout << "destructor: Obstacolul de la pozitia " << pozitie << " a fost eliminat\n";
    }

