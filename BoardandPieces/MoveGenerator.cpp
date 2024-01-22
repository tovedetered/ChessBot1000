//
// Created by tobedetered on 1/13/24.
//

#include "MoveGenerator.h"

#include <iostream>

MoveGenerator::MoveGenerator(Board* board) {
    this->board = board;
    position.clear();
    precomputeMoveData();
    initPieceOnEdge();
    slideAttackMap = 0;
    knightAttackMap = 0;
    kingAttackMap = 0;
    pawnAttackMap = 0;
}

std::vector<move> MoveGenerator::generateMoves() {
    init();
    //calculating the attack map to find checks
    calcAttackMap();
    generateKingMoves();
    //only king can move in a double check
    //(IMPORTANT): If I forgot to not include pins this will break
    if(position.inDoubleCheck) {
        return legalMoves;
    }
    generateSlidingMoves();
    return legalMoves;
}

void MoveGenerator::initPieceOnEdge() {
    for(int i = 0; i < 64; i++) {
        //if on left edge, ie file = 0
        const int file = i%8;
        const int rank = i/8;
        bool notOnFileSide = false;
        if(file < 1) {
            if(rank < 1) {
                pieceOnEdge.emplace(i, upperLeftCorner);
            }
            else if(rank > 6) {
                pieceOnEdge.emplace(i, lowerLeftCorner);
            }
            else {
                pieceOnEdge.emplace(i, leftEdge);
            }
        }
        else if(file > 6) {
            if(rank < 1) {
                pieceOnEdge.emplace(i, upperRightCorner);
            }
            else if(rank > 6) {
                pieceOnEdge.emplace(i, lowerRightCorner);
            }
            else {
                pieceOnEdge.emplace(i, rightEdge);
            }
        }
        else {
            if (rank < 1) {
                pieceOnEdge.emplace(i, upperEdge);
            }
            else if(rank > 6) {
                pieceOnEdge.emplace(i, lowerEdge);
            }
            else {
                pieceOnEdge.emplace(i, notOnEdge);
            }
        }
    }
}

void MoveGenerator::init() {
    position.clear();
    legalMoves.clear();
    position.inCheck = false;
    position.inDoubleCheck = false;
    position.pinsExistInPos = false;
    position.checkRayMask = 0;
    position.pinRayMask = 0;
}

void MoveGenerator::precomputeMoveData() {
    numSquareToEdge.reserve(64);
    for(int file = 0; file < 8; file ++) {
        for(int rank = 0; rank < 8; rank++) {
            int numNorth = rank;
            int numSouth = 7 - rank;
            int numEast = 7 - file;
            int numWest = file;

            const int squareIndex = access(file, rank);
            const std::vector<int> nums = {
                numNorth,
                numSouth,
                numEast,
                numWest,
                std::min(numEast, numNorth),
                std::min(numEast, numSouth),
                std::min(numWest, numSouth),
                std::min(numWest, numNorth),};
            numSquareToEdge[squareIndex] = nums;
        }
    }
}

int MoveGenerator::access(const int file, const int rank) {return file + 8 * rank;}

bool MoveGenerator::ableToMoveInDir(const int piece_, const bool diagonalMovement) const {
    const piece pieceType = board->getPieceType(piece_);
    if(diagonalMovement && (pieceType == bishop || pieceType == queen)) return true;
    if(!diagonalMovement && (pieceType == rook || pieceType == queen)) return true;
    return false;
}

