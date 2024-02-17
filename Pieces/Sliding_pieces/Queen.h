//
// Created by tobedetered on 2/2/24.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "SlidingPiece.h"
#include "../Piece.h"


class Queen : public SlidingPiece{
public:
    Queen(int index, color pieceColor, piece type)
        : SlidingPiece(index, pieceColor, type) {

    }
};



#endif //QUEEN_H
