#pragma once
#include <memory>
#include <string>
#include "powerUp.h"
#include "boost.h"
#include "kit.h"
#include "penalizare.h"
#include "refill.h"
#include "motorfix.h"
#include "vector.h"

class PowerUpFactory {
public:
    static std::unique_ptr<PowerUp> createPowerUp(const std::string& type, const vector& poz, sf::Texture& tex, int value = 100);
};
