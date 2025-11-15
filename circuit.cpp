#include "circuit.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "car.h"
#include "obstacol.h"
#include "kit.h"
#include "boost.h"
#include "refill.h"
#include "penalizare.h"

circuit::circuit(std::string numeCircuit) : numeCircuit(std::move(numeCircuit)) {}

circuit::~circuit()
{
    std::cout << "destructor: circuitul " << numeCircuit << " se incheie\n";
}

void circuit::addCar(const car& masina)
{
    cars.push_back(masina);
}

void circuit::addObst(const obstacol& obst)
{
    obstacole.push_back(obst);
}

void circuit::checkPwrUps()
{
    for (auto& car : cars) {
        for (auto it = powerUps.begin(); it != powerUps.end(); )
        {
            if (car.getPozitie().distance((*it)->getPozitie()) < 3.0)
            {
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
    std::cout << "\n=== SIM (timp: " << dTime << " s) in " << numeCircuit << "==\n";
    if(cars.empty())
    {
        std::cout << "Nu sunt masini pe circuit.\n";
        return;
    }
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
    os << "Obstacole:\n";
    if(circuit.obstacole.empty())
    {
    os << "None\n";
    }
    else
    {
        for(const auto& obs : circuit.obstacole)
        {
            os << " " << obs << "\n";
        }
    }
    os << "Masini din circuit:\n";
    if (circuit.cars.empty())
    {
        os << "None\n";
    }
    else
    {
        for (const auto& car : circuit.cars)
            {
                os << " " << car << "\n";
        }
    }
    os << "-----------------------------------\n";
    return os;
}

bool circuit::incarcaFisier(const std::string& cale)
{
    std::ifstream fisier(cale);
    if(!fisier.is_open())
    {
        std::cout << "ERROARE LA DESCHIDERE FISIER \n";
        return false;
    }

    char tipObiect;
    while (fisier >> tipObiect)
    {
        if (tipObiect == 'O')
        {
            float x, y, raza;
            fisier >> x >> y >> raza;
            //addObst(obstacol(vector(x, y), raza));
        }
        /*
        else if (tipObiect == 'C')
        {
            std::string nume;
            float x, y;
            int fuel, consum;
            fisier >> nume >> x >> y >> fuel >> consum;
            addCar(car(nume, vector(x, y), fuel, consum));
        }
        */
        else if (tipObiect == 'P')
        {
            int tipPowerUp;
            float x, y;
            fisier >> tipPowerUp >> x >> y;
            if (tipPowerUp == 1)
            {
                powerUps.push_back(std::make_unique<KitReparatie>(vector(x, y)));
            }
            else if (tipPowerUp == 2)
            {
                powerUps.push_back(std::make_unique<BoostNitro>(vector(x, y)));
            }
            else if (tipPowerUp == 3)
            {
                 powerUps.push_back(std::make_unique<RefillCombustibil>(vector(x, y)));
            }
            else if (tipPowerUp == 4)
            {
                powerUps.push_back(std::make_unique<PenalizareMotor>(vector(x, y)));
            }
        }
    }
    return true;
}

const std::vector<obstacol>& circuit::getObstacole() const
{
    return obstacole;
}

car* circuit::getPlayerCar()
{

    if (cars.empty())
    {
        return nullptr;
    }
    return &cars[0];
}

float clamp(float val, float min, float max) {
    return std::max(min, std::min(val, max));
}

void circuit::gestioneazaColiziuni()
{
    for (car& masina : cars)
    {
        if (masina.eliminata()) continue;

        float carHalfWidth = masina.getLatime() / 2.0f;
        float carHalfHeight = masina.getLungime() / 2.0f;
        float carAngle = masina.getUnghi();
        vector carPos = masina.getPozitie();

        float carAngleRad = carAngle * (3.14159f / 180.0f);
        float cosAngle = std::cos(-carAngleRad);
        float sinAngle = std::sin(-carAngleRad);

        for (const obstacol& obs : obstacole)
        {
            vector obsPos = obs.getPozitie();
            float obsRad = obs.getRaza();

            vector vecToCircle = vector(obsPos.getx() - carPos.getx(), obsPos.gety() - carPos.gety());

            float localCircleX = vecToCircle.getx() * cosAngle - vecToCircle.gety() * sinAngle;
            float localCircleY = vecToCircle.getx() * sinAngle + vecToCircle.gety() * cosAngle;

            float closestX = clamp(localCircleX, -carHalfWidth, carHalfWidth);
            float closestY = clamp(localCircleY, -carHalfHeight, carHalfHeight);

            float dx = localCircleX - closestX;
            float dy = localCircleY - closestY;
            float distanceSquared = (dx * dx) + (dy * dy);

            if (distanceSquared < (obsRad * obsRad))
            {
                masina.onCollision();
                if (masina.eliminata())
                    break;
            }
        }
    }
}