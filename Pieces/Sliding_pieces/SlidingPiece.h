//
// Created by tobedetered on 2/15/24.
//

#ifndef SLIDINGPIECE_H
#define SLIDINGPIECE_H

#include "../Piece.h"

class SlidingPiece : public Piece{
public:
    SlidingPiece(int index, color pieceColor, piece type)
        : Piece(index, pieceColor, type) {
    }
protected:
    int startDir;
    int endDir;
};



#endif //SLIDINGPIECE_H
