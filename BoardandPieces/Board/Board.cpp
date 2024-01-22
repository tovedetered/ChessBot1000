//
// Created by tobedetered on 1/13/24.
//

#include "Board.h"

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
