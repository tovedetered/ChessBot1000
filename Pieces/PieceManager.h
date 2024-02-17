//
// Created by tobedetered on 2/2/24.
//

#ifndef PIECEMANAGER_H
#define PIECEMANAGER_H

#include "Piece.h"
#include "../CommonUtilities.h"
#include "../BoardandPieces/Pieces.h"

class PieceManager {
public:
    PieceManager() = default;
    [[nodiscard]] create_piece_data getPieceData(int piece) const;
    [[nodiscard]] Piece* getPiece(int piece, int index) const;
private:
    pieces piece_ref;
};



#endif //PIECEMANAGER_H
