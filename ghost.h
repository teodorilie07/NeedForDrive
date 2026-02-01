#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "vector.h"
#include "car.h"

struct GhostFrame {
    vector position;
    float angle;
    float timeStamp;
};

class GhostManager {
private:
    std::vector<GhostFrame> currentLapData;
    std::vector<GhostFrame> bestLapData;
    
    sf::Sprite ghostSprite;
    bool hasBestLap;

public:
    GhostManager();
    
    void startLap();
    void recordFrame(const car& playerCar, float timeSinceStart);
    void finishLap(float finalTime);
    
    void updateAnimation(float lapTime);
    void draw(sf::RenderWindow& window);
    
    void reset();
};
