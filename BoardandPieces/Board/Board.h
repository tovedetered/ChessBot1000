//
// Created by tobedetered on 1/13/24.
//

#ifndef BOARD_H
#define BOARD_H
#include <stack>
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <string>

#include "../Pieces.h"
#include "../../CommonUtilities.h"


class Board {
public:
    Board();
    gameState getCurrentGameState() const;
    bool getColorToMove();
    int getKingPos(color color) const;
    int getPieceAtSquare(int index) const;

    void loadPosFromFen(const std::string& fenString);

    void updateBoard(int board[64]);
    std::vector<piece_data> getPiceColorList(piece piece, color color);
    bool isColor(int piece, color color) const;
    color getPieceColor(int piece_) const;
    piece getPieceType(int piece_) const;

    void makeMove(move);
    void undoMove(move toUndo);
private:
    using pieceList = std::unordered_map<color, std::vector<piece_data>*>;
    gameState currentState;
    bool colorToMove{};
    std::stack<gameState> gameHistory;
    pieces pieceRef;
    piecesDataThing pieceTable;
    int kingsPos[2]{};

    int board[64]{};


    pieceList rooks;
    pieceList queens;
    pieceList pawns;
    pieceList bishops;
    pieceList knights;
    pieceList kings;

    std::unordered_map<piece , pieceList*> pieceListMap;
    void initPiceListMap();

    void addPiceToMap(int pieceValue, int index);
    void removePiceFromMap(int pieceValue, int index);
    void movePieceInMap(int startindex, int endIndex, int pieceValue);

    std::unordered_map<char, int> fenToID;
    void initFenToID();

    void initCharMap();
    std::unordered_map<char, int> charRankMap;
    void initRankMap();
    std::unordered_map<int, int> realToFakeRank;
};



#endif //BOARD_H
