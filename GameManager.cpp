//
// Created by tobedetered on 1/11/24.
//

#include "GameManager.h"

GameManager::GameManager() {
    board = new SquareBoard;
    interface = new GraphicalBoard(1000, 1000);
}

void GameManager::startup() {
    interface->init(board);
}

void GameManager::mainLoop() {
    while(interface->isRunning()) {
        BeginDrawing();
        interface->renderLoop();
        EndDrawing();
    }
}

void GameManager::shutDown() {
    interface->shutdown();
    delete board;
    delete interface;
}
