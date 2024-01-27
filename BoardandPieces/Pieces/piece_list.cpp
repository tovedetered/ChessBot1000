//
// Created by tobedetered on 1/27/24.
//

#include "piece_list.h"

#include <iostream>

piece_list::piece_list(color color_, std::vector<piece_data>* pieces_) {
    list.emplace(color_, pieces_);
}

piece_list::piece_list() {
    list.emplace(white, new std::vector<piece_data>());
    list.emplace(black, new std::vector<piece_data>());
    std::cerr << "WORKED!!!!" << std::endl;
}

piece_list::~piece_list() {
    std::cerr << "destructor of piece_list called" << std::endl;
    delete list[white];
    delete list[black];
}
