//
// Created by tobedetered on 1/11/24.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BoardandPieces/Board/SquareBoard.h"
#include "BoardandPieces/Renderers/GraphicalBoard.h"

class GameManager {
public:
    GameManager();
    void startup();
    void mainLoop();
    void shutDown();
private:
    SquareBoard* board;
    GraphicalBoard* interface;
};



#endif //GAMEMANAGER_H
