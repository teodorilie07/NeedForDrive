#include <iostream>
#include "circuit.h"


int main() {
    std::cout << "creearea obiectelor\n";
    circuit monza("Circuitul Monza");
    car hamilton("Hamilton", vector(10, 20), 100, 5);
    car vettel("Vettel", vector(10, 25), 90, 6);
    obstacol obst(vector(50, 22), 5.0f);

    std::cout << "\n configurarea circuitului \n";
    monza.addCar(hamilton);
    monza.addCar(vettel);
    monza.addObst(obst);
    monza.addObst(obstacol(vector(80, 28), 3.0f)); //creare si adaugare directa

    std::cout << monza;

    std::cout << "\n simulare \n";
    const int numarPasiSimulare = 4;
    const float dTime = 1.5; //pas=1.5s
    for (int i = 0; i < numarPasiSimulare; ++i) 
    {
        monza.simulat(dTime);
        std::cout << monza; //afiseaza starea circuitului
    }

    std::cout << "\n cursa s-a incheiat. Obiectele vor fi distruse automat \n";
    
    return 0;
}
