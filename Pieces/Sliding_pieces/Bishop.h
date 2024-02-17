//
// Created by tobedetered on 2/2/24.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "SlidingPiece.h"
#include "../Piece.h"


class Bishop : public SlidingPiece{
public:
    Bishop(int index, color pieceColor, piece type)
        : SlidingPiece(index, pieceColor, type) {

    }
};



#endif //BISHOP_H
