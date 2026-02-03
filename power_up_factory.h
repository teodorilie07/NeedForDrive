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

enum class PowerUpType {
    Boost,
    Kit,
    Penalizare,
    Refill,
    MotorFix
};

class PowerUpFactory {
public:
    static std::unique_ptr<PowerUp> createPowerUp(PowerUpType type, const vector& poz, sf::Texture& tex, int value = 100);
};
