//
// Created by tobedetered on 2/2/24.
//

#ifndef SQUAREBOARDREPRESENTATION_H
#define SQUAREBOARDREPRESENTATION_H

#include <vector>
#include "../Pieces/Piece.h"
#include "../Pieces/piece_list.h"
#include "../CommonUtilities.h"

class SquareBoardRepresentation {
public:
    SquareBoardRepresentation();
    ~SquareBoardRepresentation();
    void updateBoard(const int numBoard[64]);

    Piece* getPieceAtSquare(int square) const;
    std::vector<Piece*> getPieceList(color color_, piece type);
    int getKingSquare(color kingColor);

    //WILL NOT CHECK TO SEE IF POSSIBLE will just move the piece
    void movePiece(int startSquare, int endSquare);
    void removePiece(int squareToRemove);
    void addPiece(piece_data pieceToAdd);

private:
    Piece* board[64]{};

    std::unordered_map<piece, piece_list*> pieceListMap;

    PieceManager manager;
};



#endif //SQUAREBOARDREPRESENTATION_H
