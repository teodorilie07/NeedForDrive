#include "power_up_factory.h"

std::unique_ptr<PowerUp> PowerUpFactory::createPowerUp(PowerUpType type, const vector& poz, sf::Texture& tex, int value) {
    switch (type) {
        case PowerUpType::Boost:
            return std::make_unique<BoostNitro>(poz, tex);
        case PowerUpType::Kit:
            return std::make_unique<KitReparatie>(poz, tex);
        case PowerUpType::Penalizare:
            return std::make_unique<PenalizareMotor>(poz, tex);
        case PowerUpType::Refill:
            return std::make_unique<RefillCombustibil>(poz, tex, value);
        case PowerUpType::MotorFix:
            return std::make_unique<MotorFix>(poz, tex);
        default:
            return nullptr;
    }
}
