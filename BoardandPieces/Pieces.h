//
// Created by tobedetered on 1/9/24.
//

#ifndef PIECES_H
#define PIECES_H

struct pieces {
    const int none = 0; //00 000
    const int king = 1; //00 001
    const int pawn = 2; //00 010
    const int knight = 3; //00 011
    const int bishop = 4; //00 100
    const int rook = 5; //00 101
    const int queen = 6; //00 110

    const int white = 8; //01 000
    const int black = 16; //10 000
};

#endif //PIECES_H
