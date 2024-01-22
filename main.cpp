#include <iostream>
#include <raylib.h>
#include "GameManager.h"

int main() {
    /*
    GameManager game;
    game.startup();
    game.mainLoop();
    game.shutDown();
    return 0;
    */
    unsigned long long int rayMask = 0;

    for (int i = 0; i < 4; i++) {
        int squareIndex = 10 + 8 * (i + 1);
        rayMask |= 1ul << squareIndex;
        std::cout << rayMask << std::endl;
    }
}
