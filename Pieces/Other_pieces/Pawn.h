//
// Created by tobedetered on 2/2/24.
//

#ifndef PAWN_H
#define PAWN_H
#include "../Piece.h"


class Pawn : public Piece {
public:
    Pawn(int index, color pieceColor, piece type)
        : Piece(index, pieceColor, type) {
    }

    std::vector<move> getMoves(SquareBoardRepresentation* board) override;
};



#endif //PAWN_H
