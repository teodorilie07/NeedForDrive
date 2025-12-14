#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "car.h"
#include "obstacol.h"
#include "powerUp.h"

class circuit
{
private:
    std::string numeCircuit;
    std::vector<car> cars;
    std::vector<obstacol> obstacole;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    static int contorCircuite;

    void checkPwrUps();
    void gestioneazaColiziuni();

public:
    explicit circuit(std::string numeCircuit);
    
    // Copy Constructor
    circuit(const circuit& other);
    // Assignment Operator
    circuit& operator=(circuit other);
    // Swap helper
    friend void swap(circuit& first, circuit& second) noexcept;

    ~circuit();

    static int getContor();

    void addCar(const car& masina);
    void addObst(const obstacol& obst);
    void addPowerUp(std::unique_ptr<PowerUp> pwrUp);

    [[nodiscard]] const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const;
    [[nodiscard]] const std::vector<obstacol>& getObstacole() const;
    car* getPlayerCar();

    void simulat(float dTime);
    friend std::ostream& operator<<(std::ostream& os, const circuit& circuit);

    bool incarcaFisier(const std::string& cale, sf::Texture& texObs, sf::Texture& texPwr);
};