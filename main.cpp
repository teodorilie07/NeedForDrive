#include <SFML/Graphics.hpp>
#include <iostream>
#include "circuit.h"
#include "vector.h"
#include "car.h"
#include "obstacol.h"

sf::Vector2f toSfmlVector(const vector& v)
{
    return {v.getx(), v.gety()};
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1024, 640}), "NFD");
    window.setFramerateLimit(60);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/track.png"))
    {
        std::cerr << "Eroare: Nu s-a putut incărca assets/track.png\n";
        return -1;
    }

    sf::Texture carTexture;
    if (!carTexture.loadFromFile("assets/car.png"))
    {
        std::cerr << "Eroare: Nu s-a putut incărca assets/car.png\n";
        return -1;
    }

    sf::Texture obstacolTexture;
    if (!obstacolTexture.loadFromFile("assets/obs.png")) { // Asigură-te că e calea corectă
        std::cerr << "Eroare: Nu s-a putut încărca assets/obs.png\n";
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);

    circuit circuitul("Circuitul Monza");

    car masinaLogica("Player", vector(400.f, 500.f), 100, 1, carTexture);
    circuitul.addCar(masinaLogica);

    std::cout << "Incarcarea circuitului din fisierul 'tastatura.txt'...\n";
    if (!circuitul.incarcaFisier("tastatura.txt"))
    {
        std::cout << "EROARE DESCHIDERE FISIER Programul se va inchide\n";
        return 1;
    }

    obstacol obstacolLogic(vector(350.f, 150.f), 15.f, obstacolTexture);
    circuitul.addObst(obstacolLogic);

    std::cout << "\n--- Configurarea initiala a circuitului ---\n";
    std::cout << circuitul;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dTime = dt.asSeconds();

        while (const auto event = window.pollEvent())
        {
            // Verificarea pentru închidere (e corectă)
            if (event->getIf<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Backspace)
                {
                    window.close();
                }
            }
        }

        car* playerCar = circuitul.getPlayerCar();

        if (playerCar != nullptr)
        {
            float moveAcceleration = 100.f;
            float rotationSpeed = 100.f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                playerCar->roteste(-rotationSpeed * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                playerCar->roteste(rotationSpeed * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                playerCar->acceleratie(moveAcceleration * dTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                playerCar->acceleratie(-moveAcceleration * 0.5f * dTime);
            }
        }

        circuitul.simulat(dTime);

        playerCar = circuitul.getPlayerCar();

        window.clear(sf::Color::Black);

        window.draw(backgroundSprite);

        for (const auto& obs : circuitul.getObstacole())
        {
            obs.draw(window);
        }

        if (playerCar != nullptr)
        {
            playerCar->draw(window);
        }

        window.display();
    }

    return 0;
}