void MoveGenerator::calcAttackMap() {
    calcSlideAttackMap();
    int startDir = 0;
    int endDir = 8;
    //need to check the king things
    if(board->getPiceColorList(queen, position.opponentcolor).empty()) {
        startDir = board->getPiceColorList(rook, position.opponentcolor).empty()? 0 : 4;
        endDir = board->getPiceColorList(bishop, position.opponentcolor).empty()? 4 : 8;
    }
    //for every direction check to see if a piece that could be attacking is
    for(int i = startDir; i < endDir; i++) {
        bool diag = i > 3;

        const int numSquare = numSquareToEdge[position.activeKingIndex][i];
        bool friendInDir = false;
        unsigned long long int rayMask = 0;
        //check sliding pieces
        for(int j = 0; j < numSquare; j++) {
            const int targetSquare = position.activeKingIndex + directionOffset[i] * (j+1);
            const int targetPiece = board->getPieceAtSquare(targetSquare);
            rayMask |= 1ul << targetSquare;
            if(targetPiece != none) {
                //see if freinds are blocking/are pinned
                if(board->isColor(targetPiece, position.friendlyColor)) {
                    if(!friendInDir) {
                        friendInDir = true;
                    }
                    else {
                        break;
                    }
                }
                else {
                    if(ableToMoveInDir(targetPiece, diag)) {
                        if(friendInDir) {
                            position.pinsExistInPos = true;
                            //OR ing the pin mask to update it with current ray
                            position.pinRayMask |= rayMask;
                        }
                        else {
                            position.checkRayMask |= rayMask;
                            position.inDoubleCheck = position.inCheck;
                            position.inCheck = true;
                        }
                        break;
                    }
                    //pice blocking any pins or attacks
                    break;
                }
            }
        }
        // ReSharper disable once CppDFAConstantConditions (it is not always false, I think)
        if(position.inDoubleCheck) break;
    }
    // need to find all of the knights
    //then find where they attack and add them
    knightAttackMap = 0;
    const std::vector<piece_data> opponentKnights = board->getPiceColorList(knight, position.opponentcolor);
    if(!opponentKnights.empty()) {
        bool knightCheck = false;
        for (const auto [piece, index] : opponentKnights) {
            generateKnightAttack(index);
            if(!knightCheck && checkValueAtPos(knightAttackMap, position.activeKingIndex)) {
                knightCheck = true;
                position.inDoubleCheck = position.inCheck;
                position.inCheck = true;
                position.checkRayMask |= static_cast<uint64_t>(1) << index;
            }
        }
    }
    //pawns
    const std::vector<piece_data> opponentPawns = board->getPiceColorList(pawn, position.opponentcolor);
    pawnAttackMap = 0;
    if(!opponentPawns.empty()) {
        bool pawnCheck = false;
        for (const auto [piece, index] : opponentPawns) {
            generatePawnAttack(index, position.opponentcolor);
            if(!pawnCheck) {
                pawnCheck = true;
                position.inDoubleCheck = position.inCheck;
                position.inCheck = true;
                position.checkRayMask |= static_cast<uint64_t>(1) << index;
            }
        }
    }
    //finally opponent's king
    //It CANNOT check the other king so we do not have to check it
    generateKingAttack(position.opponentKingIndex);
    //Then just combine them all
    attackMap |= kingAttackMap | slideAttackMap | knightAttackMap | pawnAttackMap;
}

void MoveGenerator::calcSlideAttackMap() {

    //loop through all of the sliding pieces and update the map
    const std::vector<piece_data> opponentRooks = board->getPiceColorList(rook, position.opponentcolor);
    for(piece_data rook : opponentRooks) {
        updateAttackMap(rook.index, 0, 4);
    }
    const std::vector<piece_data> opponentBishops = board->getPiceColorList(bishop, position.opponentcolor);
    for(piece_data bishop : opponentBishops) {
        updateAttackMap(bishop.index, 4, 8);
    }
    const std::vector<piece_data> opponentQueens = board->getPiceColorList(queen, position.opponentcolor);
    for(piece_data queen : opponentQueens) {
        updateAttackMap(queen.index, 0, 8);
    }
}

void MoveGenerator::generateSlidingMoves() {
    const std::vector<piece_data> opponentRooks = board->getPiceColorList(rook, position.opponentcolor);
    for(piece_data rook : opponentRooks) {
        generateSlideMove(rook.index, 0, 4);
    }
}

void MoveGenerator::generateSlideMove(int activePos, int startDirIndex, int endDirIndex) {

}

void MoveGenerator::updateAttackMap(int startSquare, int dirStart, int dirEnd) {
    for(int dirIndex = dirStart; dirIndex < dirEnd; dirIndex++) {
        const int dirOff = directionOffset[dirIndex];
        for(int n = 0; n < numSquareToEdge[startSquare][dirIndex]; n++) {
            const uint64_t targetSquare = startSquare + dirOff* (n+1);
            const int pieceAtTarget = board->getPieceAtSquare(targetSquare);
            slideAttackMap |= static_cast<uint64_t>(1) << targetSquare;
            if(targetSquare != board->getKingPos(position.friendlyColor) &&
                pieceAtTarget != 0) {
                break;
            }
        }
    }
}

std::vector<move> MoveGenerator::generateKnightMove(const int activePos) const {
    std::vector<move> moves;
    std::vector<int> targetMoves;
    const color knightColor = board->getPieceColor(board->getPieceAtSquare(activePos));
    //up 2 over 1
    //2N 1E
    if(activePos%8 <7 && activePos/8 < 6) //check to see if at edge of board so we don't warp / segfault
        targetMoves.push_back(activePos + 2 * directionOffset[0] + directionOffset[1]);
    //2E 1N
    if(activePos%8 < 6 && activePos/8 < 7)
        targetMoves.push_back(activePos + 2*directionOffset[1] + directionOffset[0]);
    //2E 1S
    if(activePos%8 < 6 && activePos/8 > 0)
        targetMoves.push_back(activePos + 2*directionOffset[1] + directionOffset[2]);
    //2S 1E
    if(activePos%8 < 7 && activePos/8 > 1)
        targetMoves.push_back(activePos + 2*directionOffset[2] + directionOffset[1]);
    // 2S 1W
    if(activePos%8 > 0 && activePos/8 > 1)
        targetMoves.push_back(activePos + 2 * directionOffset[2] - directionOffset[1]);
    // 2W 1S
    if(activePos%8 > 1 && activePos/8 > 0)
        targetMoves.push_back(activePos + 2 * directionOffset[3] - directionOffset[2]);
    // 2W 1N
    if(activePos%8 > 1 && activePos/8 < 7)
        targetMoves.push_back(activePos + 2 * directionOffset[3] + directionOffset[1]);
    // 2N 1W
    if(activePos%8 > 0 && activePos/8 < 6)
        targetMoves.push_back(activePos + 2 * directionOffset[0] - directionOffset[1]);

    //check the moves for friendly pices
   for(const int move : targetMoves) {
        if(!board->isColor(move, knightColor)) {
            moves.push_back({activePos, move});
        }
   }
    return moves;
}

