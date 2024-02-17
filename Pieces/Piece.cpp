//
// Created by tobedetered on 2/2/24.
//

#include "Piece.h"

int Piece::getIndex() const { return index; }

piece Piece::getType() const { return type; }

color Piece::getColor() const { return pieceColor; }

Piece::Piece(int index, color pieceColor, piece type) {
    this->index = index;
    this->pieceColor = pieceColor;
    this->type = type;
}


void Piece::setIndex(const int index) { this->index = index; }

int Piece::getPieceValue() const {
    return static_cast<int>(pieceColor) | static_cast<int>(type);
}
