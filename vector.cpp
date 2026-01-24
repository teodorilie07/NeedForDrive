#include "vector.h"
#include <cmath>


vector::vector(float val_x, float val_y) : x(val_x), y(val_y) {}

vector::vector(const vector& other) : x(other.x), y(other.y) {}

vector& vector::operator=(const vector& other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
    }
    return *this;
}

float vector::getx() const 
{ 
    return x;
}

 float vector::gety() const
{
    return y;
}

std::ostream& operator<<(std::ostream& os, const vector& vec)
{
    os << "(" << vec.x << "," << vec.y << ")";
    return os;
}