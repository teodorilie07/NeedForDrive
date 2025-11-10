#include <SFML/Graphics.hpp>
#include <iostream>     // Pentru 'cout' din codul tău comentat
#include "circuit.h"
#include "vector.h"     // Header-ul pentru clasa ta 'vector'
#include "car.h"        // Header-ul pentru clasa 'car'
#include "obstacol.h"   // Header-ul pentru clasa 'obstacol'

// --- Funcție ajutătoare ---
// Convertește vectorul tău custom la un vector SFML
// (Presupune că ai adăugat getX() și getY() în vector.h)
sf::Vector2f toSfmlVector(const vector& v)
{
    return {v.getx(), v.gety()};
}

int main() {
    /*
        circuit monza("Circuitul Monza");

        std::cout << "Incarcarea circuitului din fisierul 'circuit.txt'...\n";
        if (!monza.incarcaFisier("circuit.txt"))
        {
            std::cout << "EROARE DESCHIDERE FISIER Programul se va inchide\n";
            return 1;
        }

        std::cout << "\n--- Configurarea initiala a circuitului ---\n";
        std::cout << monza;

        std::cout << "\n--- INCEPE SIMULAREA ---\n";
        constexpr int numarPasiSimulare = 10;
        constexpr float dTime = 1.5f;

        for (int i = 0; i < numarPasiSimulare; ++i)
        {
            monza.simulat(dTime);
            std::cout << monza;
        }

        std::cout << "\n--- CURSA S-A INCHEIAT ---\n";
        std::cout << "Obiectele ramase vor fi distruse automat.\n";
    */



    sf::RenderWindow window(sf::VideoMode({800, 600}), "NFD - Logic vs Visual");
    window.setFramerateLimit(60);


    sf::Clock clock;

    circuit circuitul("Circuitul Meu");




    car masinaLogica("Player", vector(400.f, 500.f), 100, 1);
    circuitul.addCar(masinaLogica); // Adaugă mașina în circuit



    obstacol obstacolLogic(vector(350.f, 150.f), 50.f);
    circuitul.addObst(obstacolLogic);



    sf::RectangleShape masinaShape({40.f, 70.f});
    masinaShape.setFillColor(sf::Color::Green);

    masinaShape.setOrigin({20.f, 35.f});

    sf::CircleShape obstacolShape(50.f);
    obstacolShape.setFillColor(sf::Color::Red);
    obstacolShape.setOrigin({50.f, 50.f});



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
        float moveFactor = 100.f; // Cât de tare să accelereze


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            playerCar.acceleratie(vector(0.f, -1.f), moveFactor * dTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            playerCar.acceleratie(vector(0.f, 1.f), moveFactor * dTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            playerCar.acceleratie(vector(-1.f, 0.f), moveFactor * dTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            playerCar.acceleratie(vector(1.f, 0.f), moveFactor * dTime);
        }


        circuitul.simulat(dTime);



        masinaShape.setPosition(toSfmlVector(playerCar.getPozitie()));


        if (!circuitul.getObstacole().empty()) {
            obstacolShape.setPosition(toSfmlVector(circuitul.getObstacole()[0].getPozitie()));
        }

        window.clear(sf::Color::Black);

        window.draw(obstacolShape);
        window.draw(masinaShape);

        window.display();
    }

    return 0;
}