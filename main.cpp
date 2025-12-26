#include <SFML/Graphics.hpp>
#include <iostream>
#include "circuit.h"
#include "vector.h"
#include "car.h"
#include "obstacol.h"
#include "boost.h"
#include "kit.h"
#include "penalizare.h"
#include "refill.h"
#include "erori.h"
#include "checkpoint.h"

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({1024, 640}), "NFD");
        window.setFramerateLimit(60);

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("assets/track.png")) {
            throw FileLoadException("assets/track.png");
        }

        sf::Texture carTexture;
        if (!carTexture.loadFromFile("assets/car.png")) {
            throw FileLoadException("assets/car.png");
        }

        sf::Texture obstacolTexture;
        if (!obstacolTexture.loadFromFile("assets/obs.png")) {
            throw FileLoadException("assets/obs.png");
        }

        sf::Texture powerUpTexture;
        if (!powerUpTexture.loadFromFile("assets/PwrUp.png")) {
            throw FileLoadException("assets/PwrUp.png");
        }

        sf::Sprite backgroundSprite(backgroundTexture);

        circuit circuitul("Circuitul Monza");
        CheckpointManager checkpointManager;

        car masinaLogica("Player", vector(400.f, 500.f), 100, 1, carTexture);
        circuitul.addCar(masinaLogica);

        std::cout << "Incarcarea circuitului din fisierul 'tastatura.txt'...\n";

        circuitul.incarcaFisier("tastatura.txt", obstacolTexture, powerUpTexture);
        checkpointManager.loadFromFile("tastatura.txt");

         
        float circuitLength = checkpointManager.getCircuitLength();
         
         
        double estimatedFuelNeeded = (circuitLength * 0.15) * 1.10; 
        
        std::cout << "[INFO] Lungime circuit: " << circuitLength << " px.\n";
        std::cout << "[INFO] Combustibil necesar estimat: " << estimatedFuelNeeded << "\n";
        
         
        if (car* pCar = circuitul.getPlayerCar()) {
            pCar->setFuel(estimatedFuelNeeded);
        }

        obstacol obstacolLogic(vector(350.f, 150.f), 50.f, 50.f, 45.f, obstacolTexture);
        circuitul.addObst(obstacolLogic);

        circuitul.addPowerUp(std::make_unique<KitReparatie>(vector(100.f, 500.f), powerUpTexture));
        circuitul.addPowerUp(std::make_unique<BoostNitro>(vector(200.f, 500.f), powerUpTexture));
        circuitul.addPowerUp(std::make_unique<RefillCombustibil>(vector(300.f, 500.f), powerUpTexture));
        circuitul.addPowerUp(std::make_unique<PenalizareMotor>(vector(50.f, 500.f), powerUpTexture));

        std::cout << "\n--- Configurarea initiala a circuitului ---\n";
        std::cout << circuitul;

        sf::Clock clock;

        while (window.isOpen())
        {
            sf::Time dt = clock.restart();
            float dTime = dt.asSeconds();

            while (const auto event = window.pollEvent())
            {
                if (event->getIf<sf::Event::Closed>()) window.close();
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
                {
                    if (keyPressed->code == sf::Keyboard::Key::Backspace) window.close();
                }
            }

            car* playerCar = circuitul.getPlayerCar();

            if (playerCar == nullptr) {
                throw GameLogicException("Masina jucatorului a disparut!");
            }

            float moveAcceleration = 200.f;  
            float rotationSpeed = 100.f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                playerCar->roteste(-rotationSpeed * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                playerCar->roteste(rotationSpeed * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                playerCar->acceleratie(moveAcceleration * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                playerCar->acceleratie(-moveAcceleration * 0.5f * dTime);
            }

            circuitul.simulat(dTime);
            checkpointManager.update(*playerCar);

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);
            checkpointManager.draw(window);

            for (const auto& obs : circuitul.getObstacole()) {
                obs.draw(window);
            }

            for (const auto& pwrUp : circuitul.getPowerUps()) {
                pwrUp->draw(window);
            }

            if (playerCar != nullptr) {
                playerCar->draw(window);
            }

            window.display();

        }

    }

    catch (const GameException& e)
    {
        std::cerr << "\n[EXCEPTIE JOC] " << e.what() << "\n";
        std::cerr << "Aplicatia se va inchide.\n";
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[EXCEPTIE STANDARD] " << e.what() << "\n";
        return -1;
    }
    catch (...)
    {
        std::cerr << "\n[EXCEPTIE NECUNOSCUTA] Ceva rau s-a intamplat.\n";
        return -1;
    }

    return 0;
}
