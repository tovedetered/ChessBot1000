//
// Created by tobedetered on 1/9/24.
//

#ifndef GRAPHICALBOARD_H
#define GRAPHICALBOARD_H

#include <raylib.h>
#include <vector>
#include <unordered_map>

#include "../Pieces.h"
#include "../Board/SquareBoard.h"


struct square {
    Color color;
    Vector2 pos;
    int piece;
};

struct input {
    fileRank startSqare;
    fileRank endSquare;
};
class GraphicalBoard {
public:
    GraphicalBoard(int screenWidth, int screenHeight);
    void renderLoop();
    void init(SquareBoard* board_);
    static bool isRunning();
    static void shutdown();
private:
    void drawSquare(Color squareColor, Vector2 pos) const;
    void drawPiece(int pieceID, const fileRank pos);
    void initPieceMap();

    Vector2 screenDimension{};
    Vector2 boardDimension{};
    int boardSize;
    int squareSize;

    input getPlayerInput();
    pieces pieceTable;
    std::unordered_map<int, Texture> pieceTex;
    float scaleFactor;
    void calculateScaleFactor();

    Color lightColor{};
    Color darkColor{};

    SquareBoard* board;
};



#endif //GRAPHICALBOARD_H
