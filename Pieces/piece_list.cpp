//
// Created by tobedetered on 1/27/24.
//

#include "piece_list.h"

#include <iostream>

piece_list::piece_list(color color_, std::vector<Piece*>* pieces_) {
    list.emplace(color_, pieces_);
}

piece_list::piece_list() {
    list.emplace(white, new std::vector<Piece*>());
    list.emplace(black, new std::vector<Piece*>());
}

piece_list::~piece_list() {
    for(const Piece* piece: list[white]) {
        delete piece;
    }
    for(const Piece* piece: list[black]) {
        delete piece;
    }
    delete list[white];
    delete list[black];
}

std::vector<Piece*>* piece_list::getVectorList(const color pieceColor) {return list[pieceColor];}
