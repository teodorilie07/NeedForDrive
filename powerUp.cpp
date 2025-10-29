#include "powerUp.h"

PowerUp::PowerUp(const vector& poz) : pozitie(poz) {}

PowerUp::~PowerUp() 
{
     std::cout << "Un power-up a fost eliminat.\n";
}

const vector& PowerUp::getPozitie() const 
{
    return pozitie;
}

std::istream& operator>>(std::istream& is, PowerUp& pu)
{
    float x, y;
    is >> x >> y;
    pu.pozitie = vector(x, y);
    return is;
}
