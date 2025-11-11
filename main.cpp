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
    circuit  circuitul("Circuitul Monza");

    car masinaLogica("Player", vector(400.f, 500.f), 100, 1);
    circuitul.addCar(masinaLogica);

    std::cout << "Incarcarea circuitului din fisierul 'tastatura.txt'...\n";
    if (! circuitul.incarcaFisier("tastatura.txt"))
    {
        std::cout << "EROARE DESCHIDERE FISIER Programul se va inchide\n";
        return 1;
    }

    obstacol obstacolLogic(vector(350.f, 150.f), 50.f);
    circuitul.addObst(obstacolLogic);

    std::cout << "\n--- Configurarea initiala a circuitului ---\n";
    std::cout <<  circuitul;


    sf::RenderWindow window(sf::VideoMode({800, 600}), "NFD");
    window.setFramerateLimit(60);


    sf::Clock clock;

    sf::RectangleShape masinaShape({40.f, 70.f});
    masinaShape.setFillColor(sf::Color::Green);
    masinaShape.setOrigin({20.f, 35.f});

    std::vector<sf::CircleShape> obstacolShapes;
    for (const auto& obsLogic : circuitul.getObstacole())
    {
        sf::CircleShape shape(obsLogic.getRaza());
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin({obsLogic.getRaza(), obsLogic.getRaza()});
        shape.setPosition(toSfmlVector(obsLogic.getPozitie()));
        obstacolShapes.push_back(shape);
    }


    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dTime = dt.asSeconds();

        while (const auto event = window.pollEvent())
        {
            if (event->getIf<sf::Event::Closed>())
            {
                window.close();
            }
        }

        car& playerCar = circuitul.getPlayerCar();

        float moveAcceleration = 100.f;
        float rotationSpeed = 100.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            playerCar.roteste(-rotationSpeed * dTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            playerCar.roteste(rotationSpeed * dTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            playerCar.acceleratie(moveAcceleration * dTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            playerCar.acceleratie(-moveAcceleration * 0.5f * dTime);
        }

        circuitul.simulat(dTime);

        masinaShape.setPosition(toSfmlVector(playerCar.getPozitie()));

        masinaShape.setRotation(sf::degrees(playerCar.getUnghi()));


        window.clear(sf::Color::Black);

        for (const auto& shape : obstacolShapes)
        {
            window.draw(shape);
        }

        window.draw(masinaShape);

        window.display();
    }

    return 0;
}