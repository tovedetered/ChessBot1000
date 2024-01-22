cmake_minimum_required(VERSION 3.27)
project(ChessBot1000)

find_package(raylib 5.0 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessBot1000 main.cpp
        BoardandPieces/Pieces.h
        BoardandPieces/Renderers/GraphicalBoard.cpp
        BoardandPieces/Renderers/GraphicalBoard.h
        BoardandPieces/Board/SquareBoard.h
        BoardandPieces/Board/SquareBoard.cpp
        GameManager.cpp
        GameManager.h
        BoardandPieces/MoveGenerator.cpp
        BoardandPieces/MoveGenerator.h
        BoardandPieces/Board/Board.cpp
        BoardandPieces/Board/Board.h
        CommonUtilities.h
)

target_link_libraries(${PROJECT_NAME} raylib)