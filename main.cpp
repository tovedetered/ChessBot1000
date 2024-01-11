#include <iostream>
#include <raylib.h>
#include "BoardandPieces/Renderers/GraphicalBoard.h"

int main() {
    GraphicalBoard board(1000, 1000);
    board.init();

    while(!WindowShouldClose()) {
        BeginDrawing();
        board.renderLoop();
        EndDrawing();
    }

    CloseWindow();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
