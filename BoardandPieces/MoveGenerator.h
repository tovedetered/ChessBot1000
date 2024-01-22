//
// Created by tobedetered on 1/13/24.
//

#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H
#include <vector>
#include "Pieces.h"
#include "Board/Board.h"
#include "../CommonUtilities.h"

struct positionDetails {
    bool inCheck;
    bool inDoubleCheck;
    bool pinsExistInPos;
    uint64_t checkRayMask;
    uint64_t pinRayMask;

    bool isWhiteMove;
    color friendlyColor;
    color opponentcolor;
    int activeKingIndex;
    int opponentKingIndex;
    //0 wks, 1 wqs, 2 bks, 3 bqs
    bool castleAbility[4];
    int enPassantIndex;

    void clear() {
        inCheck = false;
        inDoubleCheck = false;
        pinsExistInPos = false;
        checkRayMask = 0;
        pinRayMask = 0;
        isWhiteMove = false;
        friendlyColor = static_cast<color>(0);
        opponentcolor = static_cast<color>(0);
        activeKingIndex = 0;
        opponentKingIndex = 0;
        enPassantIndex = -1;
        std::fill(std::begin(castleAbility), std::end(castleAbility), false);
    }
};

enum edgeData {
    notOnEdge = 0,
    leftEdge = 1,
    rightEdge = 2,
    upperEdge = 3,
    lowerEdge = 4,
    upperRightCorner = 5,
    upperLeftCorner = 6,
    lowerRightCorner = 7,
    lowerLeftCorner = 8
};
class MoveGenerator {
public:
    MoveGenerator(Board* board);
    std::vector<move> generateMoves();
private:
    pieces pieceTable;
    std::vector<piece_data> activePieces;
    std::vector<move> legalMoves;
    positionDetails position{};
    uint64_t attackMap{};
    uint64_t slideAttackMap;
    uint64_t knightAttackMap;
    uint64_t pawnAttackMap{};
    uint64_t kingAttackMap;
    std::unordered_map<int, edgeData> pieceOnEdge;
    std::unordered_map<int, edgeData> pieceOnTopBot;
    void initPieceOnEdge();

    void init();

    Board* board;

    void calcAttackMap();
    void calcSlideAttackMap();
    void generateSlidingMoves();
    void generateSlideMove(int activePos, int startDirIndex, int endDirIndex);
    void updateAttackMap(int startSquare, int dirStart, int dirEnd);

    std::vector<move> generateKnightMove(int activePos) const;
    void generateKnightAttack(int activePos);

    void generatePawnAttack(int activePos, color activeColor);
    void generatePawnMoves();
    void generatePromoionMoves(int targetSquare, int startSquare);

    void generateKingAttack(int activePos);
    void generateKingMoves();

    const int directionOffset[8] = {-8, 1, 8, -1, -7, 9, 7, -9}; //N,E,S,W,NE,SE,SW,NW
    void precomputeMoveData();
    std::vector<std::vector<int>> numSquareToEdge;
    static int access(int file, int rank); //x,y
    bool ableToMoveInDir(int piece_, bool diagonalMovement) const;
    bool IsMoveAlongRay(int dir, int start, int target) const;
};



#endif //MOVEGENERATOR_H
