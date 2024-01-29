#include <iostream>
#include <raylib.h>

#include "GameManager.h"
#include "BoardandPieces/Board/Board.h"

int main() {
    GameManager manager;
    manager.startup();
    manager.mainLoop();
    manager.shutDown();

    return 0;
}
