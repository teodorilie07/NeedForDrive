#include "circuit.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>
#include <limits>
#include "car.h"
#include "obstacol.h"
#include "powerUp.h"
#include "penalizare.h"
#include "boost.h"
#include "kit.h"
#include "refill.h"
#include "erori.h"

constexpr float PI = 3.14159265f;
constexpr float DEG_TO_RAD = PI / 180.0f;

int circuit::contorCircuite = 0;

circuit::circuit(std::string numeCircuit) : numeCircuit(std::move(numeCircuit)) 
{
    contorCircuite++;
}

 
circuit::circuit(const circuit& other)
    : numeCircuit(other.numeCircuit + "_copy"),
      cars(other.cars),
      obstacole(other.obstacole)
{
     
    for (const auto& ptr : other.powerUps) {
        if (ptr) {
            powerUps.push_back(ptr->clone());
        }
    }
    contorCircuite++;
    std::cout << "Copy Constructor Circuit. Total: " << contorCircuite << "\n";
}

 
circuit& circuit::operator=(circuit other)
{
    swap(*this, other);
    return *this;
}

 
void swap(circuit& first, circuit& second) noexcept
{
    using std::swap;
    swap(first.numeCircuit, second.numeCircuit);
    swap(first.cars, second.cars);
    swap(first.obstacole, second.obstacole);
    swap(first.powerUps, second.powerUps);
}

circuit::~circuit()
{
    contorCircuite--;
    std::cout << "destructor: circuitul " << numeCircuit << " se incheie. Ramase: " << getContor() << "\n";
}

int circuit::getContor() {
    return contorCircuite;
}

void circuit::addCar(const car& masina)
{
    cars.push_back(masina);
}

void circuit::addObst(const obstacol& obst)
{
    obstacole.push_back(obst);
}

void circuit::addPowerUp(std::unique_ptr<PowerUp> pwrUp)
{
    powerUps.push_back(std::move(pwrUp));
}

 

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

 

void circuit::gestioneazaColiziuni()
{
    for (car& masina : cars)
    {
        if (masina.eliminata()) continue;

        for (const obstacol& obs : obstacole)
        {
            vector colNormal(0,0);
            float colDepth = 0;

            bool isColliding = checkCollisionSAT(
                masina.getPozitie(), masina.getLatime(), masina.getLungime(), masina.getUnghi(),
                obs.getPozitie(), obs.getLat(), obs.getLung(), obs.getRotatie(),
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

                    std::cout << "[COLLISION] Lovitura! Imunitate 2s.\n";
                }

                if (masina.eliminata())
                    break;
            }
        }
    }
}

void circuit::checkPwrUps()
{
    for (auto& car : cars) {
        for (auto it = powerUps.begin(); it != powerUps.end(); )
        {
            if (car.getPozitie().distance((*it)->getPozitie()) < 35.0) 
            {
                 
                 
                if (auto* pen = dynamic_cast<PenalizareMotor*>(it->get())) {
                    std::cout << "[GAME INFO] Atentie! Se apropie o penalizare de motor!\n";
                    (void)pen;  
                }

                (*it)->printInfo();
                (*it)->aplicaEfect(car);
                it = powerUps.erase(it);
            } else
            {
                ++it;
            }
        }
    }
}

void circuit::simulat(float dTime)
{
    if(cars.empty()) return;

    for (auto& car : cars)
    {
        float factorFrictiune = 0.98f;
        car.aplicaFrictiune(factorFrictiune);
        car.uptState(dTime);
    }

    gestioneazaColiziuni();
    checkPwrUps();

    for (auto it = cars.begin(); it != cars.end();)
    {
        if(it->eliminata())
        {
         std::cout << "[INFO] " << it->getNume() << " s-a retras din cursa din cauza avariilor\n";
         it = cars.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const circuit& circuit)
{
    os << " --- Statusul circuitului: " << circuit.numeCircuit << " ---\n";
    os << "Obstacole: " << circuit.obstacole.size() << "\n";
    os << "Masini: " << circuit.cars.size() << "\n";
    return os;
}

bool circuit::incarcaFisier(const std::string& cale, sf::Texture& texObs, sf::Texture& texPwr)
{
    (void)texPwr;

    std::ifstream fisier(cale);
    if(!fisier.is_open())
    {
        throw FileLoadException(cale);
    }

    std::cout << "Se incarca din fisier...\n";
    char tipObiect;
    while (fisier >> tipObiect)
    {
        if (tipObiect == 'O')
        {
            float x, y, lat, lung, rot;
            if(fisier >> x >> y >> lat >> lung >> rot) {
                 addObst(obstacol(vector(x, y), lat, lung, rot, texObs));
            }
        }
        else if (tipObiect == 'P')
        {
            int tipPowerUp;
            float x, y;
            if(fisier >> tipPowerUp >> x >> y)
            {
                switch (tipPowerUp) {
                    case 1:
                        addPowerUp(std::make_unique<KitReparatie>(vector(x, y), texPwr));
                        break;
                    case 2:
                        addPowerUp(std::make_unique<BoostNitro>(vector(x, y), texPwr));
                        break;
                    case 3:
                        addPowerUp(std::make_unique<RefillCombustibil>(vector(x, y), texPwr));
                        break;
                    case 4:
                        addPowerUp(std::make_unique<PenalizareMotor>(vector(x, y), texPwr));
                        break;
                    default:
                        std::cerr << "Tip PowerUp necunoscut: " << tipPowerUp << "\n";
                        break;
                }
            }
        }
    }
    return true;
}

const std::vector<obstacol>& circuit::getObstacole() const
{
    return obstacole;
}

const std::vector<std::unique_ptr<PowerUp>>& circuit::getPowerUps() const
{
    return powerUps;
}

car* circuit::getPlayerCar()
{
    if (cars.empty()) return nullptr;
    return &cars[0];
}
