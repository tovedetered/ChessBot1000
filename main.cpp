#include <iostream>
#include <raylib.h>
#include "BoardandPieces/Board/Board.h"

int main() {
    Board* board = new Board;
    MoveGenerator moveGen(board);
    board->setMoveGen(&moveGen);
    std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board->loadPosFromFen(fenString);
    uint64_t nodes = board->perft(5);

    std::cout << nodes << std::endl;
    return 0;
}
