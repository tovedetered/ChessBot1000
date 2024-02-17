//
// Created by tobedetered on 2/5/24.
//

#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <stack>
#include <string>
#include <iostream>
#include "board_representations/SquareBoardRepresentation.h"
#include "CommonUtilities.h"


class BoardManager {
public:
    BoardManager();
    ~BoardManager();

    void makeMove(move played);
    void undoMove(move toUndo);

    void loadFenPos(const std::string& fen);

    gameState getCurrentState();

private:
    SquareBoardRepresentation* squareBoard;

    gameState currentState;
    std::stack<gameState> gameHistory;

    pieces pieceRef;
    piecesDataThing pieceTable;

    std::unordered_map<char, int> fenToID;
    void initFenToID();

    void initCharMap();
    std::unordered_map<char, int> charRankMap;
};



#endif //BOARDMANAGER_H
