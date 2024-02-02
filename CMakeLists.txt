cmake_minimum_required(VERSION 3.27)
project(ChessBot1000)

set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

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
        Pieces/piece_list.cpp
        Pieces/piece_list.h
        Pieces/Piece.cpp
        Pieces/Piece.h
        Pieces/Sliding_pieces/Queen.cpp
        Pieces/Sliding_pieces/Queen.h
        Pieces/Sliding_pieces/Rook.cpp
        Pieces/Sliding_pieces/Rook.h
        Pieces/Sliding_pieces/Bishop.cpp
        Pieces/Sliding_pieces/Bishop.h
        Pieces/Other_pieces/King.cpp
        Pieces/Other_pieces/King.h
        board_representations/SquareBoardRepresentation.cpp
        board_representations/SquareBoardRepresentation.h
        Pieces/PieceManager.cpp
        Pieces/PieceManager.h
        Pieces/Other_pieces/Knight.cpp
        Pieces/Other_pieces/Knight.h
        Pieces/Other_pieces/Pawn.cpp
        Pieces/Other_pieces/Pawn.h
)

target_link_libraries(${PROJECT_NAME} raylib)