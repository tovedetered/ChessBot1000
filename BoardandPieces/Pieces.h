//
// Created by tobedetered on 1/9/24.
//

#ifndef PIECES_H
#define PIECES_H

struct pieces {
    const int none = 0b000; //00 000
    const int king = 0b001; //00 001
    const int pawn = 0b010; //00 010
    const int knight = 0b011; //00 011
    const int bishop = 0b100; //00 100
    const int rook = 0b101; //00 101
    const int queen = 0b110; //00 110

    const int white = 0b01000; //01 000
    const int black = 0b10000; //10 000

    const int typeMask = 0b00111;
    const int colorMask = 0b11000;
    const int locMask = 0b11111100000;
    static bool isColorToMove(int piece, bool color) {
        if(piece == 0) return false;
        if(piece > 16 && !color) return true;
        if(piece < 16 && color) return true;
        return false;
    }
    static bool isPawn(const int piece) {
        if(piece == 10 || piece == 18) return true;
        return false;
    }
    static bool isKnight(const int piece) {
        if(piece == 11 || piece == 19) return true;
        return false;
    }
    static bool isBishop(const int piece) {
        if(piece == 12 || piece == 20) return true;
        return false;
    }
    static bool isRook(const int piece) {
        if(piece == 13 || piece == 21) return true;
        return false;
    }
    static bool isQueen(const int piece) {
        if(piece == 14 || piece == 22) return true;
        return false;
    }
    static bool isKing(const int piece) {
        if(piece == 9 || piece == 17) return true;
        return false;
    }
    static bool isSlidingPiece(const int piece) {
        if(isRook(piece) || isBishop(piece) || isQueen(piece)) return true;
        return false;
    }
};

#endif //PIECES_H
