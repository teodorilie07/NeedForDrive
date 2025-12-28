#include "checkpoint.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>  
#include "erori.h"

CheckpointManager::CheckpointManager()
    : m_nextCheckpointIndex(0), m_currentLap(0)
{
}

void CheckpointManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul " << filename << " pentru checkpoint-uri.\n";
        return;
    }

    m_checkpoints.clear();
    char type;
    while (file >> type) {
        if (type == 'C' || type == 'F') {
            float x, y, rot;
            if (file >> x >> y >> rot) {
                Checkpoint cp;
                cp.isFinishLine = (type == 'F');
                
                 
                cp.shape.setSize({120.f, 15.f});
                cp.shape.setOrigin({60.f, 7.5f});
                cp.shape.setPosition({x, y});
                 
                cp.shape.setRotation(sf::degrees(rot));

                m_checkpoints.push_back(cp);
            }
        } else {
             
            std::string dummy;
            std::getline(file, dummy);
        }
    }
    file.close();
    std::cout << "S-au incarcat " << m_checkpoints.size() << " checkpoint-uri.\n";
}

void CheckpointManager::update(const car& playerCar) {
    if (m_checkpoints.empty()) return;

     
    if (m_checkpoints[m_nextCheckpointIndex].shape.getGlobalBounds().findIntersection(playerCar.getGlobalBounds())) {
        std::cout << "Checkpoint " << m_nextCheckpointIndex << " atins!\n";
        
        bool wasFinishLine = m_checkpoints[m_nextCheckpointIndex].isFinishLine;

        m_nextCheckpointIndex++;
        if (m_nextCheckpointIndex >= static_cast<int>(m_checkpoints.size())) {
            m_nextCheckpointIndex = 0;
        }

        if (wasFinishLine) {
            m_currentLap++;
            std::cout << "Tura " << getLaps() << " completata! (Finish Line trecut)\n";
        }
    }
}

void CheckpointManager::draw(sf::RenderWindow& window) const {
    if (m_checkpoints.empty()) return;

    for (size_t i = 0; i < m_checkpoints.size(); ++i) {
        size_t dist = (i - m_nextCheckpointIndex + m_checkpoints.size()) % m_checkpoints.size();

        bool isVisible = false;
         
        std::uint8_t alpha = 0;

        if (dist == 0) {
            alpha = 255;
            isVisible = true;
        } else if (dist <= 3) {
            alpha = 80; 
            isVisible = true;
        }

        if (isVisible) {
            sf::Color col = m_checkpoints[i].isFinishLine ? sf::Color::Red : sf::Color::Yellow;
            col.a = alpha;
            m_checkpoints[i].shape.setFillColor(col);
            window.draw(m_checkpoints[i].shape);
        }
    }
}

int CheckpointManager::getLaps() const {
    return m_currentLap;
}

float CheckpointManager::getCircuitLength() const {
    if (m_checkpoints.size() < 2) return 0.0f;

    float totalDistance = 0.0f;
    for (size_t i = 0; i < m_checkpoints.size(); ++i) {
         
        sf::Vector2f p1 = m_checkpoints[i].shape.getPosition();
         
        sf::Vector2f p2 = m_checkpoints[(i + 1) % m_checkpoints.size()].shape.getPosition();

         
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        totalDistance += std::sqrt(dx * dx + dy * dy);
    }
    return totalDistance;
}