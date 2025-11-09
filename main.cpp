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


    // 1. --- Inițializarea Ferestrei ---
    sf::RenderWindow window(sf::VideoMode({800, 600}), "NFD - Logic vs Visual");
    window.setFramerateLimit(60);

    // Ceas pentru a măsura dTime (delta time)
    sf::Clock clock;

    // 2. --- Crearea Obiectelor de LOGICĂ ---
    circuit circuitul("Circuitul Meu");

    // Creăm logica mașinii (folosind constructorul din car.h)
    // Nume, Pozitie initiala, Combustibil, Consum

    // CORECTAT: Am schimbat {400.f, 500.f} în vector(400.f, 500.f)
    car masinaLogica("Player", vector(400.f, 500.f), 100, 1);
    circuitul.addCar(masinaLogica); // Adaugă mașina în circuit

    // Creăm logica obstacolului (folosind constructorul din obstacol.h)

    // CORECTAT: Am apelat constructorul cu (vector, rază)
    obstacol obstacolLogic(vector(350.f, 150.f), 50.f); // Pozitie, Raza
    circuitul.addObst(obstacolLogic); // Adaugă obstacolul în circuit


    // 3. --- Crearea Obiectelor VIZUALE ---
    // Forma vizuală pentru mașină
    sf::RectangleShape masinaShape({40.f, 70.f});
    masinaShape.setFillColor(sf::Color::Green);
    // Setează originea pe centru pentru rotații mai ușoare (opțional)
    masinaShape.setOrigin({20.f, 35.f});

    // CORECTAT: Folosim CircleShape deoarece obstacolul are o rază
    sf::CircleShape obstacolShape(50.f); // Raza de 50
    obstacolShape.setFillColor(sf::Color::Red);
    obstacolShape.setOrigin({50.f, 50.f}); // Originea e în centrul cercului


    // 4. --- Bucla Principală ---
    while (window.isOpen())
    {
        // Calculăm dTime (timpul scurs de la ultimul cadru)
        sf::Time dt = clock.restart();
        float dTime = dt.asSeconds();

        // 5. --- Procesarea Evenimentelor ---
        while (const auto event = window.pollEvent())
        {
            if (event->getIf<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 6. --- Procesarea Input-ului (Trimite comenzi logicii) ---
        car& playerCar = circuitul.getPlayerCar();
        float moveFactor = 100.f; // Cât de tare să accelereze

        // CORECTAT: Am schimbat {x, y} în vector(x, y) pentru toate tastele
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

        // 7. --- Logica Jocului (Update) ---
        circuitul.simulat(dTime);


        // 8. --- Sincronizare Vizuală (Actualizează vizualul cu logica) ---
        masinaShape.setPosition(toSfmlVector(playerCar.getPozitie()));

        // Setează poziția obstacolului (presupunând că avem doar 1)
        if (!circuitul.getObstacole().empty()) {
            obstacolShape.setPosition(toSfmlVector(circuitul.getObstacole()[0].getPozitie()));
        }

        // 9. --- Randarea (Desenarea) ---
        window.clear(sf::Color::Black);

        window.draw(obstacolShape); // Desenează forma obstacolului
        window.draw(masinaShape);   // Desenează forma mașinii

        window.display();
    }

    return 0;
}