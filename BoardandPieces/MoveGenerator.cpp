//
// Created by tobedetered on 1/13/24.
//



//Heaily Inspired by / C++ port of Sebastian Lauge's V1 ChessBot
//https://github.com/SebLague/Chess-Coding-Adventure/tree/Chess-V1-Unity
/*MIT License

Copyright (c) 2021 Sebastian Lague

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "MoveGenerator.h"

#include <iostream>

#include "Renderers/GraphicalBoard.h"

MoveGenerator::MoveGenerator(Board* board) {
    this->board = board;
    position.clear();
    precomputeMoveData();
    initPieceOnEdge();
    slideAttackMap = 0;
    knightAttackMap = 0;
    kingAttackMap = 0;
    pawnAttackMap = 0;
    pawnsPinned = 0;
}

std::vector<move> MoveGenerator::generateMoves() {
    init();
    updatePostiton();
    //calculating the attack map to find checks
    calcAttackMap(); //Working?
    generateKingMoves(); //Maybe Broke
    //only king can move in a double check

    if(position.inDoubleCheck) {
        return legalMoves;
    }
    if(position.inCheck) {
        board->debug.checks += 1;
    }
    generateSlidingMoves();
    generatePawnMoves();
    generateKnightMoves();
    return legalMoves;
}

void MoveGenerator::initPieceOnEdge() {
    for(int i = 0; i < 64; i++) {
        //if on left edge, ie file = 0
        const int file = i%8;
        const int rank = i/8;
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
    kingAttackMap = 0;
    pawnsPinned = 0;
    pawnAttackMap = 0;
    slideAttackMap = 0;
    attackMap = 0;
    knightAttackMap = 0;
    kingAttackMap = 0;
    game = board->getCurrentGameState();
}

void MoveGenerator::updatePostiton() {
    //need to update the game board somehow
    position.friendlyColor = game.colorMove;
    position.opponentcolor = position.friendlyColor == white? black:white;
    position.enPassantIndex = game.enPassantIndex;
    position.activeKingIndex = board->getKingPos(position.friendlyColor);
    position.opponentKingIndex = board->getKingPos(position.opponentcolor);
    position.castleAbility[0] = checkValueAtPos(game.castleAbility, 0);
    position.castleAbility[1] = checkValueAtPos(game.castleAbility, 1);
    position.castleAbility[2] = checkValueAtPos(game.castleAbility, 2);
    position.castleAbility[3] = checkValueAtPos(game.castleAbility, 3);
}

void MoveGenerator::precomputeMoveData() {
    numSquareToEdge.resize(64);
    for(int file = 0; file < 8; file ++) {
        for(int rank = 0; rank < 8; rank++) {
            int numNorth = rank;
            int numSouth = 7 - rank;
            int numEast = 7 - file;
            int numWest = file;

            const int squareIndex = access(file, rank);
            if(squareIndex == 60) {
                std::cout << rank << " " << file << " " << squareIndex;
            }
            const std::vector nums = {
                numNorth,
                numEast,
                numSouth,
                numWest,
                std::min(numEast, numNorth),
                std::min(numEast, numSouth),
                std::min(numWest, numSouth),
                std::min(numWest, numNorth),};
            numSquareToEdge.at(squareIndex) = nums;
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
// This is terrible
bool MoveGenerator::IsMoveAlongRay(int dir, int start, const int target) const {
    //stupid way
    //always down to the left
    int startOfRay;
    int numOfSquareInRay;
    //up or down
    if(dir == 8 || dir == -8) {
        const int numToEdge = numSquareToEdge[start][2];
        startOfRay = start + numToEdge * directionOffset[2];
        numOfSquareInRay = numSquareToEdge[startOfRay][0];
    }
    else if(dir == -1 || dir == 1) {
        //N,E,S,W,NE,SE,SW,NW
        int numToEdge = numSquareToEdge[start][3];
        startOfRay = start + numToEdge * directionOffset[3];
        numOfSquareInRay = numSquareToEdge[startOfRay][1];
    }
    else if (dir == -7 || dir == 7) {
        int numToEdge = numSquareToEdge[start][4];
        startOfRay = start + numToEdge * (-7);
        numOfSquareInRay = numSquareToEdge[startOfRay][6];
    }
    else {
        int numToEdge = numSquareToEdge[start][5];
        startOfRay = start + numToEdge * (9);
        numOfSquareInRay = numSquareToEdge[startOfRay][7];
    }
    for(int i = 0; i < numOfSquareInRay; i++) {
        if(const int targetSquare = startOfRay +  dir * (i); targetSquare == target) {
            return true;
        }
    }
    return false;


    //need to find the direction
    //int moveDirection = 0;
    //then check it
    //return (dir == moveDirection || -dir == moveDirection);
}

void MoveGenerator::calcAttackMap() {
    calcSlideAttackMap();
    int startDir = 0;
    int endDir = 8;
    //need to check the king things
    if(board->getPiceColorList(queen, position.opponentcolor).empty()) {
        startDir = board->getPiceColorList(rook, position.opponentcolor).empty()? 4 : 0;
        endDir = board->getPiceColorList(bishop, position.opponentcolor).empty()? 4 : 8;
    }
    //for every direction check to see if a piece that could be attacking is
    for(int i = startDir; i < endDir; i++) {
        const bool diag = i > 3;
        const int numSquare = numSquareToEdge[position.activeKingIndex][i];
        if(numSquare == 0) {
            continue;
        }
        bool friendInDir = false;
        bool enPassFlag = false;
        bool freindPawnInDir = false;
        bool alreadyPawnInDir = false;
        unsigned long long int rayMask = 0;
        //check sliding pieces
        for(int j = 0; j < numSquare; j++) {
            const int targetSquare = position.activeKingIndex + directionOffset[i] * (j+1);
            const int targetPiece = board->getPieceAtSquare(targetSquare);
            rayMask |= 1ul << targetSquare;
            if(board->getPieceType(targetPiece) == pawn &&
                board->getPieceColor(targetPiece) == position.opponentcolor) {
                //see if enpassant square is adjacent to it
                if(const int backwardsDir = board->getPieceColor(targetPiece) == white? 8:-8;
                    targetSquare + backwardsDir == position.enPassantIndex) {
                    //if it is then check to see if it can be captured by a pawn, and if so raise a flag
                    //It cannot be the freindly color being en-passant'ed in this case
                    //we already know that it will not check the king by moving, only matters if the piece gets taken
                    //and it IS blocking the pin
                    //we just cannot take it
                    //Here we need to raise a look and continue
                    int enTargetEast = -1;
                    int enTargetWest = -1;
                    if(numSquareToEdge[targetSquare][1] !=0) {
                        enTargetEast = board->getPieceAtSquare(targetSquare + 1);
                    }
                    if(numSquareToEdge[targetSquare][3] != 0) {
                        enTargetWest = board->getPieceAtSquare(targetSquare - 1);
                    }
                    if(!alreadyPawnInDir) {
                        alreadyPawnInDir = true;
                        if(enTargetEast != - 1) {
                            if(board->getPieceType(enTargetEast) == pawn &&
                            board->getPieceColor(enTargetEast) == position.friendlyColor) {
                                enPassFlag = true;
                            }
                        }
                        if(enTargetWest != -1) {
                            if(board->getPieceType(enTargetWest) == pawn &&
                                board->getPieceColor(enTargetEast) == position.friendlyColor) {
                                enPassFlag = true;
                                }
                        }
                        continue;
                    }
                    break;
                }
            }
            if(targetPiece != none) {
                //see if freinds are blocking/are pinned
                if(board->isColor(targetPiece, position.friendlyColor)) {
                    if(!friendInDir) {
                        if(board->getPieceType(targetPiece) == pawn) freindPawnInDir = true;
                        friendInDir = true;
                    }
                    else {
                        break;
                    }
                }
                //opponent piece there
                else {
                    if(ableToMoveInDir(targetPiece, diag)) {

                        if((friendInDir && !freindPawnInDir) ||
                            (friendInDir && (freindPawnInDir && (i != 1 && i != 3)))) {
                            position.pinsExistInPos = true;
                            //OR ing the pin mask to update it with current ray
                            position.pinRayMask |= rayMask;
                        }
                        else if(friendInDir && freindPawnInDir && (i == 1 || i == 3)) {
                            if(enPassFlag) {
                                position.enPassImposible = true;
                                break;
                            }
                            position.pinsExistInPos = true;
                            //OR ing the pin mask to update it with current ray
                            position.pinRayMask |= rayMask;
                        }
                        else {
                            //here enPass is impossible,
                            if(enPassFlag) {
                                //We are not pinned, but we cant take en pass
                                if(freindPawnInDir && (i == 1 || i == 3)) {
                                    position.enPassImposible = true;
                                    break;
                                }
                            }
                            position.checkRayMask |= rayMask;
                            position.inDoubleCheck = position.inCheck;
                            position.inCheck = true;
                            board->debug.checks += 1;
                        }
                        break;
                    }
                    //pice blocking any pins or attacks
                    //but if it is a pawn we need to continue to look to stop any funny buisness with
                    //en-passant
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
            if(!pawnCheck && checkValueAtPos(pawnAttackMap, position.activeKingIndex)) {
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
    for(const piece_data rook : opponentRooks) {
        updateAttackMap(rook.index, 0, 4);
    }
    const std::vector<piece_data> opponentBishops = board->getPiceColorList(bishop, position.opponentcolor);
    for(const piece_data bishop : opponentBishops) {
        updateAttackMap(bishop.index, 4, 8);
    }
    const std::vector<piece_data> opponentQueens = board->getPiceColorList(queen, position.opponentcolor);
    for(const piece_data queen : opponentQueens) {
        updateAttackMap(queen.index, 0, 8);
    }
}

void MoveGenerator::generateSlidingMoves() {
    const std::vector<piece_data> friendlyRooks =
        board->getPiceColorList(rook, position.friendlyColor);
    for(const piece_data rook : friendlyRooks) {
        generateSlideMove(rook.index, 0, 4);
    }

    const std::vector<piece_data> friendlyBishops =
        board->getPiceColorList(bishop, position.friendlyColor);
    for(const piece_data bishop : friendlyBishops) {
        generateSlideMove(bishop.index, 4, 8);
    }

    const std::vector<piece_data> freindlyQueens =
        board->getPiceColorList(queen, position.friendlyColor);
    for(const auto queen : freindlyQueens) {
        generateSlideMove(queen.index, 0,8);
    }
}

void MoveGenerator::generateSlideMove(int activePos, int startDirIndex, int endDirIndex) {
    const bool pinned = checkValueAtPos(position.pinRayMask, activePos);
    if(pinned && position.inCheck) {
        return;
    }

    for(int i = startDirIndex; i < endDirIndex; i++) {
        const int currentDirOffset = directionOffset[i];
        if(pinned && !IsMoveAlongRay(currentDirOffset, position.activeKingIndex, activePos)) {
            continue;
        }
        for(int n = 0; n < numSquareToEdge[activePos][i]; n++) {
            const int targetSquare = activePos + currentDirOffset * (n+1);
            const int targetPiece = board->getPieceAtSquare(targetSquare);

            if(board->isColor(targetPiece, position.friendlyColor)) break;
            const bool capture = targetPiece != 0;
            const bool stopCheck = position.inCheck? checkValueAtPos(position.checkRayMask, targetSquare):false;
            if(stopCheck || !position.inCheck) {
                legalMoves.push_back({activePos, targetSquare});
            }
            if(capture || stopCheck) {
                break;
            }
        }
    }
}

void MoveGenerator::updateAttackMap(int startSquare, int dirStart, int dirEnd) {
    for(int dirIndex = dirStart; dirIndex < dirEnd; dirIndex++) {
        const int dirOff = directionOffset[dirIndex];
        for(int n = 0; n < numSquareToEdge[startSquare][dirIndex]; n++) {
            const uint64_t targetSquare = startSquare + dirOff* (n+1);
            const int pieceAtTarget = board->getPieceAtSquare(static_cast<int>(targetSquare));
            slideAttackMap |= static_cast<uint64_t>(1) << targetSquare;
            if(targetSquare != board->getKingPos(position.friendlyColor) &&
                pieceAtTarget != 0) {
                break;
            }
        }
    }
}

void MoveGenerator::generateKnightMoves() {
    const std::vector<piece_data> knightList = board->getPiceColorList(knight, position.friendlyColor);
    for(const auto [piece, index] : knightList) {
        if(std::vector<move> knightMove = generateKnightMove(index); !knightMove.empty()) {
            legalMoves.insert(legalMoves.end(), knightMove.begin(), knightMove.end());
        }
    }
}

std::vector<move> MoveGenerator::generateKnightMove(const int activePos) const {
    std::vector<move> moves;
    std::vector<int> targetMoves;
    const color knightColor = board->getPieceColor(board->getPieceAtSquare(activePos));
    //up 2 over 1
    //2N 1E
    if(activePos%8 <7 && activePos/8 > 1) //check to see if at edge of board so we don't warp / segfault
        targetMoves.push_back(activePos + 2 * directionOffset[0] + directionOffset[1]);
    //2E 1N
    if(activePos%8 < 6 && activePos/8 > 0)
        targetMoves.push_back(activePos + 2*directionOffset[1] + directionOffset[0]);
    //2E 1S
    if(activePos%8 < 6 && activePos/8 < 7)
        targetMoves.push_back(activePos + 2*directionOffset[1] + directionOffset[2]);
    //2S 1E
    if(activePos%8 < 7 && activePos/8 < 6)
        targetMoves.push_back(activePos + 2*directionOffset[2] + directionOffset[1]);
    // 2S 1W
    if(activePos%8 > 0 && activePos/8 < 6)
        targetMoves.push_back(activePos + 2 * directionOffset[2] + directionOffset[3]);
    // 2W 1S
    if(activePos%8 > 1 && activePos/8 < 7)
        targetMoves.push_back(activePos + 2 * directionOffset[3] + directionOffset[2]);
    // 2W 1N
    if(activePos%8 > 1 && activePos/8 > 0)
        targetMoves.push_back(activePos + 2 * directionOffset[3] + directionOffset[0]);
    // 2N 1W
    if(activePos%8 > 0 && activePos/8 > 1)
        targetMoves.push_back(activePos + 2 * directionOffset[0] + directionOffset[3]);

    //check the moves for friendly pices
   for(const int move : targetMoves) {
       if(checkValueAtPos(position.pinRayMask, activePos)) {
           break;
       }
       if(board->isColor(board->getPieceAtSquare(move), knightColor)) {
           continue;
       }
       if(!position.inCheck) {
           moves.push_back({activePos, move});
       }
       else { //not pinned and in check and the target is empty or opponent
           if(checkValueAtPos(position.checkRayMask, move)) {
               moves.push_back({activePos, move});
           }
       }
   }
    return moves;
}

void MoveGenerator::generateKnightAttack(const int activePos) {
    std::vector<int> targetMoves;
    //up 2 over 1
    //2N 1E
    if(activePos%8 <7 && activePos/8 > 1) //check to see if at edge of board so we don't warp / segfault
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[0] + directionOffset[1];
    //2E 1N
    if(activePos%8 < 6 && activePos/8 > 0)
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2*directionOffset[1] + directionOffset[0];
    //2E 1S
    if (activePos % 8 < 6 && activePos / 8 < 7) {
        // 2E 1S
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[1] + directionOffset[2];
    }
    if (activePos % 8 < 7 && activePos / 8 < 6) {
        // 2S 1E
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[2] + directionOffset[1];
    }
    if (activePos % 8 > 0 && activePos / 8 < 6) {
        // 2S 1W
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[2] + directionOffset[3];
    }
    if (activePos % 8 > 1 && activePos / 8 < 7) {
        // 2W 1S
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[3] + directionOffset[2];
    }
    if (activePos % 8 > 1 && activePos / 8 > 0) {
        // 2W 1N
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[3] + directionOffset[0];
    }
    if (activePos % 8 > 0 && activePos / 8 > 1) {
        // 2N 1W
        knightAttackMap |= static_cast<uint64_t>(1) << activePos + 2 * directionOffset[0] + directionOffset[3];
    }
}

void MoveGenerator::generatePawnAttack(const int activePos, const color activeColor) {
    const int dirMultiplier = activeColor == white ? 1:-1;
    const int dirOffset = activeColor == white? 0:2;
    //need to make sure that they are not at edges of board
    if(numSquareToEdge[activePos][3] !=0 && numSquareToEdge[activePos][dirOffset] != 0) {
        const int forwardLeft = directionOffset[3] + directionOffset[0] * dirMultiplier;
        const int targetSquare = forwardLeft + activePos;
        bitToOneAtPos(&pawnAttackMap, targetSquare);
    }
    if(numSquareToEdge[activePos][1] !=0 && numSquareToEdge[activePos][dirOffset] != 0) {
        const int forwardRight = directionOffset[1] + directionOffset[0] * dirMultiplier;
        const int targetSquare = forwardRight + activePos;
        bitToOneAtPos(&pawnAttackMap, targetSquare);
    }
}

void MoveGenerator::generatePawnMoves() {
    const std::vector<piece_data> pawns = board->getPiceColorList(pawn, position.friendlyColor);
    const int pawnForwards = position.friendlyColor == white? -8:8;
    const int startRank = position.friendlyColor == white? 6:1;
    const int promoteFlagRank = position.friendlyColor == white? 1:6;

    for(piece_data pawn : pawns) {
        const bool promoteFlag = promoteFlagRank == pawn.index/8;
        const int targetSquare = pawn.index + pawnForwards;
        if(board->getPieceAtSquare(targetSquare) == none) {
            if(!checkValueAtPos(position.pinRayMask,pawn.index) ||
                IsMoveAlongRay(pawnForwards, pawn.index, position.activeKingIndex)) {
                if(!position.inCheck || checkValueAtPos(position.checkRayMask, targetSquare)) {
                    if(promoteFlag) {
                        generatePromoionMoves(targetSquare, pawn.index);
                    }
                    else {
                        legalMoves.push_back({pawn.index, targetSquare});
                    }
                }
                if(pawn.index/8 == startRank) {
                    const int targetTwo = targetSquare + pawnForwards;
                    if(board->getPieceAtSquare(targetTwo) == none) {
                        if(!position.inCheck || checkValueAtPos(position.checkRayMask, targetTwo)) {
                            legalMoves.push_back({pawn.index, targetTwo, twoSquarePawnMove});
                        }
                    }
                }
            }
        }
        struct enPassDetails {
            int target;
            int dirOffset;
        };
        std::vector<enPassDetails> enPassTargets;
        const color activeColor = position.friendlyColor;
        const int dirMultiplier = activeColor == white ? 1:-1;
        const int dirOffset = activeColor == white? 0:2;
        //need to make sure that they are not at edges of board
        if(numSquareToEdge[pawn.index][3] !=0 && numSquareToEdge[pawn.index][dirOffset] != 0) {
            const int forwardLeft = directionOffset[3] + directionOffset[0] * dirMultiplier;
             enPassTargets.push_back({forwardLeft + pawn.index, forwardLeft});
        }
        if(numSquareToEdge[pawn.index][1] !=0 && numSquareToEdge[pawn.index][dirOffset] != 0) {
            const int forwardRight = directionOffset[1] + directionOffset[0] * dirMultiplier;
            enPassTargets.push_back({forwardRight + pawn.index, forwardRight});
        }
        for(auto [target, dirOffset] : enPassTargets) {
            if(checkValueAtPos(position.pinRayMask, pawn.index) &&
                !IsMoveAlongRay(dirOffset, position.activeKingIndex, pawn.index)) {
                continue;
            }
            if(board->isColor(board->getPieceAtSquare(target), position.opponentcolor)) {
                if(position.inCheck && !checkValueAtPos(position.checkRayMask, target)) {
                    continue;
                }
                if(promoteFlag) {
                    generatePromoionMoves(target, pawn.index);
                }
                else {
                    legalMoves.push_back({pawn.index, target});
                }
            }
            if(target == position.enPassantIndex && !position.enPassImposible) {
                legalMoves.push_back({pawn.index, target, enPassantCapture});
            }
        }
    }


}

void MoveGenerator::generatePromoionMoves(const int targetSquare, const int startSquare) {
    for(int i = 7; i < 11; i++) {
        const auto promFlag = static_cast<moveFlag>(i);
        legalMoves.push_back({startSquare, targetSquare, promFlag});
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
            if(const int index = directionOffset[i] + position.activeKingIndex;
                board->getPieceColor(board->getPieceAtSquare(index)) != position.friendlyColor) {
                targetSquares.push_back(index);
            }
        }
    }
    //Now check to see if they are legal
    for(const int target : targetSquares) {
        const bool capture = board->isColor(board->getPieceAtSquare(target), position.opponentcolor);
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
        const color fColor = position.friendlyColor;
        const bool kingside = fColor == white ? position.castleAbility[0] : position.castleAbility[2];
        const bool queenside = fColor == white ? position.castleAbility[1] : position.castleAbility[3];
        if(kingside) {
            const int targetIndex = position.activeKingIndex + 2; //right two
            const int passThroughIndex = position.activeKingIndex + 1; //need to check both
            if(!checkValueAtPos(attackMap, targetIndex) &&
                !checkValueAtPos(attackMap, passThroughIndex)) {
                if(board->getPieceAtSquare(targetIndex) == 0 &&
                    board->getPieceAtSquare(passThroughIndex) == 0) {
                    legalMoves.push_back({position.activeKingIndex,targetIndex,
                    fColor == white? whiteKingSide:blackKingSide});
                }
            }
        }
        if(queenside) {
            const int targetIndex = position.activeKingIndex - 2; //left two
            const int passThroughIndex = position.activeKingIndex - 1; //need to check both
            if(!checkValueAtPos(attackMap, targetIndex) &&
                !checkValueAtPos(attackMap, passThroughIndex)) {
                if(board->getPieceAtSquare(targetIndex) == 0 &&
                    board->getPieceAtSquare(passThroughIndex) == 0) {
                    legalMoves.push_back({position.activeKingIndex,targetIndex,
                        fColor == white? whiteQueenSide:blackQueenSide});
                    }
                }
        }
    }
}
