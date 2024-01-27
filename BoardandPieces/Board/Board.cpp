//
// Created by tobedetered on 1/13/24.
//

#include "Board.h"

#include <iostream>

Board::Board() {
    std::fill(std::begin(board), std::end(board), 0);
    initCharMap();
    initRankMap();
    initPiceListMap();
    initFenToID();
}

Board::~Board() {
    delete pieceListMap[pawn];
    delete pieceListMap[knight];
    delete pieceListMap[bishop];
    delete pieceListMap[rook];
    delete pieceListMap[queen];
    delete pieceListMap[king];
}

gameState Board::getCurrentGameState() const {return currentState;}

int Board::getKingPos(const color color) const {return kingsPos[color];}

int Board::getPieceAtSquare(const int index) const {return board[index];}

void Board::loadPosFromFen(const std::string& fenString) {
//starting pos
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    int file = 0;
    int rank = 0;
    int j = 0;
    //get position
    for(char token : fenString) {
        if(token == ' ') {
            j++;
            break;
        }
        if(token == '/') {
            file = 0;
            rank ++;
            j++;
            continue;
        }
        if(std::isdigit(token)) {
            //convert token(char) to int
            file += token - '0';
            j++;
            continue;
        }
        if(std::isalpha(token)) {
            board[access(file, rank)] = fenToID[token];
            addPiceToMap(board[access(file, rank)], access(file,rank));
            if(token == 'k') {
                kingsPos[0] = {access(file, rank)};
            }
            else if(token == 'K') {
                kingsPos[1] = {access(file, rank)};
            }
            file ++;
        }
        j++;
    }
    //get whose move
    for(int i = j;i < fenString.size(); i++) {
        const char token = fenString[i];
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
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
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
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
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
            enRank = realToFakeRank[(token - 1) - '0'];
        }
        j++;
    }
    //get half moves
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
        if(token == ' ') {
            j++;
            break;
        }
        currentState.halfMoves = token - '0';
        j++;
    }
    //get full moves
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
        if(token == ' ') {
            break;
        }
        currentState.totalPly = token - '0';
        j++;
    }
}

std::vector<piece_data> Board::getPiceColorList(const piece piece_, const color color_) {
    piece_list *allPiecesOfType = pieceListMap[piece_];
    return *allPiecesOfType->list.at(color_);
}

bool Board::isColor(const int piece, const color color) const {
    if(color == white && pieceRef.colorMask & piece == pieceRef.white) return true;
    if(color == black && pieceRef.colorMask & piece == pieceRef.black) return true;
    return false;
}

color Board::getPieceColor(const int piece_) const {
    int colorType = piece_&pieceRef.colorMask; //isolate color
    const auto pieceColor = static_cast<color>(colorType);
    return pieceColor;
}

piece Board::getPieceType(const int piece_) const {
    int pieceType = piece_&pieceRef.typeMask; //isolate piece code
    const auto result = static_cast<piece>(pieceType);
    return result;
}

