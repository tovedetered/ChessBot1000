//
// Created by tobedetered on 2/2/24.
//

#ifndef ROOK_H
#define ROOK_H
#include "../Piece.h"


class Rook : public Piece{
public:
    Rook(int index, color pieceColor, piece type)
        : Piece(index, pieceColor, type) {

    }
};



#endif //ROOK_H
