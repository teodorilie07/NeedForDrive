#include "obstacol.h"
#include <cmath>
#include <array>
#include <limits>

constexpr float PI = 3.14159265f;
constexpr float DEG_TO_RAD = PI / 180.0f;

struct Point { float x, y; };

std::array<Point, 4> getCorners(const vector& pos, float w, float h, float rotationDeg) {
    float angleRad = rotationDeg * DEG_TO_RAD;
    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);

    float dx = w / 2.0f;
    float dy = h / 2.0f;

    return {
        Point{ pos.getx() + (dx * cosA - dy * sinA), pos.gety() + (dx * sinA + dy * cosA) },
        Point{ pos.getx() + (-dx * cosA - dy * sinA), pos.gety() + (-dx * sinA + dy * cosA) },
        Point{ pos.getx() + (-dx * cosA - (-dy) * sinA), pos.gety() + (-dx * sinA + (-dy) * cosA) },
        Point{ pos.getx() + (dx * cosA - (-dy) * sinA), pos.gety() + (dx * sinA + (-dy) * cosA) }
    };
}

void project(const std::array<Point, 4>& corners, Point axis, float& min, float& max) {
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::lowest();

    for (const auto& p : corners) {
        float dot = p.x * axis.x + p.y * axis.y;
        if (dot < min) min = dot;
        if (dot > max) max = dot;
    }
}

bool checkCollisionSAT(const vector& posA, float wA, float hA, float rotA,
                       const vector& posB, float wB, float hB, float rotB,
                       vector& outNormal, float& outDepth)
{
    auto cornersA = getCorners(posA, wA, hA, rotA);
    auto cornersB = getCorners(posB, wB, hB, rotB);

    std::array<Point, 4> axes{};

    axes[0] = { cornersA[1].x - cornersA[0].x, cornersA[1].y - cornersA[0].y };
    axes[1] = { cornersA[1].x - cornersA[2].x, cornersA[1].y - cornersA[2].y };
    axes[2] = { cornersB[1].x - cornersB[0].x, cornersB[1].y - cornersB[0].y };
    axes[3] = { cornersB[1].x - cornersB[2].x, cornersB[1].y - cornersB[2].y };

    float minOverlap = std::numeric_limits<float>::max();
    Point smallestAxis = {0, 0};

    for (int i = 0; i < 4; i++) {
        float len = std::sqrt(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        if (len == 0) continue;
        Point axis = { axes[i].x / len, axes[i].y / len };

        float minA, maxA, minB, maxB;
        project(cornersA, axis, minA, maxA);
        project(cornersB, axis, minB, maxB);

        if (maxA < minB || maxB < minA) {
            return false;
        }

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
        }
    }

    outDepth = minOverlap;

    vector centerA = posA;
    vector centerB = posB;
    float dirX = centerA.getx() - centerB.getx();
    float dirY = centerA.gety() - centerB.gety();

    if (dirX * smallestAxis.x + dirY * smallestAxis.y < 0) {
        smallestAxis.x = -smallestAxis.x;
        smallestAxis.y = -smallestAxis.y;
    }

    outNormal = vector(smallestAxis.x, smallestAxis.y);
    return true;
}

obstacol::obstacol(const vector& pos, float lat, float lung, float rot, sf::Texture& texturaObstacol)
    : pozitie(pos), latime(lat), lungime(lung), rotatie(rot), m_sprite(texturaObstacol)
{
    const auto bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    m_sprite.setPosition(toSfmlVector(pozitie));
    m_sprite.setRotation(sf::degrees(rotatie));

    if (bounds.size.x > 0 && bounds.size.y > 0)
    {
        m_sprite.setScale(sf::Vector2f(latime / bounds.size.x, lungime / bounds.size.y));
    }
}

obstacol::~obstacol() {}

const vector& obstacol::getPozitie() const { return pozitie; }
float obstacol::getLat() const { return latime; }
float obstacol::getLung() const { return lungime; }
float obstacol::getRotatie() const { return rotatie; }

void obstacol::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

bool obstacol::verificaColiziune(car& masina) const {
    vector colNormal(0,0);
    float colDepth = 0;

    bool isColliding = checkCollisionSAT(
        masina.getPozitie(), masina.getLatime(), masina.getLungime(), masina.getUnghi(),
        this->pozitie, this->latime, this->lungime, this->rotatie,
        colNormal, colDepth
    );

    if (isColliding)
    {
        vector currentPos = masina.getPozitie();
        float pushFactor = 1.1f;
        vector pushBack(colNormal.getx() * colDepth * pushFactor, colNormal.gety() * colDepth * pushFactor);

        masina.setPozitie(vector(currentPos.getx() + pushBack.getx(), currentPos.gety() + pushBack.gety()));

        if (!masina.esteImuna())
        {
            masina.onCollision();
            masina.brake();
            masina.activeazaImunitate(2.0f);
        }
    }
    return isColliding;
}

std::ostream& operator<<(std::ostream& os, const obstacol& obs)
{
    os << "Obstacol [Poz: " << obs.pozitie << "]";
    return os;
}