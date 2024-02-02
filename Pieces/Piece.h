//
// Created by tobedetered on 2/2/24.
//

#ifndef PIECE_H
#define PIECE_H
#include "PieceManager.h"
#include "../CommonUtilities.h"


class Piece {
public:
    Piece(int index, color pieceColor, piece type);
    int getIndex();
    void setIndex(int index);
    piece getType();
    color getColor();

private:
    int index;
    piece type;
    color pieceColor;
};



#endif //PIECE_H
