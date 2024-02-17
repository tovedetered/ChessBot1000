//
// Created by tobedetered on 2/5/24.
//

#include "BoardManager.h"

BoardManager::BoardManager() {
    squareBoard = new SquareBoardRepresentation;
    currentState = {};
}

BoardManager::~BoardManager() {
    delete squareBoard;
}

void BoardManager::makeMove(move played) {
    Piece* movingPiece = squareBoard->getPieceAtSquare(played.startIndex);
    //setting things up
    int miscSquare = -1;
    currentState.pieceCaptureInMove = -1;
    currentState.mostRecentPromoPiece = -1;
    currentState.enPassantIndex = -1;
    currentState.castlePlayed.clear();
    bool promoted = false;
    bool resetFiftyPly = false;
    if(squareBoard->getPieceAtSquare(played.endIndex) != nullptr) resetFiftyPly = true;
    //Castleing
    if(played.flag != normal && static_cast<int>(played.flag) < 5) {
        int movementMultiplier = 0;
        int startRookSquare;
        int newRookSquare;
        switch (static_cast<int>(played.flag)) {
        case 1: //W-KS
            startRookSquare = 0;
            movementMultiplier = 1;
            newRookSquare = 2;
            break;
        case 2:
            startRookSquare = 7;
            movementMultiplier = -1;
            newRookSquare = 4;
            break;
        case 3:
            startRookSquare = 63;
            movementMultiplier = 1;
            newRookSquare = 61;
            break;
        case 4:
            startRookSquare = 56;
            movementMultiplier = -1;
            newRookSquare = 59;
            break;
        default:
            std::cerr << "WTF how the hell did this happen???";
            exit(-950);
        }
        squareBoard->movePiece(startRookSquare, newRookSquare);

        currentState.castlePlayed.rookStartIndex = startRookSquare;
        currentState.castlePlayed.rookType = squareBoard->getPieceAtSquare(newRookSquare)->getType();

        currentState.castlePlayed.rookEndIndex = newRookSquare;
    }
    else if(played.flag == twoSquarePawnMove) {
        resetFiftyPly = true;
        //need to set en pass index to one behind
        int dirOff = movingPiece->getColor() == white? -8:8;
        const int enSquare = played.endIndex + dirOff;
        currentState.enPassantIndex = enSquare;
    }
    //enPassant
    else if(played.flag == enPassantCapture) {
        resetFiftyPly = true;
        // In order to remove the other pawn we need to look at current pawn's color
        const color captureingColor = movingPiece->getColor();
        const int dirOff = captureingColor == white? -8:8;
        currentState.pieceCaptureInMove = squareBoard->getPieceAtSquare(played.endIndex + dirOff)->getPieceValue();
        squareBoard->removePiece(played.endIndex + dirOff);
        currentState.enPassantIndex = -1;
    }
    //Promotions
    else if(static_cast<int>(played.flag) > 6) {
        resetFiftyPly = true;
        const int pieceColor = static_cast<int>(movingPiece->getColor());
        int index = movingPiece->getIndex();
        int piece;
        piece_data pieceData{};
        currentState.mostRecentPromoPiece = movingPiece->getPieceValue();
        //remove the pawn
        squareBoard->removePiece(played.startIndex);
        promoted = true;
        switch (static_cast<int>(played.flag)) {
        case 7:
            //Queen
            piece = static_cast<int>(queen)|pieceColor;
            break;
        case 8:
            piece = static_cast<int>(pieceColor) | static_cast<int>(rook);
            break;
        case 9:
            piece = static_cast<int>(pieceColor) | static_cast<int>(bishop);
            break;
        case 10:
            piece = static_cast<int>(pieceColor) | static_cast<int>(knight);
            break;
        default:
            std::cerr << "enum exceeded somehow";
            exit(-1);
        }
        pieceData.index = played.endIndex;
        pieceData.piece = piece;
        currentState.enPassantIndex = -1;
        squareBoard->addPiece(pieceData);
    }
    else {
        //if a pawn did not move two spaces up we need to update it
        currentState.enPassantIndex = -1;
    }

    if(!promoted) {
        //If there is a piece there this will remove it
        //And if we promoted we already "moved" the piece
        squareBoard->movePiece(played.startIndex, played.endIndex);
    }
    gameHistory.emplace(currentState);
    if(!resetFiftyPly) currentState.fiftyPlyCount ++;
    else currentState.fiftyPlyCount = 0;
    currentState.totalPly ++;
    currentState.switchColor();
}

