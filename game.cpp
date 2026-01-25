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
      isMultiplayer(false),
      targetLaps(-1)
{
    window.setFramerateLimit(60);
    lastLap[0] = 0; lastLap[1] = 0;
    cachedPowerUps[0] = 0; cachedPowerUps[1] = 0;
    cachedLaps[0] = 0; cachedLaps[1] = 0;
    
    resetGame(false, -1);
}

void Game::resetGame(bool multiplayer, int laps) {
    isMultiplayer = multiplayer;
    targetLaps = laps;
    winnerName = "";
    
    gameCircuit = circuit("Circuitul Monza");
    checkpointManager = CheckpointManager();
    
    sf::Texture& carTex = ResourceManager<sf::Texture>::getInstance().get("assets/car.png");
    
     
    car p1("Player 1", vector(385.f, 575.f), 100, 1, carTex);
    p1.roteste(90.f); 
    gameCircuit.addCar(p1);

     
    if (isMultiplayer) {
        sf::Texture& car2Tex = ResourceManager<sf::Texture>::getInstance().get("assets/car2.png");
        car p2("Player 2", vector(385.f + 60.f, 575.f), 100, 1, car2Tex);
        p2.roteste(90.f);
        gameCircuit.addCar(p2);
    }

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

    for (int i = 0; i < (isMultiplayer ? 2 : 1); ++i) {
        if (car* pCar = gameCircuit.getCar(i)) {
            pCar->setMaxFuel(fuelPerLap * 3.0); 
            pCar->setFuel(fuelPerLap * 1.5); 
        }
    }
    gameCircuit.regeneratePowerUps();
    
    lastLap[0] = 0; lastLap[1] = 0;
    cachedPowerUps[0] = 0; cachedPowerUps[1] = 0;
    cachedLaps[0] = 0; cachedLaps[1] = 0;
    hud.setMultiplayerMode(isMultiplayer);
    
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
             
            if (menu.isActive()) {
                if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up) {
                    menu.moveUp();
                }
                if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down) {
                    menu.moveDown();
                }
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    MenuState state = menu.getState();
                    int selection = menu.getPressedItem();

                    if (state == MenuState::MainMenu) {
                        if (selection == 0) menu.setState(MenuState::SPLaps);
                        else if (selection == 1) menu.setState(MenuState::MPLaps);
                        else if (selection == 2) window.close();
                    }
                    else if (state == MenuState::SPLaps) {
                        int laps = menu.getSelectedLapsSP();
                        if (laps == 0 && selection == 4) {  
                            menu.setState(MenuState::MainMenu);
                        } else {
                            resetGame(false, laps);
                            menu.setState(MenuState::Inactive);
                        }
                    }
                    else if (state == MenuState::MPLaps) {
                        int laps = menu.getSelectedLapsMP();
                        if (laps == 0 && selection == 4) {  
                            menu.setState(MenuState::MainMenu);
                        } else {
                            resetGame(true, laps);
                            menu.setState(MenuState::Inactive);
                        }
                    }
                    else if (state == MenuState::Pause) {
                        if (selection == 0) menu.setState(MenuState::Inactive);  
                        else if (selection == 1) {  
                            resetGame(isMultiplayer, targetLaps);
                            menu.setState(MenuState::Inactive);
                        }
                        else if (selection == 2) menu.setState(MenuState::MainMenu);  
                    }
                }
            } 
            else if (isGameOver) {
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    resetGame(isMultiplayer, targetLaps);
                } else if (keyPressed->code == sf::Keyboard::Key::Q) {
                    menu.setState(MenuState::MainMenu);
                    isGameOver = false;  
                }
            }
            else {
                 
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    menu.setState(MenuState::Pause);
                    std::cout << "Game Paused\n";
                }
            }
        }
    }
}

void Game::update(float dTime) {
    int playerCount = isMultiplayer ? 2 : 1;
    bool anyAlive = false;

    for (int i = 0; i < playerCount; ++i) {
        car* pCar = gameCircuit.getCar(i);
        if (!pCar || pCar->eliminata()) continue;
        
        anyAlive = true;
        
         
        cachedPowerUps[i] = pCar->getCollectedPowerUps();
        cachedLaps[i] = checkpointManager.getLaps(i);

         
        int currentLap = checkpointManager.getLaps(i);
        if (currentLap > lastLap[i]) {
            if (currentLap > 0 && currentLap % 2 == 0) {
                gameCircuit.regeneratePowerUps();
                hud.showMessage("PowerUps Regenerate!");
            }
            lastLap[i] = currentLap;
        }

         
        if (targetLaps > 0 && currentLap >= targetLaps) {
            isGameOver = true;
            winnerName = (i == 0 ? "Player 1" : "Player 2");
            hud.showMessage(winnerName + " Wins!");
            return;
        }

         
        float moveAcceleration = 200.f;  
        float rotationSpeed = 150.f;

        if (i == 0) {  
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) pCar->roteste(-rotationSpeed * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) pCar->roteste(rotationSpeed * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) pCar->acceleratie(moveAcceleration * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) pCar->acceleratie(-moveAcceleration * 0.5f * dTime);
        } else {  
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) pCar->roteste(-rotationSpeed * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) pCar->roteste(rotationSpeed * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) pCar->acceleratie(moveAcceleration * dTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) pCar->acceleratie(-moveAcceleration * 0.5f * dTime);
        }

        checkpointManager.update(*pCar, i);
    }

    if (!anyAlive) {
        isGameOver = true;
        return;
    }

    gameCircuit.simulat(dTime);
    
     
    if (car* p1 = gameCircuit.getCar(0)) {
        int lapP1 = checkpointManager.getLaps(0);
        int lapP2 = isMultiplayer ? checkpointManager.getLaps(1) : 0;
        int cpP1 = checkpointManager.getNextCheckpointIndex(0);
        int cpP2 = isMultiplayer ? checkpointManager.getNextCheckpointIndex(1) : 0;
        
        int damageP2 = 0;
        if (isMultiplayer) {
            if (car* p2 = gameCircuit.getCar(1)) {
                damageP2 = p2->getDamage();
            }
        }

        hud.update(*p1, dTime, lapP1, lapP2, cpP1, cpP2, damageP2);
    }
    
    std::vector<std::string> msgs = gameCircuit.popMessages();
    for (const auto& msg : msgs) {
        hud.showMessage(msg);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);
    
    if (!isMultiplayer) {
        checkpointManager.draw(window, 0);
    } else {
        checkpointManager.draw(window, -1);
    }

    for (const auto& obs : gameCircuit.getObstacole()) {
        obs.draw(window);
    }

    for (const auto& pwrUp : gameCircuit.getPowerUps()) {
        pwrUp->draw(window);
    }

    if (!isGameOver || true) { 
        int count = isMultiplayer ? 2 : 1;
        for(int i=0; i<count; ++i) {
            if (car* pCar = gameCircuit.getCar(i)) {
                if(!pCar->eliminata()) pCar->draw(window);
            }
        }
    }

    if (!menu.isActive()) {
        hud.draw(window);
    }

    if (isGameOver) {
        hud.drawGameOver(window, cachedPowerUps[0], cachedLaps[0], winnerName); 
    }

    if (menu.isActive()) {
        menu.draw(window);
    }

    window.display();
}