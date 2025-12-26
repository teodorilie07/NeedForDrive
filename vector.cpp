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

float vector::distance(const vector& other) const
{
    const float dx = this->x - other.x;
    const float dy = this->y - other.y;
    return std::sqrt(dx*dx + dy*dy);
}

std::ostream& operator<<(std::ostream& os, const vector& vec)
{
    os << "(" << vec.x << "," << vec.y << ")";
    return os;
}