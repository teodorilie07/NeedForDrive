#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>

class vector
{
private:
    float x, y;

public:

    vector(float xVal = 0, float yVal = 0);

    float getx() const;
    float gety() const;

    float distance(const vector& other) const;

    friend std::ostream& operator<<(std::ostream& os, const vector& vec);
};

inline sf::Vector2f toSfmlVector(const vector& v)
{
    return sf::Vector2f(v.getx(), v.gety());
}