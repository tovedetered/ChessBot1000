//
// Created by tobedetered on 1/11/24.
//

#include "GameManager.h"

#include <iostream>

GameManager::GameManager() {
    board = new Board;
    moveGen = new MoveGenerator(board);
    board->setMoveGen(moveGen);
    interface = new GraphicalBoard(1000, 1000);
}

void GameManager::startup() {
    interface->init(board);
    board->loadPosFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void GameManager::mainLoop() {
    bool playerWhite = true;
    bool whiteTurn = true;
    bool completeMove = false;
    bool moveStart = false;
    bool movesGenerated = false;
    move inputMove{};
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
                inputMove.startIndex = access(input.file, input.rank);
                moveStart = true;
            }
            else {
                inputMove.endIndex = access(input.file, input.rank);
                moveStart = false;
                completeMove = true;
            }
        }
        else {
            //get robot move
        }
        //update

        /*if(completeMove) {
            if(board->isLegalMove(inputMove.start, inputMove.end) || false TODO: Players.bothBots?) {
                board->movePiece(inputMove.start, inputMove.end);
                whiteTurn = !whiteTurn;
                movesGenerated = false;
            }

            inputMove.end = {0,0};
            inputMove.start = {0,0};
            completeMove = false;
        }
        */
        Draw:
        //draw
        BeginDrawing();
        interface->renderLoop();
        EndDrawing();
        uint64_t nodes = board->perft(5); //correct to depth 3

        //incorrect: c2-c3
        //c2-c4
        //e2-e3
        //e2-e4
        std::cerr << nodes << std::endl; //pins are broke
        std::cerr << board->debug.captures << " " << board->debug.checks << " " << std::endl;
        break;
    }
}

void GameManager::shutDown() {
    interface->shutdown();
    delete board;
    delete interface;

}
