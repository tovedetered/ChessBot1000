//
// Created by tobedetered on 2/2/24.
//

#include "PieceManager.h"

#include "Other_pieces/King.h"
#include "Other_pieces/Knight.h"
#include "Other_pieces/Pawn.h"
#include "Sliding_pieces/Bishop.h"
#include "Sliding_pieces/Queen.h"
#include "Sliding_pieces/Rook.h"

create_piece_data PieceManager::getPieceData(const int piece) const {
    create_piece_data data{};
    int pieceType = piece & piece_ref.typeMask;
    data.type = static_cast<::piece>(pieceType);

    int pieceColor = piece & piece_ref.colorMask;
    data.pieceColor = static_cast<color>(pieceColor);

    return data;
}

Piece* PieceManager::getPiece(const int piece, const int index) const {
    switch(auto [pieceColor, type] = getPieceData(piece); type) {
    case pawn:
        return new Pawn(index, pieceColor, type);
    case knight:
        return new Knight(index, pieceColor, type);
    case rook:
        return new Rook(index, pieceColor, type);
    case bishop:
        return new Bishop(index, pieceColor, type);
    case king:
        return new King(index, pieceColor, type);
    case queen:
        return new Queen(index, pieceColor, type);
    case none:
        return nullptr;
    }
    return nullptr;
}
