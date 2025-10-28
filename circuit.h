#pragma once
#include <vector>
#include "car.h"
#include "obstacol.h"

class circuit
{
private:
    std::string nume;
    std::vector<car> cars;
    std::vector<obstacol> obstacole;
    void checkCol();//verifica coliziunea

public:
    explicit circuit(std::string numeCircuit);
    void addCar(const car& masina);
    void addObst(const obstacol& obst);//adauga obstacol
    void simulat(float dTime);
    friend std::ostream& operator<<(std::ostream& os, const circuit& circuit);
};
