//
// Created by tobedetered on 1/9/24.
//

#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <raylib.h>
#include <vector>
#include <unordered_map>

#include "../Pieces.h"
#include "../Board/Board.h"
#include "../../CommonUtilities.h"


struct square {
    Color color;
    Vector2 pos;
    int piece;
};


class GraphicalBoard {
public:
    GraphicalBoard(int screenWidth, int screenHeight);
    void renderLoop();
    void init(Board* board_);
    static bool isRunning();
    static void shutdown();
    fileRank getPlayerInput();
private:
    void drawSquare(Color squareColor, Vector2 pos) const;
    void drawPiece(int pieceID, const fileRank pos);
    void initPieceMap();

    Vector2 screenDimension{};
    Vector2 boardDimension{};
    int boardSize;
    int squareSize;

    pieces pieceTable;
    std::unordered_map<int, Texture> pieceTex;
    float scaleFactor;
    void calculateScaleFactor();

    Color lightColor{};
    Color darkColor{};

    Board* board;
};



#endif //GRAPHICALBOARD_H