void Board::makeMove(const move currentMove) {
    //now make the move
    int movingPiece = board[currentMove.startIndex];
    board[currentMove.startIndex] = 0;
    int miscSquare = -1;
    currentState.pieceCaptureInMove = -1;
    currentState.mostRecentPromoPiece = -1;
    currentState.enPassantIndex = -1;
    currentState.castlePlayed.clear();
    //Catleing
    if(currentMove.flag != normal && static_cast<int>(currentMove.flag) < 5) {
        int movementMultiplier = 0;
        switch (static_cast<int>(currentMove.flag)) {
        case 1: //W-KS
            miscSquare = 63;
            movementMultiplier = 1;
            break;
        case 2:
            miscSquare = 56;
            movementMultiplier = -1;
            break;
        case 3:
            miscSquare = 7;
            movementMultiplier = 1;
            break;
        case 4:
            miscSquare = 0;
            movementMultiplier = -1;
            break;
        default:
            std::cerr << "WTF how the hell did this happen???";
            exit(-950);
        }
        const int targetRook = board[miscSquare];
        currentState.castlePlayed.rookStartIndex = miscSquare;

        //remove Rook from the board
        board[miscSquare] = 0;

        //put the rook on its new square
        //king handled below
        const int newRookSquare = miscSquare - 2*movementMultiplier;

        board[newRookSquare] = targetRook;
        movePieceInMap(miscSquare, newRookSquare, targetRook);

        currentState.castlePlayed.rookEndIndex = newRookSquare;
    }
    else if(currentMove.flag == twoSquarePawnMove) {
        //need to set en pass index to one behind
        const int backOffset = (currentMove.startIndex - currentMove.endIndex)/2;
        const int enSquare = currentMove.endIndex - backOffset;
        currentState.enPassantIndex = enSquare;
        movePieceInMap(currentMove.startIndex, currentMove.endIndex, movingPiece);
        //then move piece
    }
    else if(currentMove.flag == enPassantCapture) {
        // In order to remove the other pawn we need to look at current pawn's color
        const color captureingColor = getPieceColor(movingPiece);
        const int dirOff = captureingColor == white? 8:-8;
        removePiceFromMap(board[currentMove.endIndex + dirOff], currentMove.endIndex + dirOff);
        currentState.pieceCaptureInMove = board[currentMove.endIndex + dirOff];
        board[currentMove.endIndex + dirOff] = 0;
        currentState.enPassantIndex = -1;
    }
    else if(static_cast<int>(currentMove.flag) > 6) {
        const color pieceColor = getPieceColor(movingPiece);
        piece_data pieceData{};
        //remove the pawn
        removePiceFromMap(movingPiece, currentMove.startIndex);
        switch (static_cast<int>(currentMove.flag)) {
        case 7:
            //Queen
            movingPiece = static_cast<int>(pieceColor) | static_cast<int>(queen);
            break;
        case 8:
            movingPiece = static_cast<int>(pieceColor) | static_cast<int>(rook);
            break;
        case 9:
            movingPiece = static_cast<int>(pieceColor) | static_cast<int>(bishop);
            break;
        case 10:
            movingPiece = static_cast<int>(pieceColor) | static_cast<int>(knight);
            break;
        default:
            std::cerr << "enum exceeded somehow";
            exit(-1);
        }
        pieceData.index = currentMove.endIndex;
        pieceData.piece = movingPiece;
        currentState.enPassantIndex = -1;
        addPiceToMap(movingPiece, currentMove.endIndex);
        currentState.mostRecentPromoPiece = movingPiece;
    }
    else {
        //if a pawn did not move two spaces up we need to update it
        currentState.enPassantIndex = -1;
    }
    const int removedPiece = board[currentMove.endIndex];
    removePiceFromMap(removedPiece, currentMove.endIndex);
    if(removedPiece != 0) {
        currentState.pieceCaptureInMove = removedPiece;
    }
    movePieceInMap(currentMove.startIndex, currentMove.endIndex, movingPiece);
    board[currentMove.endIndex] = movingPiece;

    gameHistory.emplace(currentState);
    currentState.fiftyPlyCount ++;
    currentState.totalPly ++;
    currentState.switchColor();
}

void Board::undoMove(move toUndo) {
    currentState = gameHistory.top();
    gameHistory.pop();

    const int startIndex = toUndo.startIndex;
    const int endIndex = toUndo.endIndex;
    const int undoingPiece = board[endIndex];

    board[endIndex] = 0;
    //castleing
    if(static_cast<int>(toUndo.flag) < 5 && static_cast<int>(toUndo.flag) > 1) {
        //just need to move the rook
        board[currentState.castlePlayed.rookEndIndex] = 0;
        board[currentState.castlePlayed.rookStartIndex] = currentState.castlePlayed.rookType;
        movePieceInMap(currentState.castlePlayed.rookEndIndex, currentState.castlePlayed.rookStartIndex,
            currentState.castlePlayed.rookType);
    }
    //If there was a promotion
    if(static_cast<int>(toUndo.flag) <= 6) {
        board[startIndex] = undoingPiece;
        //Has to be wrong order
        movePieceInMap(endIndex, startIndex, undoingPiece);
    }
    else { //promotion happened
        board[startIndex] = currentState.mostRecentPromoPiece;
        addPiceToMap(static_cast<int>(currentState.colorMove) | static_cast<int>(pawn), startIndex);
        removePiceFromMap(undoingPiece, endIndex);
    }

    //if there was a capture
    if(currentState.pieceCaptureInMove != -1) {
        if(toUndo.flag != enPassantCapture) {
            board[endIndex] = currentState.pieceCaptureInMove;
            addPiceToMap(currentState.pieceCaptureInMove, endIndex);
        }
        else {
            const color captureingColor = getPieceColor(undoingPiece);
            const int dirOff = captureingColor == white? 8:-8;
            board[endIndex + dirOff] = currentState.pieceCaptureInMove;
            addPiceToMap(currentState.pieceCaptureInMove, endIndex + dirOff);
        }
    }
}

