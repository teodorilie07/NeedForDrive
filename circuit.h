#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "car.h"
#include "obstacol.h"
#include "powerUp.h"
#include "decor.h"

class circuit
{
private:
    std::string numeCircuit;
    std::vector<car> cars;
    std::vector<obstacol> obstacole;
    std::vector<Decor> decoruri;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    std::vector<vector> powerUpSpawnPoints;
    std::vector<std::string> messageLog;
    int refillAmount = 100;
    static int contorCircuite;

    void checkPwrUps();
    void gestioneazaColiziuni();

public:
    explicit circuit(std::string numeCircuit);
    
    circuit(const circuit& other);
    circuit& operator=(circuit other);
    friend void swap(circuit& first, circuit& second) noexcept;

    ~circuit();

    static int getContor();

    void addCar(const car& masina);
    void addObst(const obstacol& obst);
    void addPowerUp(std::unique_ptr<PowerUp> pwrUp);
    void regeneratePowerUps();

     
    void logMessage(const std::string& msg);
    std::vector<std::string> popMessages();

    [[nodiscard]] const std::vector<obstacol>& getObstacole() const;
    [[nodiscard]] const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const;
    car* getPlayerCar();  
    car* getCar(int index);

    void simulat(float dTime);
    friend std::ostream& operator<<(std::ostream& os, const circuit& circuit);

    void incarcaFisier(const std::string& cale);
    void setRefillAmount(int val) { refillAmount = val; }
};