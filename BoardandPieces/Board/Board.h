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

#include "../MoveGenerator.h"
#include "../Pieces.h"
#include "../../CommonUtilities.h"
#include "../Pieces/piece_list.h"


class Board {
public:
    Board();
    ~Board();
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
    uint64_t perft(int depth);
    uint64_t dividePerft(int depth);
    int getKingPos(color color_);
    void setMoveGen(class MoveGenerator* inGen);

    debugInfo debug{};
private:

    gameState currentState;
    bool colorToMove{};
    std::stack<gameState> gameHistory;
    pieces pieceRef;
    piecesDataThing pieceTable;
    int kingsPos[2]{};

    int board[64]{};
    std::string convertIndexToThing(int index);
    MoveGenerator* moveGen{};

    piece_list rooks;
    piece_list queens;
    piece_list pawns;
    piece_list bishops;
    piece_list knights;
    piece_list kings;
    void initPieceLists();

    std::unordered_map<piece , piece_list*> pieceListMap;
    void initPiceListMap();

    void addPiceToMap(int pieceValue, int index);
    void removePiceFromMap(int pieceValue, int index);
    void movePieceInMap(int startindex, int endIndex, int pieceValue);

    std::unordered_map<char, int> fenToID;
    void initFenToID();

    void initCharMap();
    std::unordered_map<char, int> charRankMap;
    std::unordered_map<int , char> fileToCharMAp;
    std::unordered_map<int , char> rankToCharMap;
    void initRankMap();
    std::unordered_map<int, int> realToFakeRank;
};



#endif //BOARD_H
