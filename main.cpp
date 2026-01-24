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
#include "resource_manager.h"
#include "hud.h"

void resetGame(circuit& circ, CheckpointManager& cm) {
    circ = circuit("Circuitul Monza");
    cm = CheckpointManager();
    
    sf::Texture& carTex = ResourceManager<sf::Texture>::getInstance().get("assets/car.png");
    car masinaLogica("Player", vector(385.f, 575.f), 100, 1, carTex);
    masinaLogica.roteste(90.f); // Face right
    circ.addCar(masinaLogica);

    std::cout << "Incarcarea circuitului din fisierul 'tastatura.txt'...\n";
    circ.incarcaFisier("tastatura.txt");
    cm.loadFromFile("tastatura.txt");

    // Obstacole mici hardcodate
    sf::Texture& obsTex = ResourceManager<sf::Texture>::getInstance().get("assets/obs.png");
    float smW = 6.f;
    float smH = 6.f;

    // Coordonatele specifice
    circ.addObst(obstacol(vector(262, 377), smW, smH, 0, obsTex));
    circ.addObst(obstacol(vector(262, 390), smW, smH, 0, obsTex));
    circ.addObst(obstacol(vector(255, 390), smW, smH, 0, obsTex));
    circ.addObst(obstacol(vector(255, 377), smW, smH, 0, obsTex));

    // Extindere spre stanga pe liniile Y=377 si Y=390
    float currentX = 255.f - 15.f;
    while (currentX > 100.f) { // Limita arbitrara la stanga
        circ.addObst(obstacol(vector(currentX, 377), smW, smH, 0, obsTex));
        circ.addObst(obstacol(vector(currentX, 390), smW, smH, 0, obsTex));
        currentX -= 15.f;
    }

    float circuitLength = cm.getCircuitLength();
    double fuelPerLap = circuitLength * 0.35; 
    circ.setRefillAmount(static_cast<int>(fuelPerLap));

    if (car* pCar = circ.getPlayerCar()) {
        pCar->setMaxFuel(fuelPerLap * 3.0); 
        pCar->setFuel(fuelPerLap * 1.5); 
        std::cout << "[INFO] Restart: Capacitate maxima: " << pCar->getMaxFuel() << "\n";
    }
    circ.regeneratePowerUps();
}

int main() {
    try {
        sf::RenderWindow window(sf::VideoMode({1024, 640}), "NFD");
        window.setFramerateLimit(60);

        sf::Texture& backgroundTexture = ResourceManager<sf::Texture>::getInstance().get("assets/track.png");
        sf::Sprite backgroundSprite(backgroundTexture);

        circuit circuitul("Circuitul Monza");
        CheckpointManager checkpointManager;
        resetGame(circuitul, checkpointManager);

        HUD hud(window.getSize().x, window.getSize().y);

        std::cout << "\n--- Configurarea initiala a circuitului ---\n";
        std::cout << circuitul;

        Menu menu(window.getSize().x, window.getSize().y);
        sf::Clock clock;
        int lastLap = 0;
        int cachedPowerUps = 0;
        int cachedLaps = 0;
        bool isGameOver = false;

        while (window.isOpen())
        {
            sf::Time dt = clock.restart();
            float dTime = dt.asSeconds();

            while (const auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) window.close();
                
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (isGameOver) {
                        if (keyPressed->code == sf::Keyboard::Key::R) {
                            std::cout << "Restarting Game...\n";
                            resetGame(circuitul, checkpointManager);
                            lastLap = 0;
                            isGameOver = false;
                            cachedPowerUps = 0;
                            cachedLaps = 0;
                            clock.restart();
                        } else if (keyPressed->code == sf::Keyboard::Key::Q) {
                            window.close();
                        }
                    } else {
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
                                    resetGame(circuitul, checkpointManager);
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
            }

            if (!menu.isActive() && !isGameOver) {
                car* playerCar = circuitul.getPlayerCar();

                if (playerCar == nullptr) {
                    isGameOver = true;
                } else {
                    cachedPowerUps = playerCar->getCollectedPowerUps();
                    cachedLaps = checkpointManager.getLaps();

                    int currentLap = checkpointManager.getLaps();
                    if (currentLap > lastLap) {
                        if (currentLap > 0 && currentLap % 2 == 0) {
                            std::cout << "[GAME LOGIC] Sfarsit de tura para (" << currentLap << "). Regenerare PowerUps...\n";
                            circuitul.regeneratePowerUps();
                            hud.showMessage("PowerUps Regenerate!");
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
                    
                    // HUD Updates
                    hud.update(*playerCar, dTime, cachedLaps);
                    
                    // Pull messages from circuit
                    std::vector<std::string> msgs = circuitul.popMessages();
                    for (const auto& msg : msgs) {
                        hud.showMessage(msg);
                    }
                }
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

            if (!isGameOver) {
                if (car* pCar = circuitul.getPlayerCar()) {
                    pCar->draw(window);
                }
            }

            // Draw HUD
            hud.draw(window);

            if (isGameOver) {
                hud.drawGameOver(window, cachedPowerUps, cachedLaps);
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
