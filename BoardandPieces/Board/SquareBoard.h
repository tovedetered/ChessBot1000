//
// Created by tobedetered on 1/9/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include "../Pieces.h"
#include <string>
#include <unordered_map>

struct gameStats {
    bool whiteMove; //1 = white to move //0 is black to move
    int castleAbility; //0b0001 black queenside, 0b0010 black kingside, 0b0100 white queenside,0b 1000 white kingside
    int enPessantTarget; //form of board id for ease of use
    int halfMvoes;
    int fullMoves;
};

struct fileRank {
    int file;
    int rank;
};


class SquareBoard {
public:
    SquareBoard();
    void readFromFen(std::string fenString);
    static int access(int file, int rank); //x,y
    [[nodiscard]] int getPieceAtValue(int index) const;

    void movePiece(fileRank start, fileRank end);
private:
    int board[64];
    pieces pieceTable;

    void initFenToID();
    std::unordered_map<char, int> fenToID;
    void initCharMap();
    std::unordered_map<char, int> charRankMap;
    void initRankMap();
    std::unordered_map<int, int> realToFakeRank;
    gameStats gameDetails;


};



#endif //BITBOARD_H