uint64_t Board::perft(int depth) {
    std::vector<move> movesAtDepth = moveGen->generateMoves();
    uint64_t nodes = 0;

    unsigned long numMoves = movesAtDepth.size();
    if(depth == 1) {
        return static_cast<uint64_t>(numMoves);
    }

    for(int i = 0; i < numMoves; i++) {
        makeMove(movesAtDepth[i]);
        nodes += perft(depth - 1);
        undoMove(movesAtDepth[i]);
    }
    return nodes;
}

void Board::setMoveGen(MoveGenerator* inGen) {
    moveGen = inGen;
}

void Board::initPiceListMap() {
    pieceListMap.emplace(rook, &rooks);
    pieceListMap.emplace(queen, &queens);
    pieceListMap.emplace(knight, &knights);
    pieceListMap.emplace(king, &kings);
    pieceListMap.emplace(bishop, &bishops);
    pieceListMap.emplace(pawn, &pawns);
}

void Board::addPiceToMap(const int pieceValue, const int index) {
    piece pieceType = getPieceType(pieceValue);

    const piece_list* listOfPieces = pieceListMap[pieceType];

    std::vector<piece_data>* piecesOfColorAndPiece = listOfPieces->list.at(getPieceColor(pieceValue));
    piecesOfColorAndPiece->push_back({pieceValue, index});
}

void Board::removePiceFromMap(const int pieceValue, const int index) {
    if(pieceValue == 0) {
        return;
    }

    const piece_list* listOfPieces = pieceListMap[getPieceType(pieceValue)];
    std::vector<piece_data>* piecesOfColorAndPiece = listOfPieces->list.at(getPieceColor(pieceValue));

    for(int i = 0; i < piecesOfColorAndPiece->size(); i++) {
        const piece_data currentPice = piecesOfColorAndPiece->at(i);
        if(currentPice.index == index && currentPice.piece == pieceValue) {
            piecesOfColorAndPiece->erase(piecesOfColorAndPiece->begin() + i);
            return;
        }
    }
    std::cerr << "Removing Piece Unsucessful";
    exit(-1);
}

void Board::movePieceInMap(int startindex, int endIndex, int pieceValue) {
    if(pieceValue == 0) {
        return;
    }
    const piece_list* listOfPieces = pieceListMap[getPieceType(pieceValue)];
    // ReSharper disable once CppLocalVariableMayBeConst
    //It cannot be
    std::vector<piece_data>* piecesOfColorAndPiece = listOfPieces->list.at(getPieceColor(pieceValue));

    for(auto & i : *piecesOfColorAndPiece) {
        const piece_data currentPice = i;
        if(currentPice.index == startindex && currentPice.piece == pieceValue) {
            i = {pieceValue, endIndex};
            return;
        }
    }
    std::cerr << "Moving Piece at" << startindex << "unsucessful";
    exit(-1);
}

void Board::initFenToID() {
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

void Board::initCharMap() {
    charRankMap.emplace('a', 0);
    charRankMap.emplace('b', 1);
    charRankMap.emplace('c', 2);
    charRankMap.emplace('d', 3);
    charRankMap.emplace('e', 4);
    charRankMap.emplace('f', 5);
    charRankMap.emplace('g', 6);
    charRankMap.emplace('h', 7);
}

void Board::initRankMap() {
    realToFakeRank.emplace(0, 7);
    realToFakeRank.emplace(1, 6);
    realToFakeRank.emplace(2, 5);
    realToFakeRank.emplace(3, 4);
    realToFakeRank.emplace(4, 3);
    realToFakeRank.emplace(5, 2);
    realToFakeRank.emplace(6, 1);
    realToFakeRank.emplace(7, 0);
}