//
// Created by tobedetered on 1/27/24.
//

#ifndef PIECE_LIST_H
#define PIECE_LIST_H

#include <unordered_map>
#include <vector>

#include "../../CommonUtilities.h"

class piece_list {
public:
    piece_list(color color_, std::vector<piece_data>*);
    piece_list();
    ~piece_list();
    std::unordered_map<color, std::vector<piece_data>*> list;
};



#endif //PIECE_LIST_H
