#include "game.h"
#include <iostream>
#include "erori.h"
#include "power_up_factory.h" 

Game::Game() 
    : window(sf::VideoMode({1024, 640}), "NFD"),
      backgroundTexture(ResourceManager<sf::Texture>::getInstance().get("assets/track.png")),
      backgroundSprite(backgroundTexture),
      gameCircuit("Circuitul Monza"),
      hud(1024.f, 640.f),
      menu(1024.f, 640.f),
      isGameOver(false),
      lastLap(0),
      cachedPowerUps(0),
      cachedLaps(0)
{
    window.setFramerateLimit(60);
    resetGame();
}

void Game::resetGame() {
    gameCircuit = circuit("Circuitul Monza");
    checkpointManager = CheckpointManager();
    
     
    sf::Texture& carTex = ResourceManager<sf::Texture>::getInstance().get("assets/car.png");
    car masinaLogica("Player", vector(385.f, 575.f), 100, 1, carTex);
    masinaLogica.roteste(90.f); 
    gameCircuit.addCar(masinaLogica);

    std::cout << "Incarcarea circuitului din fisierul 'assets/circuit_data.txt'...\n";
    gameCircuit.incarcaFisier("assets/circuit_data.txt"); 
    checkpointManager.loadFromFile("assets/circuit_data.txt");

     
    sf::Texture& obsTex = ResourceManager<sf::Texture>::getInstance().get("assets/obs.png");
    float smW = 6.f;
    float smH = 6.f;
    gameCircuit.addObst(obstacol(vector(262, 377), smW, smH, 0, obsTex));
    gameCircuit.addObst(obstacol(vector(262, 390), smW, smH, 0, obsTex));
    gameCircuit.addObst(obstacol(vector(255, 390), smW, smH, 0, obsTex));
    gameCircuit.addObst(obstacol(vector(255, 377), smW, smH, 0, obsTex));

    float currentX = 255.f - 15.f;
    while (currentX > 100.f) { 
        gameCircuit.addObst(obstacol(vector(currentX, 377), smW, smH, 0, obsTex));
        gameCircuit.addObst(obstacol(vector(currentX, 390), smW, smH, 0, obsTex));
        currentX -= 15.f;
    }

     
    currentX = 124.f;
    while (currentX <= 280.f) {
        gameCircuit.addObst(obstacol(vector(currentX, 512), smW, smH, 0, obsTex));
        currentX += 15.f;
    }

     
    float circuitLength = checkpointManager.getCircuitLength();
    double fuelPerLap = circuitLength * 0.35; 
    gameCircuit.setRefillAmount(static_cast<int>(fuelPerLap));

    if (car* pCar = gameCircuit.getPlayerCar()) {
        pCar->setMaxFuel(fuelPerLap * 3.0); 
        pCar->setFuel(fuelPerLap * 1.5); 
        std::cout << "[INFO] Restart: Capacitate maxima: " << pCar->getMaxFuel() << "\n";
    }
    gameCircuit.regeneratePowerUps();
    
    lastLap = 0;
    cachedPowerUps = 0;
    cachedLaps = 0;
    isGameOver = false;
    clock.restart();
}

void Game::run() {
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float dTime = dt.asSeconds();

        processEvents();
        
        if (!isGameOver && !menu.isActive()) {
            update(dTime);
        }
        
        render();
    }
}

void Game::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (isGameOver) {
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    std::cout << "Restarting Game...\n";
                    resetGame();
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
                            resetGame();
                            menu.toggle();
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
}

void Game::update(float dTime) {
    car* playerCar = gameCircuit.getPlayerCar();

    if (playerCar == nullptr) {
        isGameOver = true;
        return;
    }

    cachedPowerUps = playerCar->getCollectedPowerUps();
    cachedLaps = checkpointManager.getLaps();

    int currentLap = checkpointManager.getLaps();
    if (currentLap > lastLap) {
        if (currentLap > 0 && currentLap % 2 == 0) {
            std::cout << "[GAME LOGIC] Sfarsit de tura para (" << currentLap << "). Regenerare PowerUps...\n";
            gameCircuit.regeneratePowerUps();
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

    gameCircuit.simulat(dTime);
    checkpointManager.update(*playerCar);
    
     
    if (playerCar->eliminata()) {
        isGameOver = true;
    }

    hud.update(*playerCar, dTime, cachedLaps);
    
    std::vector<std::string> msgs = gameCircuit.popMessages();
    for (const auto& msg : msgs) {
        hud.showMessage(msg);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);
    checkpointManager.draw(window);

    for (const auto& obs : gameCircuit.getObstacole()) {
        obs.draw(window);
    }

    for (const auto& pwrUp : gameCircuit.getPowerUps()) {
        pwrUp->draw(window);
    }

    if (!isGameOver) {
        if (car* pCar = gameCircuit.getPlayerCar()) {
            pCar->draw(window);
        }
    }

    hud.draw(window);

    if (isGameOver) {
        hud.drawGameOver(window, cachedPowerUps, cachedLaps);
    }

    if (menu.isActive()) {
        menu.draw(window);
    }

    window.display();
}