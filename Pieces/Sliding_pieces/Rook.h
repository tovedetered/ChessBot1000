//
// Created by tobedetered on 2/2/24.
//

#ifndef ROOK_H
#define ROOK_H
#include "SlidingPiece.h"
#include "../Piece.h"


class Rook : public SlidingPiece{
public:
    Rook(int index, color pieceColor, piece type)
        : SlidingPiece(index, pieceColor, type) {

    }
};



#endif //ROOK_H
