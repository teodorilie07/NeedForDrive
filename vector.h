#pragma once
#include <iostream>

class vector
{
private:
    float x;
    float y;

public:
    explicit vector(float x=0, float y=0);
    float getx() const;
    float gety() const;
    float distance(const vector& other) const;
    friend std::ostream& operator<<(std::ostream& os, const vector vec);
};