void MoveGenerator::generateKnightAttack(const int activePos) {
    std::vector<int> targetMoves;
    //up 2 over 1
    //2N 1E
    if(activePos%8 <7 && activePos/8 < 6) //check to see if at edge of board so we don't warp / segfault
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[0] + directionOffset[1];
    //2E 1N
    if(activePos%8 < 6 && activePos/8 < 7)
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2*directionOffset[1] + directionOffset[0];
    //2E 1S
    if (activePos % 8 < 6 && activePos / 8 > 0) {
        // 2E 1S
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[1] + directionOffset[2];
    }
    if (activePos % 8 < 7 && activePos / 8 > 1) {
        // 2S 1E
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[2] + directionOffset[1];
    }
    if (activePos % 8 > 0 && activePos / 8 > 1) {
        // 2S 1W
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[2] - directionOffset[1];
    }
    if (activePos % 8 > 1 && activePos / 8 > 0) {
        // 2W 1S
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[3] - directionOffset[2];
    }
    if (activePos % 8 > 1 && activePos / 8 < 7) {
        // 2W 1N
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[3] + directionOffset[1];
    }
    if (activePos % 8 > 0 && activePos / 8 < 6) {
        // 2N 1W
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[0] - directionOffset[1];
    }
}

void MoveGenerator::generatePawnAttack(const int activePos, const color activeColor) {
    const int dirMultiplier = activeColor == white ? 1:-1;
    //need to make sure that they are not at edges of board
    if(pieceOnEdge[activePos] != (leftEdge | upperLeftCorner | lowerLeftCorner)) {
        const int forwardLeft = directionOffset[3] + directionOffset[0] * dirMultiplier;
        const int targetSquare = forwardLeft + activePos;
        bitToOneAtPos(&pawnAttackMap, targetSquare);
    }
    if(pieceOnEdge[activePos] != (rightEdge | upperRightCorner | lowerRightCorner)) {
        const int forwardRight = directionOffset[1] + directionOffset[0] * dirMultiplier;
        const int targetSquare = forwardRight + activePos;
        bitToOneAtPos(&pawnAttackMap, targetSquare);
    }
}

void MoveGenerator::generateKingAttack(const int activePos) {
    for(int i = 0; i < 8; i++) {
        if(numSquareToEdge[activePos][i] !=0) {
            bitToOneAtPos(&kingAttackMap, directionOffset[1] + activePos);
        }
    }
}

void MoveGenerator::generateKingMoves() {
    std::vector<int> targetSquares;
    for(int i = 0; i < 8; i++) {
        if(numSquareToEdge[position.activeKingIndex][i] !=0) {
            if(const int index = directionOffset[1] + position.activeKingIndex;
                board->getPieceColor(board->getPieceAtSquare(index)) != position.friendlyColor) {
                targetSquares.push_back(index);
            }
        }
    }
    //Now check to see if they are legal
    for(int target : targetSquares) {
        bool capture = board->isColor(board->getPieceAtSquare(target), position.opponentcolor);
        if(!capture) {
            if(checkValueAtPos(position.checkRayMask, target)) {
                continue;
            }
        }
        //if the square is not attacked
        if(!checkValueAtPos(attackMap, target)) {
            legalMoves.push_back({position.activeKingIndex, target});
        }
    }
    //Now Castleing
    if(!position.inCheck) {
        color fColor = position.friendlyColor;
        bool kingside = fColor == white ? position.castleAbility[0] : position.castleAbility[2];
        bool queenside = fColor == white ? position.castleAbility[1] : position.castleAbility[3];
        if(kingside) {
            int targetIndex = position.activeKingIndex + 2; //right two
            int passThroughIndex = position.activeKingIndex + 1; //need to check both
            if(!checkValueAtPos(attackMap, targetIndex) &&
                !checkValueAtPos(attackMap, passThroughIndex)) {
                if(board->getPieceAtSquare(targetIndex) == 0 &&
                    board->getPieceAtSquare(passThroughIndex) == 0) {
                    legalMoves.push_back({position.activeKingIndex,
                        fColor == white ? 70 : 90});
                }
            }
        }
        if(queenside) {
            int targetIndex = position.activeKingIndex - 2; //right two
            int passThroughIndex = position.activeKingIndex - 1; //need to check both
            if(!checkValueAtPos(attackMap, targetIndex) &&
                !checkValueAtPos(attackMap, passThroughIndex)) {
                if(board->getPieceAtSquare(targetIndex) == 0 &&
                    board->getPieceAtSquare(passThroughIndex) == 0) {
                    legalMoves.push_back({position.activeKingIndex,
                        fColor == white ? 80 : 100});
                    }
                }
        }
    }
}
