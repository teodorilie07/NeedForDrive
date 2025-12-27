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
         
         
         
        double fuelPerLap = circuitLength * 0.35; 
        
        std::cout << "[INFO] Lungime circuit: " << circuitLength << " px.\n";
        std::cout << "[INFO] Combustibil estimat per tur: " << fuelPerLap << "\n";
        
        circuitul.setRefillAmount(static_cast<int>(fuelPerLap));  

        if (car* pCar = circuitul.getPlayerCar()) {
            pCar->setMaxFuel(fuelPerLap * 3.0);  
            pCar->setFuel(fuelPerLap * 1.5);  
        }

         
        circuitul.regeneratePowerUps(powerUpTexture);

        std::cout << "\n--- Configurarea initiala a circuitului ---\n";
        std::cout << circuitul;

        sf::Clock clock;
        int lastLap = 0;

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

             
            int currentLap = checkpointManager.getLaps();
            if (currentLap > lastLap) {
                if (currentLap > 0 && currentLap % 2 == 0) {
                    std::cout << "[GAME LOGIC] Sfarsit de tura para (" << currentLap << "). Regenerare PowerUps...\n";
                    circuitul.regeneratePowerUps(powerUpTexture);
                }
                lastLap = currentLap;
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

            playerCar->draw(window);

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
