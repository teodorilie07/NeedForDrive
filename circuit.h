#pragma once
#include <vector>
#include <string>
#include <memory>
#include "car.h"
#include "obstacol.h"
#include "powerUp.h"

class circuit
{
private:
    std::string nume;
    std::vector<car> cars;
    std::vector<obstacol> obstacole;
    std::vector<std::unique_ptr<PowerUp>> powerUps; //vector dinamic pt powerUps
    void checkCol();//verifica coliziunea
    void checkPwrUps();

public:
    explicit circuit(std::string numeCircuit);
    ~circuit();
    void addCar(const car& masina);
    void addObst(const obstacol& obst);//adauga obstacol
    void simulat(float dTime);
    friend std::ostream& operator<<(std::ostream& os, const circuit& circuit);
    bool incarcaFisier(const std::string& cale);
};

