#include "power_up_factory.h"

std::unique_ptr<PowerUp> PowerUpFactory::createPowerUp(const std::string& type, const vector& poz, sf::Texture& tex, int value) {
    if (type == "boost") {
        return std::make_unique<BoostNitro>(poz, tex);
    } else if (type == "kit") {
        return std::make_unique<KitReparatie>(poz, tex);
    } else if (type == "penalizare") {
        return std::make_unique<PenalizareMotor>(poz, tex);
    } else if (type == "refill") {
        return std::make_unique<RefillCombustibil>(poz, tex, value);
    } else if (type == "motorfix") {
        return std::make_unique<MotorFix>(poz, tex);
    }
    return nullptr;
}
