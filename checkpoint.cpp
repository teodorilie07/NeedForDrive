#include "checkpoint.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>  
#include "erori.h"

CheckpointManager::CheckpointManager()
{
    m_nextCheckpointIndex[0] = 0;
    m_nextCheckpointIndex[1] = 0;
    m_currentLap[0] = 0;
    m_currentLap[1] = 0;
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

void CheckpointManager::update(const car& playerCar, int playerIndex) {
    if (m_checkpoints.empty()) return;
    if (playerIndex < 0 || playerIndex > 1) return;

    if (m_checkpoints[m_nextCheckpointIndex[playerIndex]].shape.getGlobalBounds().findIntersection(playerCar.getGlobalBounds())) {
        bool wasFinishLine = m_checkpoints[m_nextCheckpointIndex[playerIndex]].isFinishLine;
        m_nextCheckpointIndex[playerIndex]++;
        if (m_nextCheckpointIndex[playerIndex] >= static_cast<int>(m_checkpoints.size())) {
            m_nextCheckpointIndex[playerIndex] = 0;
        }
        if (wasFinishLine) {
            m_currentLap[playerIndex]++;
            std::cout << "Jucator " << playerIndex + 1 << ": Tura " << m_currentLap[playerIndex] << " completata!\n";
        }
    }
}

void CheckpointManager::draw(sf::RenderWindow& window, int focusedPlayerIndex) const {
    if (m_checkpoints.empty()) return;

    if (focusedPlayerIndex >= 0 && focusedPlayerIndex < 2) {
        int nextIdx = m_nextCheckpointIndex[focusedPlayerIndex];
        for (size_t i = 0; i < m_checkpoints.size(); ++i) {
            size_t dist = (i - nextIdx + m_checkpoints.size()) % m_checkpoints.size();
            if (dist == 0) { 
                m_checkpoints[i].shape.setFillColor(m_checkpoints[i].isFinishLine ? sf::Color::Red : sf::Color::Yellow);
                window.draw(m_checkpoints[i].shape);
            } else if (dist <= 2) { 
                 sf::Color col = m_checkpoints[i].isFinishLine ? sf::Color::Red : sf::Color::Yellow;
                 col.a = 80;
                 m_checkpoints[i].shape.setFillColor(col);
                 window.draw(m_checkpoints[i].shape);
            }
        }
    } else {
        for (size_t i = 0; i < m_checkpoints.size(); ++i) {
            sf::Color col = m_checkpoints[i].isFinishLine ? sf::Color::Red : sf::Color(255, 255, 0, 50);
            m_checkpoints[i].shape.setFillColor(col);
            window.draw(m_checkpoints[i].shape);
        }
    }
}

int CheckpointManager::getLaps(int playerIndex) const {
    if (playerIndex < 0 || playerIndex > 1) return 0;
    return m_currentLap[playerIndex];
}

int CheckpointManager::getNextCheckpointIndex(int playerIndex) const {
    if (playerIndex < 0 || playerIndex > 1) return 0;
    return m_nextCheckpointIndex[playerIndex];
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
