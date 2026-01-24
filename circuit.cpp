#include "circuit.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>
#include <limits>
#include <random>
#include "car.h"
#include "obstacol.h"
#include "powerUp.h"
#include "penalizare.h"
#include "boost.h"
#include "kit.h"
#include "refill.h"
#include "motorfix.h"
#include "erori.h"
#include "decor.h"
#include "resource_manager.h"
#include "power_up_factory.h"

int circuit::contorCircuite = 0;

circuit::circuit(std::string numeCircuit) : numeCircuit(std::move(numeCircuit)) 
{
    contorCircuite++;
}

circuit::circuit(const circuit& other)
    : numeCircuit(other.numeCircuit + "_copy"),
      cars(other.cars),
      obstacole(other.obstacole),
      decoruri(other.decoruri),
      powerUpSpawnPoints(other.powerUpSpawnPoints),
      refillAmount(other.refillAmount)
{
    // Deep copy powerups
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
    swap(first.decoruri, second.decoruri);
    swap(first.powerUps, second.powerUps);
    swap(first.powerUpSpawnPoints, second.powerUpSpawnPoints);
    swap(first.refillAmount, second.refillAmount);
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

void circuit::gestioneazaColiziuni()
{
    for (car& masina : cars)
    {
        if (masina.eliminata()) continue;

        // Obstacole solide
        for (const auto& obs : obstacole)
        {
            if (obs.verificaColiziune(masina)) {
                if (masina.eliminata()) break;
            }
        }

        // Decoruri (incetinire)
        for (const auto& dec : decoruri)
        {
            dec.verificaSiAplica(masina);
        }
    }
}

void circuit::checkPwrUps()
{
    for (auto& car : cars) {
        for (auto it = powerUps.begin(); it != powerUps.end(); )
        {
            // Use template function calculate distance
            if (getDistance(car, **it) < 35.0) 
            {
                if (auto* pen = dynamic_cast<PenalizareMotor*>(it->get())) {
                    logMessage("Atentie! Penalizare motor!");
                    (void)pen;  
                } else {
                    logMessage("PowerUp Colectat!");
                    car.incrementPowerUps();
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
    os << "Decoruri: " << circuit.decoruri.size() << "\n";
    os << "Masini: " << circuit.cars.size() << "\n";
    return os;
}

bool circuit::incarcaFisier(const std::string& cale)
{
    std::ifstream fisier(cale);
    if(!fisier.is_open())
    {
        throw FileLoadException(cale);
    }

    sf::Texture& texObs = ResourceManager<sf::Texture>::getInstance().get("assets/obs.png");
    sf::Texture& texPwr = ResourceManager<sf::Texture>::getInstance().get("assets/PwrUp.png");

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
        else if (tipObiect == 'D')
        {
            int nrPuncte;
            if (fisier >> nrPuncte && nrPuncte > 0) {
                std::vector<vector> puncte;
                for (int i = 0; i < nrPuncte; ++i) {
                    float x, y;
                    if (fisier >> x >> y) {
                        puncte.emplace_back(x, y);
                    }
                }
                decoruri.emplace_back(puncte);
            }
        }
        else if (tipObiect == 'P')
        {
            int tipPowerUp;
            float x, y;
            if(fisier >> tipPowerUp >> x >> y)
            {
                if (tipPowerUp == 0) {
                    powerUpSpawnPoints.emplace_back(x, y);
                } else if (tipPowerUp == 5) {
                    // MotorFix, fix spawn
                    addPowerUp(PowerUpFactory::createPowerUp("motorfix", vector(x, y), texPwr));
                }
            }
        }
    }
    regeneratePowerUps();
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

void circuit::regeneratePowerUps()
{
    powerUps.clear();

    if (powerUpSpawnPoints.empty()) return;

    sf::Texture& texPwr = ResourceManager<sf::Texture>::getInstance().get("assets/PwrUp.png");

    std::vector<int> types;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> distr(1, 4);

    for (size_t i = 0; i < powerUpSpawnPoints.size(); ++i) {
        types.push_back(distr(g));
    }
    
    for (size_t i = 0; i < powerUpSpawnPoints.size(); ++i)
    {
        vector pos = powerUpSpawnPoints[i];
        
        if (std::abs(pos.getx() - 640.f) < 1.0f && std::abs(pos.gety() - 575.f) < 1.0f) {
             addPowerUp(PowerUpFactory::createPowerUp("motorfix", pos, texPwr));
        } else {
            int type = types[i];
            switch (type) {
                case 1: addPowerUp(PowerUpFactory::createPowerUp("kit", pos, texPwr)); break;
                case 2: addPowerUp(PowerUpFactory::createPowerUp("boost", pos, texPwr)); break;
                case 3: addPowerUp(PowerUpFactory::createPowerUp("refill", pos, texPwr, refillAmount)); break;
                case 4: addPowerUp(PowerUpFactory::createPowerUp("penalizare", pos, texPwr)); break;
            }
        }
    }
    std::cout << "[INFO] PowerUps regenerate.\n";
}

void circuit::logMessage(const std::string& msg) {
    messageLog.push_back(msg);
}

std::vector<std::string> circuit::popMessages() {
    std::vector<std::string> temp = std::move(messageLog);
    messageLog.clear();
    return temp;
}