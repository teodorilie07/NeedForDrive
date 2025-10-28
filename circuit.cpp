#include "circuit.h"
#include <iostream>
#include <utility>

circuit::circuit(std::string numeCircuit) : nume(std::move(numeCircuit)) {}

void circuit::addCar(const car& car)
{
    car.push_back(car);
}

void circuit::addObst(cost obstacle& obst)
{
    obstacle.push_back(obst);
}

void circuit::checkCol() //verifica coliziunile
{
    for(auto& car : cars)
    {
        for(const auto& obstacle : obstacles)
        {
            if(car.getPozitie().distance(obstacle.getPozitie()) < obstacle.getRaza())
            {
            std::cout << "[COLIZIUNE] "<< car.getNume() << " a lovit un obstacol";
                car.brake();
            }
        }
    }
}

void circuit::simulate(float dTime)
{
    std::cout << "\n=== SIM STEP (Time: " << dTime << " s) in " << nume << "==\n";
    if(cars.empty())
    {
        std::cout << "Nu sunt masini pe circuit.\n";
        return;
    }
    for (auto& car : cars)
    {
        car.accelerateeleratie(vector(1.0, 2.0), 2.5);
        car.uptState(dTime);
    }
    checkCol();
}

std::ostream& operator<<(std::ostream& os, const circuit& circuit)
{
    os << " --- Statusul circuitului: " << circuit.nume << " ---\n";
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

