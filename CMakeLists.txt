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
        BoardandPieces/Pieces/piece_list.cpp
        BoardandPieces/Pieces/piece_list.h
)

target_link_libraries(${PROJECT_NAME} raylib)