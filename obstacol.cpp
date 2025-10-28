#include "obstacol.h"
#include <iostream>

obstacol::obstacol(const vector& pos, float r) : position(pos), raza(r) {}
    const vector& obstacol::getPozitie() const
    {
        return pozitie;
    }
    float obstacol::getRaza() const
    {
        return raza;
    }

    std::ostream& operator<<(std::ostream& os, const obstacol& obs)
    {
        os << "Obstacol [Pozitie: ] " << obs.pozitie << ", Radius: " << obs.raza << "]\n";
        return os;
    }

