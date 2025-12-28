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
#include "menu.h"

void resetGame(circuit& circ, CheckpointManager& cm, sf::Texture& obsTex, sf::Texture& pwrTex, sf::Texture& carTex) {
    circ = circuit("Circuitul Monza");
    cm = CheckpointManager();
    
    car masinaLogica("Player", vector(400.f, 500.f), 100, 1, carTex);
    circ.addCar(masinaLogica);

    std::cout << "Incarcarea circuitului din fisierul 'tastatura.txt'...\n";
    circ.incarcaFisier("tastatura.txt", obsTex, pwrTex);
    cm.loadFromFile("tastatura.txt");

    float circuitLength = cm.getCircuitLength();
    double fuelPerLap = circuitLength * 0.35; 
    circ.setRefillAmount(static_cast<int>(fuelPerLap));

    if (car* pCar = circ.getPlayerCar()) {
        pCar->setMaxFuel(fuelPerLap * 3.0); 
        pCar->setFuel(fuelPerLap * 1.5); 
        std::cout << "[INFO] Restart: Capacitate maxima: " << pCar->getMaxFuel() << "\n";
    }
    circ.regeneratePowerUps(pwrTex);
}

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
        resetGame(circuitul, checkpointManager, obstacolTexture, powerUpTexture, carTexture);

        std::cout << "\n--- Configurarea initiala a circuitului ---\n";
        std::cout << circuitul;

        Menu menu(window.getSize().x, window.getSize().y);
        sf::Clock clock;
        int lastLap = 0;

        while (window.isOpen())
        {
            sf::Time dt = clock.restart();
            float dTime = dt.asSeconds();

            while (const auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) window.close();
                
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        menu.toggle();
                        if (menu.isActive()) {
                            std::cout << "Game Paused\n";
                        } else {
                            std::cout << "Game Resumed\n";
                            clock.restart();
                        }
                    }

                    if (menu.isActive()) {
                        if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up) {
                            menu.moveUp();
                        }
                        if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down) {
                            menu.moveDown();
                        }
                        if (keyPressed->code == sf::Keyboard::Key::Enter) {
                            int selection = menu.getPressedItem();
                            if (selection == 0) { 
                                menu.toggle();
                                clock.restart();
                            } else if (selection == 1) { 
                                std::cout << "Restarting Game...\n";
                                resetGame(circuitul, checkpointManager, obstacolTexture, powerUpTexture, carTexture);
                                lastLap = 0;
                                menu.toggle();
                                clock.restart();
                            } else if (selection == 2) { 
                                window.close();
                            }
                        }
                    } else {
                        
                        if (keyPressed->code == sf::Keyboard::Key::Backspace) window.close();
                    }
                }
            }

            if (!menu.isActive()) {
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
                float rotationSpeed = 150.f;

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
            }

            
            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);
            checkpointManager.draw(window);

            for (const auto& obs : circuitul.getObstacole()) {
                obs.draw(window);
            }

            for (const auto& pwrUp : circuitul.getPowerUps()) {
                pwrUp->draw(window);
            }

            if (car* pCar = circuitul.getPlayerCar()) {
                pCar->draw(window);
            }

            if (menu.isActive()) {
                menu.draw(window);
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
