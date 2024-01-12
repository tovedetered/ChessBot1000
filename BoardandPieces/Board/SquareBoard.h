//
// Created by tobedetered on 1/9/24.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include "../Pieces.h"
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>

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

struct moveStats {
    fileRank start;
    fileRank end;
};

class SquareBoard {
public:
    SquareBoard();
    void readFromFen(std::string fenString);
    static int access(int file, int rank); //x,y
    static fileRank reverseAccess(int index);
    [[nodiscard]] int getPieceAtValue(int index) const;

    void movePiece(fileRank start, fileRank end);
    bool isLegalMove(fileRank start, fileRank end);

    void generateMoves();
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

    std::stack<moveStats> mostRecentMoves;
    moveStats lastMove;

    std::vector<std::vector<int>> numSquareToEdge;
    const int directionOffset[8] = {-8, 1, 8, -1, -7, 9, 7, -9}; //N,E,S,W,NE,SE,SW,NW
    void precomputeMoveData();
    std::vector<moveStats> legalMoves;

    std::vector<moveStats> generatePawnMoves(int activePos) const;
    std::vector<moveStats> generateKnightMoves(int activePos) const;
    std::vector<moveStats> generateSlidingMoves(int pos) const;
};



#endif //BITBOARD_H
