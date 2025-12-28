#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "vector.h"
#include "car.h"

class Decor {
private:
    sf::ConvexShape shape;

public:
    explicit Decor(const std::vector<vector>& puncte);
    
    // Nu desenam nimic, dar verificam interactiunea
    void verificaSiAplica(car& masina) const;
};
