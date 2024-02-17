//
// Created by tobedetered on 2/2/24.
//

#ifndef PIECE_H
#define PIECE_H
#include "PieceManager.h"
#include "../CommonUtilities.h"
#include "../board_representations/SquareBoardRepresentation.h"


class Piece {
public:
    virtual ~Piece() = default;
    [[nodiscard]] int getIndex() const;
    [[nodiscard]] piece getType() const;
    [[nodiscard]] color getColor() const;
    Piece(int index, color pieceColor, piece type);
    void setIndex(int index);
    [[nodiscard]] int getPieceValue() const;
protected:
    int index;
    piece type;
    color pieceColor;
};



#endif //PIECE_H
