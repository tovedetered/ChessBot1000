//
// Created by tobedetered on 1/13/24.
//

#ifndef BOARD_H
#define BOARD_H
#include <stack>
#include <unordered_map>
#include <vector>
#include <cstdio>

#include "../Pieces.h"
#include "../../CommonUtilities.h"


class Board {
public:
    Board();
    gameState getCurrentGameState() const;
    bool getColorToMove();
    int getKingPos(color color) const;
    int getPieceAtSquare(int index) const;

    void updateBoard(int board[64]);
    std::vector<piece_data> getPiceColorList(piece piece, color color);
    bool isColor(int piece, color color) const;
    color getPieceColor(int piece_) const;
    piece getPieceType(int piece_) const;
    void makeMove(move);
private:
    gameState currentState;
    bool colorToMove;
    std::stack<gameState> gameHistory;
    pieces pieceRef;

    int kingsPos[2];

    int board[64];


    std::unordered_map<color, std::vector<piece_data>> rooks;
    std::unordered_map<color, std::vector<piece_data>> queens;
    std::unordered_map<color, std::vector<piece_data>> pawns;
    std::unordered_map<color, std::vector<piece_data>> bishops;
    std::unordered_map<color, std::vector<piece_data>> knights;

    std::unordered_map<piece ,std::unordered_map<color, std::vector<piece_data>>> pieceListMap;
};



#endif //BOARD_H
