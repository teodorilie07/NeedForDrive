#include "circuit.h"
#include <iostream>
#include <utility>

circuit::circuit(std::string numeCircuit) : nume(std::move(numeCircuit)) {}

circuit::~circuit()
{
    std::cout << "destructor: circuitul " << nume << " se incheie\n";
}

void circuit::addCar(const car& masina)
{
    cars.push_back(masina);
}

void circuit::addObst(const obstacol& obst)
{
    obstacole.push_back(obst);
}

void circuit::checkCol() //verifica coliziunile
{
    for(auto& car : cars)
    {
        for(const auto& obstacol : obstacole)
        {
            if(car.getPozitie().distance(obstacol.getPozitie()) < obstacol.getRaza())
            {
            std::cout << "[COLIZIUNE] "<< car.getNume() << " a lovit un obstacol";
                car.brake();
            }
        }
    }
}

void circuit::simulat(float dTime)
{
    std::cout << "\n=== SIM (timp: " << dTime << " s) in " << nume << "==\n";
    if(cars.empty())
    {
        std::cout << "Nu sunt masini pe circuit.\n";
        return;
    }
    for (auto& car : cars)
    {
        car.acceleratie(vector(1.0, 2.0), 2.5);
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

