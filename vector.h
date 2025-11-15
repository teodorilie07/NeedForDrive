#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>

class vector
{
private:
    float x;
    float y;

public:
    explicit vector(float x=0, float y=0);
    [[nodiscard]] float getx() const;
    [[nodiscard]] float gety() const;
    [[nodiscard]] float distance(const vector& other) const;
    friend std::ostream& operator<<(std::ostream& os, const vector& vec);
    friend sf::Vector2f toSfmlVector(const vector& v);
};

