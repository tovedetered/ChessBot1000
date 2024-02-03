//
// Created by tobedetered on 2/2/24.
//

#include "Piece.h"

Piece::Piece(int index, color pieceColor, piece type) {
    this->index = index;
    this->type = type;
    this->pieceColor = pieceColor;
}
void Piece::setIndex(const int index) { this->index = index; }
