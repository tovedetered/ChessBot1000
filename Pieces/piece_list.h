//
// Created by tobedetered on 1/27/24.
//

#ifndef PIECE_LIST_H
#define PIECE_LIST_H

#include <unordered_map>
#include <vector>

#include "../CommonUtilities.h"
#include "../Pieces/Piece.h"

class piece_list {
public:
    piece_list(color color_, std::vector<Piece*>*);
    piece_list();
    ~piece_list();

    std::vector<Piece*>* getVectorList(color pieceColor);
private:
    std::unordered_map<color, std::vector<Piece*>*> list;
};



#endif //PIECE_LIST_H
