#include "game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[CRITICAL ERROR] " << e.what() << "\n";
        return -1;
    }

    return 0;
}