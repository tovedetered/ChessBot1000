//
// Created by tobedetered on 2/2/24.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "../Piece.h"


class Bishop : public Piece{
public:
    Bishop(int index, color pieceColor, piece type)
        : Piece(index, pieceColor, type) {

    }
};



#endif //BISHOP_H
