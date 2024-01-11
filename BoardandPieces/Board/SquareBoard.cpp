//
// Created by tobedetered on 1/9/24.
//

#include "SquareBoard.h"

#include <iostream>

SquareBoard::SquareBoard() {
    initFenToID();
    initCharMap();
    initRankMap();
    readFromFen("7K/8/k1P5/7p/8/8/8/8");
}

void SquareBoard::readFromFen(std::string fenString) {
    //starting pos
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    int file = 0;
    int rank = 0;
    int j = 0;
    //get position
    for(int i = 0; i < fenString.size(); i++) {
        char token = fenString[i];
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
            gameDetails.whiteMove = true;
        }
        else {
            gameDetails.whiteMove = false;
        }
        j++;
    }
    //getCastleAbility
    gameDetails.castleAbility = 0;
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
        if(token == ' ') {
            j++;
            break;
        }
        switch (token) {
        case 'K':
            gameDetails.castleAbility = gameDetails.castleAbility | 8;
            break;
        case 'Q':
            gameDetails.castleAbility = gameDetails.castleAbility | 4;
            break;
        case 'k':
            gameDetails.castleAbility = gameDetails.castleAbility | 2;
            break;
        case 'q':
            gameDetails.castleAbility = gameDetails.castleAbility | 1;
            break;
        case '-':
            gameDetails.castleAbility = 0;
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
            gameDetails.enPessantTarget = access(enFile, enRank);
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
        gameDetails.halfMvoes = token - '0';
        j++;
    }
    //get full moves
    for(int i = j; i < fenString.size(); i++) {
        const char token = fenString[i];
        if(token == ' ') {
            break;
        }
        gameDetails.fullMoves = token - '0';
        j++;
    }
}


int SquareBoard::access(const int file, const int rank) {return file + 8 * rank;}

int SquareBoard::getPieceAtValue(const int index) const {return board[index];}

void SquareBoard::movePiece(const fileRank start, const fileRank end) {
    if(start.file > 7 || start.rank > 7 || start.file < 0 || start.rank < 0) {
        std::cerr << "ERROR: Invalid start selected" << std::endl;
        exit(1);
    }
    if(end.file > 7 || end.rank > 7 || end.file < 0 || end.rank < 0) {
        std::cerr << "ERROR: Invalid end selected" << std::endl;
        exit(1);
    }
    const int activePiece = board[access(start.file, start.rank)];
    board[access(end.file, end.rank)] = activePiece;
}

void SquareBoard::initFenToID() {
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

void SquareBoard::initCharMap() {
    //they are opposite because upper left is 0,0
    charRankMap.emplace('a', 0);
    charRankMap.emplace('b', 1);
    charRankMap.emplace('c', 2);
    charRankMap.emplace('d', 3);
    charRankMap.emplace('e', 4);
    charRankMap.emplace('f', 5);
    charRankMap.emplace('g', 6);
    charRankMap.emplace('h', 7);
}

void SquareBoard::initRankMap() {
    //need to convert from top left origin to bottom left
    realToFakeRank.emplace(0, 7);
    realToFakeRank.emplace(1, 6);
    realToFakeRank.emplace(2, 5);
    realToFakeRank.emplace(3, 4);
    realToFakeRank.emplace(4, 3);
    realToFakeRank.emplace(5, 2);
    realToFakeRank.emplace(6, 1);
    realToFakeRank.emplace(7, 0);
}

