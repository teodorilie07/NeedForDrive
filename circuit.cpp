#include "circuit.h"
#include <iostream>
#include <fstream>
#include "car.h"
#include "obstacol.h"
#include "kit.h"
#include "boost.h"

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
                car.aplicaDamage(1);
            }
        }
    }
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
            it++;
        }
    }
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
            addObst(obstacol(vector(x, y), raza));
        }
        else if (tipObiect == 'C')
        {
            std::string nume;
            float x, y;
            int fuel, consum;
            fisier >> nume >> x >> y >> fuel >> consum;
            addCar(car(nume, vector(x, y), fuel, consum));
        }
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
        }
    }
    return true;
}


