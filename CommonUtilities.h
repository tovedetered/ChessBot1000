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
    normal = 0,
    whiteKingSide = 1,
    whiteQueenSide = 2,
    blackKingSide = 3,
    blackQueenSide = 4,
    enPassantCapture = 5,
    twoSquarePawnMove = 6,
    promoteToQueen = 7,
    promoteToRook = 8,
    promoteToBishop = 9,
    promoteToKnight = 10,
};

struct move {
    int startIndex = -1;
    int endIndex = -1;
    moveFlag flag = normal;
};

enum color {
    white = 0b01000,
    black = 0b10000,
    noMove = -1
};
struct castleStats {
    int rookStartIndex;
    int rookEndIndex;
    int rookType;

    void clear() {
        rookStartIndex = -1;
        rookEndIndex = -1;
        rookType = -1;
    }
};


struct gameState {
    color colorMove;
    //0 none, 0b0001 white kingside,0010 white queenside, 0100 black Kingside,1000 black queenside
    int castleAbility;
    castleStats castlePlayed;
    //-1 = no en Passant
    int enPassantIndex;
    int fiftyPlyCount;
    int totalPly;
    int halfMoves;
    move mostRecentMove;
    int pieceCaptureInMove;
    int mostRecentPromoPiece;

    void clear() {
        colorMove = noMove;
        enPassantIndex = -1;
    }

    void switchColor() {
        if(colorMove == white) {
            colorMove = black;
        }
        else if(colorMove == black){
            colorMove = white;
        }
    }
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

struct piecesDataThing {
    int none = 0b000;
    int rook = 0b101;
    int queen = 0b110;
    int pawn = 0b010;
    int bishop = 0b100;
    int knight = 0b011;
    int king = 0b001;
    int white = 0b01000;
    int black = 0b10000;
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

inline int access(const int file, const int rank) {return file + 8 * rank;}

struct fileRank {
    int file;
    int rank;
};
#endif //COMMONUTILITIES_H