void BoardManager::undoMove(move toUndo) {
    int endOfUndo = toUndo.startIndex;
    int startOfUndo = toUndo.endIndex;

    currentState = gameHistory.top();
    gameHistory.pop();

    Piece* undoingPiece = squareBoard->getPieceAtSquare(startOfUndo);

    if(static_cast<int>(toUndo.flag) < 5 && static_cast<int>(toUndo.flag) > 1) {
        //just need to move the rook
        squareBoard->movePiece(currentState.castlePlayed.rookEndIndex,
            currentState.castlePlayed.rookStartIndex);
    }
    //If there was no promotion
    if(static_cast<int>(toUndo.flag) <= 6) {
        squareBoard->movePiece(startOfUndo, endOfUndo);
    }
    else { //promotion happened
        squareBoard->removePiece(startOfUndo);
        squareBoard->addPiece({currentState.mostRecentPromoPiece, endOfUndo});
    }

    if(currentState.pieceCaptureInMove != -1) {
        if(toUndo.flag != enPassantCapture) {

            squareBoard->addPiece({currentState.pieceCaptureInMove, startOfUndo});
        }
        else {
            const color captureingColor = undoingPiece->getColor();
            const int dirOff = captureingColor == white? -8:8;
            squareBoard->addPiece({currentState.pieceCaptureInMove, startOfUndo + dirOff});
        }
    }
}


void BoardManager::loadFenPos(const std::string& fen) {
    //rank starts at 7 becasue of the fen notation screwing things up
    int file = 0;
    int rank = 7;
    //for keeping track of the increments
    int j = 0;

    int numBoard[64];
    std::fill(std::begin(numBoard), std::end(numBoard), 0);

    for(char token : fen) {
        if(token == ' ') {
            j++;
            break;
        }
        if(token == '/') {
            file = 0;
            rank--;
            j++;
            continue;
        }
        if(std::isdigit(token)) {
            file += token - '0';
            j++;
            continue;
        }
        if(std::isalpha(token)) {
            numBoard[access(file, rank)] = fenToID[token];

            if(token == 'k') {
                currentState.kingsPos[1] = {access(file, rank)};
            }
            else if(token == 'K') {
                currentState.kingsPos[0] = {access(file,rank)};
            }
            file++;
        }
        j++;
    }

    for(int i = j; i < fen.size(); i++) {
        const char token = fen[i];
        if(token == ' ') {
            j++;
            break;
        }
        if(token == 'w') {
            currentState.colorMove = white;
        }
        else {
            currentState.colorMove = black;
        }
        j++;
    }

    //getCastleAbility
    currentState.castleAbility = 0;
    for(int i = j; i < fen.size(); i++) {
        const char token = fen[i];
        if(token == ' ') {
            j++;
            break;
        }
        switch (token) {
        case 'K':
            currentState.castleAbility = currentState.castleAbility | 1;
            break;
        case 'Q':
            currentState.castleAbility = currentState.castleAbility | 2;
            break;
        case 'k':
            currentState.castleAbility = currentState.castleAbility | 4;
            break;
        case 'q':
            currentState.castleAbility = currentState.castleAbility | 8;
            break;
        case '-':
            currentState.castleAbility = 0;
            break;
        default:
            std::cerr << "ERROR: Invalid Fen string" << std::endl;
            exit(1);
        }
        j++;
    }
    //Enpessant target square
    int enRank = 9;
    int enFile = 9;
    for(int i = j; i < fen.size(); i++) {
        const char token = fen[i];
        if(token == ' ') {
            currentState.enPassantIndex = access(enFile, enRank);
            j++;
            break;
        }
        //first is c etc so the rank
        if(std::isalpha(token)) {
            enFile = charRankMap[token];
        }
        else if(std::isdigit(token)) {
            enRank = (token - '0') - 1;
        }
        j++;
    }
    //get half moves
    for(int i = j; i < fen.size(); i++) {
        const char token = fen[i];
        if(token == ' ') {
            j++;
            break;
        }
        currentState.halfMoves = token - '0';
        j++;
    }
    //get full moves
    for(int i = j; i < fen.size(); i++) {
        const char token = fen[i];
        if(token == ' ') {
            break;
        }
        currentState.totalPly = token - '0';
        j++;
    }

    squareBoard->updateBoard(numBoard);
}

gameState BoardManager::getCurrentState() {
    return currentState;
}

void BoardManager::initFenToID() {
    fenToID.emplace('p', pieceTable.pawn | pieceTable.black);
    fenToID.emplace('P', pieceTable.pawn | pieceTable.white);
    fenToID.emplace('r', pieceTable.rook | pieceTable.black);
    fenToID.emplace('R', pieceTable.rook | pieceTable.white);
    fenToID.emplace('n', pieceTable.knight | pieceTable.black);
    fenToID.emplace('N', pieceTable.knight | pieceTable.white);
    fenToID.emplace('b', pieceTable.bishop | pieceTable.black);
    fenToID.emplace('B', pieceTable.bishop | pieceTable.white);
    fenToID.emplace('q', pieceTable.queen | pieceTable.black);
    fenToID.emplace('Q', pieceTable.queen | pieceTable.white);
    fenToID.emplace('k', pieceTable.king | pieceTable.black);
    fenToID.emplace('K', pieceTable.king | pieceTable.white);
}

void BoardManager::initCharMap() {
    charRankMap.emplace('a', 0);
    charRankMap.emplace('b', 1);
    charRankMap.emplace('c', 2);
    charRankMap.emplace('d', 3);
    charRankMap.emplace('e', 4);
    charRankMap.emplace('f', 5);
    charRankMap.emplace('g', 6);
    charRankMap.emplace('h', 7);
}
