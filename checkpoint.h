#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "car.h"

class CheckpointManager {
private:
    struct Checkpoint {
        mutable sf::RectangleShape shape;
        bool isFinishLine;
    };

    std::vector<Checkpoint> m_checkpoints;
    int m_nextCheckpointIndex[2];
    int m_currentLap[2];

public:
    CheckpointManager();
    void loadFromFile(const std::string& filename);
    void update(const car& playerCar, int playerIndex);
    void draw(sf::RenderWindow& window, int focusedPlayerIndex = -1) const;
    int getLaps(int playerIndex) const;
    int getNextCheckpointIndex(int playerIndex) const;
    float getCircuitLength() const;
};
