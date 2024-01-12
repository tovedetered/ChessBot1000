//
// Created by tobedetered on 1/9/24.
//

#include "GraphicalBoard.h"

GraphicalBoard::GraphicalBoard(const int screenWidth, const int screenHeight) {
    screenDimension.x = screenWidth;
    screenDimension.y = screenHeight;
    if(screenWidth < screenHeight) {
        boardDimension.x = screenWidth;
        boardDimension.y = screenHeight;
    }
    else {
        boardDimension.x = screenWidth;
        boardDimension.y = screenHeight;
    }
    boardSize = 64;
    lightColor = {238, 238, 210, 100};
    darkColor = {118, 150, 86, 100};
}

void GraphicalBoard::renderLoop() {
    calculateScaleFactor();
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file ++){
            Vector2 pos;
            const bool isLight = (file + rank) % 2 ? true:false;
            const Color squareColor = isLight ? darkColor:lightColor;
            pos.x = file * (boardDimension.x/8);
            pos.y = rank * (boardDimension.y/8);
            drawSquare(squareColor, pos);
            int piece = board->getPieceAtValue(board->access(file, rank));
            if(piece != 0) {
                drawPiece(piece, {file, rank});
            }
        }
    }
}

void GraphicalBoard::init(SquareBoard* board_) {
    InitWindow(static_cast<int>(screenDimension.x), static_cast<int>(screenDimension.y), "Chess Bot 1000");
    SetTargetFPS(60);
    initPieceMap();
    board = board_;
}

bool GraphicalBoard::isRunning() {return !WindowShouldClose();}

void GraphicalBoard::shutdown() {CloseWindow();}

void GraphicalBoard::drawSquare(const Color squareColor, const Vector2 pos) const {
    DrawRectangle(static_cast<int>(pos.x),static_cast<int>(pos.y),
        static_cast<int>(boardDimension.x/8), static_cast<int>(boardDimension.y/8),
        squareColor);
}

void GraphicalBoard::drawPiece(const int pieceID, const fileRank coordPos) {
    const Texture activeTex = pieceTex[pieceID];
    Vector2 pos{};
    pos.x = coordPos.file * (boardDimension.x/8);
    pos.y = coordPos.rank * (boardDimension.y/8);
    Vector2 correctedPos = pos;
    //pos is upper right corner
    //by knowing the scaled dimensions and the size of each square we can center it
    //find difference between right edge of square and right edge of piece and move pos -> that/2
    const float xShift = ((pos.x + squareSize) - (pos.x + (activeTex.width*scaleFactor)))/2;
    correctedPos.x += xShift;
    const float yShift = ((pos.y + squareSize) - (pos.y + (activeTex.height * scaleFactor)))/2;
    correctedPos.y += yShift;
    DrawTextureEx(activeTex,correctedPos, 0, scaleFactor, RAYWHITE);
}

void GraphicalBoard::initPieceMap() {
    //first Init Textures of pieces then we add them to the map
    Texture bBishop = LoadTexture("../assets/BlackPieces/b_bishop.png");
    pieceTex.emplace(pieceTable.bishop | pieceTable.black, bBishop);
    Texture bKing = LoadTexture("../assets/BlackPieces/b_king.png");
    pieceTex.emplace(pieceTable.king | pieceTable.black, bKing);
    Texture bKnight = LoadTexture("../assets/BlackPieces/b_knight.png");
    pieceTex.emplace(pieceTable.knight | pieceTable.black, bKnight);
    Texture bPawn = LoadTexture("../assets/BlackPieces/b_pawn.png");
    pieceTex.emplace(pieceTable.pawn | pieceTable.black, bPawn);
    Texture bQueen = LoadTexture("../assets/BlackPieces/b_queen.png");
    pieceTex.emplace(pieceTable.queen | pieceTable.black, bQueen);
    Texture bRook = LoadTexture("../assets/BlackPieces/b_rook.png");
    pieceTex.emplace(pieceTable.rook | pieceTable.black, bRook);

    Texture wBishop = LoadTexture("../assets/WhitePieces/w_bishop.png");
    pieceTex.emplace(pieceTable.bishop | pieceTable.white, wBishop);
    Texture wKing = LoadTexture("../assets/WhitePieces/w_king.png");
    pieceTex.emplace(pieceTable.king | pieceTable.white, wKing);
    Texture wKnight = LoadTexture("../assets/WhitePieces/w_knight.png");
    pieceTex.emplace(pieceTable.knight | pieceTable.white, wKnight);
    Texture wPawn = LoadTexture("../assets/WhitePieces/w_pawn.png");
    pieceTex.emplace(pieceTable.pawn | pieceTable.white, wPawn);
    Texture wQueen = LoadTexture("../assets/WhitePieces/w_queen.png");
    pieceTex.emplace(pieceTable.queen | pieceTable.white, wQueen);
    Texture wRook = LoadTexture("../assets/WhitePieces/w_rook.png");
    pieceTex.emplace(pieceTable.rook | pieceTable.white, wRook);
}

fileRank GraphicalBoard::getPlayerInput() {
    //TODO implement drag and drop
    //First find what square the mouse is on, then if clicked pick up the piece,
    //bind center of piece to mouse pos
    //place on square when clicked
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int file = GetMouseX()/squareSize;
        int rank = GetMouseY()/squareSize;
        return {file, rank};
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        return {9,9};
    }

    return {30,30}; //30,30 is do nothing
}

void GraphicalBoard::calculateScaleFactor() {
    //First find how big each square is
    squareSize = boardDimension.x/8;
    //383 is largets height
    //Find value that *383 = squareSize and has some wiggle room
    scaleFactor = (squareSize-(boardDimension.x/100.0f))/383.0f;
}
