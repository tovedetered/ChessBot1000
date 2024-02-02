//
// Created by tobedetered on 2/2/24.
//

#include "SquareBoardRepresentation.h"

SquareBoardRepresentation::SquareBoardRepresentation() {
    //Initialize the piece lists for each piece
    pieceListMap.emplace(pawn, new piece_list);
    pieceListMap.emplace(knight, new piece_list);
    pieceListMap.emplace(rook, new piece_list);
    pieceListMap.emplace(bishop, new piece_list);
    pieceListMap.emplace(king, new piece_list);
    pieceListMap.emplace(queen, new piece_list);

    //initialize the board to an empty one
    board = {};
}

SquareBoardRepresentation::~SquareBoardRepresentation() {
    //should delete the pieces as well
    delete pieceListMap[pawn];
    delete pieceListMap[knight];
    delete pieceListMap[rook];
    delete pieceListMap[bishop];
    delete pieceListMap[king];
    delete pieceListMap[queen];

    //If it does not
    for (const auto piece : board) {
        delete piece;
    }
}

//Takes an input of a board represented by integers and converts it to objects (this is due to the fen init)
void SquareBoardRepresentation::updateBoard(const int numBoard[64]) {
    for(int i = 0; i < 64; i++) {
        int item = numBoard[i];
        board[i] = manager.getPiece(item);
    }
}

Piece* SquareBoardRepresentation::getPieceAtSquare(const int square) const {return board[square];}

std::vector<Piece*> SquareBoardRepresentation::getPieceList(const color color_, const piece type) {
    piece_list* list = pieceListMap[type];
    return *list->getVectorList(color_);
}

int SquareBoardRepresentation::getKingSquare(const color kingColor) {
    piece_list* list = pieceListMap[king];
    //Will always be one Unless something went horribly wrong
    const std::vector<Piece*> kingsOfColor = *list->getVectorList(kingColor);
    return kingsOfColor[0]->getIndex();
}

void SquareBoardRepresentation::movePiece(const int startSquare, const int endSquare) {
    //Because they are objects we can just move them in one place, and not both...
    //Basically we keep track of pos in object not map
    board[startSquare]->setIndex(endSquare);
    if(board[endSquare] != nullptr)
        removePiece(endSquare);

    board[endSquare] = board[startSquare];
    board[startSquare] = nullptr;
}

void SquareBoardRepresentation::removePiece(const int squareToRemove) {
    Piece* toBeRemoved = board[squareToRemove];
    const color colorRemove = toBeRemoved->getColor();
    const piece pieceRemove = toBeRemoved->getType();

    board[squareToRemove] = nullptr;

    //we need to find the piece in the vector then remove it
    std::vector<Piece*>* pieces = pieceListMap[pieceRemove]->getVectorList(colorRemove);
    for(int i = 0; i < pieces->size(); i ++) {
        if(Piece* item = pieces->at(i); item->getIndex() == squareToRemove) {
            pieces->erase(pieces->begin() + i);
        }
    }
    //It is either in the vector where it was erased, or it was not and its being deleted
    delete toBeRemoved;
}

void SquareBoardRepresentation::addPiece(const piece_data pieceToAdd) {
    const int index = pieceToAdd.index;
    Piece* toBeAdded = manager.getPiece(pieceToAdd.piece, index);
    if(board[index] != nullptr) {
        removePiece(index);
    }
    board[index] = toBeAdded;

    color addColor = toBeAdded.getColor();
    piece addType = toBeAdded.getType();

    //has to be the pointer becasuse we are modifying the vector
    std::vector<Piece*>* listToAddTo = pieceListMap[addType]->getVectorList(addColor);
}
