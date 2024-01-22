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
    bool playerWhite = true;
    bool whiteTurn = true;
    bool completeMove = false;
    bool moveStart = false;
    bool movesGenerated = false;
    move_stats move{};
    while(interface->isRunning()) {
        //Generate Moves
        //get input
        if(((playerWhite && whiteTurn) || (!playerWhite && !whiteTurn)) && !completeMove) {
            fileRank input = interface->getPlayerInput();
            if(input.file == 30) {
                goto Draw;
            }
            if(input.file == 9 && moveStart) {
                moveStart = false;
                continue;
            }
            if(!moveStart) {
                move.start = input;
                moveStart = true;
            }
            else {
                move.end = input;
                moveStart = false;
                completeMove = true;
            }
        }
        else {
            //get robot move
        }
        //update

        if(completeMove) {
            if(board->isLegalMove(move.start, move.end) || false /*TODO: Players.bothBots?*/) {
                board->movePiece(move.start, move.end);
                whiteTurn = !whiteTurn;
                movesGenerated = false;
            }

            move.end = {0,0};
            move.start = {0,0};
            completeMove = false;
        }
        Draw:
        //draw
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
