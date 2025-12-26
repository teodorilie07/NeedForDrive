#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "car.h"

class CheckpointManager {
private:
    struct Checkpoint {
        sf::RectangleShape shape;
        bool isFinishLine;
    };

    std::vector<Checkpoint> m_checkpoints;
    int m_nextCheckpointIndex;
    int m_currentLap;

public:
    CheckpointManager();
    void loadFromFile(const std::string& filename);
    void update(const car& playerCar);
    void draw(sf::RenderWindow& window);
    int getLaps() const;
    float getCircuitLength() const;
};
