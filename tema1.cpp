#pragma once
#include <iostream>
#include "circuit.h"


int main() 
{

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
    const int numarPasiSimulare = 10; 
    const float dTime = 1.5f;       

    for (int i = 0; i < numarPasiSimulare; ++i)
    {
        monza.simulat(dTime);
        std::cout << monza;    
    }

    std::cout << "\n--- CURSA S-A INCHEIAT ---\n";
    std::cout << "Obiectele ramase vor fi distruse automat.\n";

    return 0;
}
