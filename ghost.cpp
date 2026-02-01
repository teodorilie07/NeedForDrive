#include "ghost.h"
#include "resource_manager.h"
#include <iostream>

GhostManager::GhostManager() 
    : ghostSprite(ResourceManager<sf::Texture>::getInstance().get("assets/car.png")),
      hasBestLap(false) 
{
    // Texture is already set via initializer list
    
    // Setam origin la mijloc, similar cu clasa Car
    sf::FloatRect bounds = ghostSprite.getLocalBounds();
    ghostSprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    
    // Ajustare scara (hardcoded based on car.cpp logic approx)
    if (bounds.size.x > 0 && bounds.size.y > 0) {
        ghostSprite.setScale({ 40.f / bounds.size.x, 70.f / bounds.size.y });
    }
}

void GhostManager::startLap() {
    currentLapData.clear();
    // Pre-allocate memory to avoid frequent reallocations
    currentLapData.reserve(3600); // Approx 60 seconds at 60fps
}

void GhostManager::recordFrame(const car& playerCar, float timeSinceStart) {
    GhostFrame frame;
    frame.position = playerCar.getPozitie();
    frame.angle = playerCar.getUnghi();
    frame.timeStamp = timeSinceStart;
    
    currentLapData.push_back(frame);
}

void GhostManager::finishLap(float) {
    // Daca nu avem un best lap SAU tura curenta e mai rapida (dar aici e infinite,
    // deci in mod ghost vrem sa batem tura precedenta daca a fost valida).
    // In mod simplu: salvam tura curenta ca fiind noul Ghost pentru tura urmatoare.
    
    if (!currentLapData.empty()) {
        // Daca e prima tura sau e mai buna (logica poate fi extinsa), 
        // momentan suprascriem mereu pentru a avea "umbra" turei anterioare.
        bestLapData = currentLapData;
        hasBestLap = true;
        std::cout << "[Ghost] Tura inregistrata: " << bestLapData.size() << " frames.\n";
    }
}

void GhostManager::updateAnimation(float lapTime) {
    if (!hasBestLap || bestLapData.empty()) return;

    // Cautam frame-ul potrivit pentru timpul curent
    size_t frameIndex = 0;
    
    for (size_t i = 0; i < bestLapData.size(); ++i) {
        if (bestLapData[i].timeStamp >= lapTime) {
            frameIndex = i;
            break;
        }
        frameIndex = bestLapData.size() - 1; // Daca am depasit timpul, ramanem la ultimul
    }
    
    const GhostFrame& frame = bestLapData[frameIndex];
    ghostSprite.setPosition({frame.position.getx(), frame.position.gety()});
    ghostSprite.setRotation(sf::degrees(frame.angle));
}

void GhostManager::draw(sf::RenderWindow& window) {
    if (hasBestLap) {
        // Desenam fantoma semi-transparenta (Alpha = 100 din 255)
        ghostSprite.setColor(sf::Color(255, 255, 255, 100));
        window.draw(ghostSprite);
    }
}

void GhostManager::reset() {
    currentLapData.clear();
    bestLapData.clear();
    hasBestLap = false;
}
