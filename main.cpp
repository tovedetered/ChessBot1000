#include <iostream>
#include <raylib.h>
#include "GameManager.h"

int main() {
    GameManager game;
    game.startup();
    game.mainLoop();
    game.shutDown();
    return 0;
}
