//
// Created by tobedetered on 1/14/24.
//

#ifndef COMMONUTILITIES_H
#define COMMONUTILITIES_H

#include <cstdint>

struct piece_data {
    int piece;
    int index;
};
//end Index 70 = white Kingside
//80 = white Queenside
//90 = black Kingside
//100 = black Queenside
enum moveFlag {
    normal,
    whiteKingSide,
    whiteQueenSide,
    blackKingSide,
    blackQueenSide,
    enPassantCapture,
    twoSquarePawnMove,
    promoteToQueen,
    promoteToRook,
    promoteToBishop,
    promoteToKnight,
};

struct move {
    int startIndex = -1;
    int endIndex = -1;
    moveFlag flag = normal;
};

struct gameState {
    bool colorMove;
    //0 none, 0b0001 white kingside,0010 white queenside, 0100 black Kingside,1000 black queenside
    int castleAbility;
    //-1 = no en Passant
    int enPassantIndex;
    int fiftyPlyCount;
    int totalPly;
    int mostRecentMove[2];
};

enum color {
    white = 0b01000,
    black = 0b10000
};

enum piece {
    none = 0b000,
    rook = 0b101,
    queen = 0b110,
    pawn = 0b010,
    bishop = 0b100,
    knight = 0b011,
    king = 0b001,
};

inline bool checkValueAtPos(const uint64_t num, const uint64_t n) {
    return static_cast<bool>((num >> n) & static_cast<uint64_t>(1));
}

inline void flipBitAtPos(uint64_t* num, const uint64_t n) {
    *num ^= static_cast<uint64_t>(1) << n;
}

inline void bitToOneAtPos(uint64_t* num, const uint64_t n) {
    *num |= static_cast<uint64_t>(1) << n;
}
#endif //COMMONUTILITIES_H
