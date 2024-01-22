//
// Created by tobedetered on 1/9/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include "../Pieces.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <vector>

struct gameStats {
    bool whiteMove; //1 = white to move //0 is black to move
    int castleAbility; //0b0001 black queenside, 0b0010 black kingside, 0b0100 white queenside,0b 1000 white kingside
    int enPessantTarget; //form of board id for ease of use
    int halfMvoes;
    int fullMoves;
};

enum color {
    white = 1,
    black = 0
};

struct fileRank {
    int file;
    int rank;
};

struct move_stats {
    fileRank start;
    fileRank end;
};

struct dual_move_stats {
    std::vector<move_stats> moves;
    std::unordered_set<int> squaresAttacked;
};

class SquareBoard {
public:
    SquareBoard();
    void readFromFen(const std::string& fenString);
    static int access(int file, int rank); //x,y
    static fileRank reverseAccess(int index);
    [[nodiscard]] int getPieceAtValue(int index) const;

    void movePiece(fileRank start, fileRank end);
    bool isLegalMove(fileRank start, fileRank end);
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

    std::stack<move_stats> mostRecentMoves;
    move_stats lastMove;

    std::vector<std::vector<int>> numSquareToEdge;
    const int directionOffset[8] = {-8, 1, 8, -1, -7, 9, 7, -9}; //N,E,S,W,NE,SE,SW,NW
    void precomputeMoveData();
    std::vector<move_stats> legalMoves;
    //mutable here is evil (I removed it)
    std::unordered_set<int> squaresAttacked;
    fileRank kingsPos[2];
};



#endif //BITBOARD_H
