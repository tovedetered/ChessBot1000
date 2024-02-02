//
// Created by tobedetered on 2/2/24.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "../Piece.h"


class Queen : public Piece{
public:
    Queen(int index, color pieceColor, piece type)
        : Piece(index, pieceColor, type) {

    }
};



#endif //QUEEN_H
