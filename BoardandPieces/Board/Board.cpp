//
// Created by tobedetered on 1/13/24.
//

#include "Board.h"

#include <iostream>
#include <process.h>

Board::Board() {
    std::fill(std::begin(board), std::end(board), 0);
}

gameState Board::getCurrentGameState() const {return currentState;}

int Board::getKingPos(const color color) const {return kingsPos[color];}

int Board::getPieceAtSquare(const int index) const {return board[index];}

std::vector<piece_data> Board::getPiceColorList(const piece piece, const color color) {
    return pieceListMap[piece][color];
}

bool Board::isColor(int piece, color color) const {
    if(color == white && pieceRef.colorMask & piece == pieceRef.white) return true;
    if(color == black && pieceRef.colorMask & piece == pieceRef.black) return true;
    return false;
}

color Board::getPieceColor(const int piece_) const {
    int colorType = piece_&pieceRef.colorMask; //isolate color
    const auto pieceColor = static_cast<color>(colorType);
    return pieceColor;
}

piece Board::getPieceType(const int piece_) const {
    int pieceType = piece_&pieceRef.typeMask; //isolate piece code
    const auto result = static_cast<piece>(pieceType);
    return result;
}

void Board::makeMove(move currentMove) {
    //1st save current Game-State
    gameHistory.emplace(currentState);
    currentState.fiftyPlyCount ++;
    currentState.totalPly ++;
    currentState.switchColor();
    //now make the move
    const int movingPiece = board[currentMove.startIndex];
    board[currentMove.startIndex] = 0;
    int miscSquare = -1;
    //Catleing
    if(currentMove.flag != normal && static_cast<int>(currentMove.flag) < 5) {
        int movementMultiplier = 0;
        switch (static_cast<int>(currentMove.flag)) {
        case 1: //W-KS
            miscSquare = 63;
            movementMultiplier = 1;
            break;
        case 2:
            miscSquare = 56;
            movementMultiplier = -1;
            break;
        case 3:
            miscSquare = 7;
            movementMultiplier = 1;
            break;
        case 4:
            miscSquare = 0;
            movementMultiplier = -1;
            break;
        default:
            std::cerr << "WTF how the hell did this happen???";
            exit(-950);
        }
        const int targetRook = board[miscSquare];
        //remove Rook from the board
        board[miscSquare] = 0;
        //put king on new square
        board[currentMove.startIndex + 2*movementMultiplier] = movingPiece;
        //put the rook on its new square
        board[miscSquare - 2*movementMultiplier] = targetRook;
    }
    else if(currentMove.flag == twoSquarePawnMove) {

        board[currentMove.endIndex] = movingPiece;
    }
    else {
        board[currentMove.endIndex] = movingPiece;
    }
}
