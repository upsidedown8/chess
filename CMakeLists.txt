cmake_minimum_required(VERSION 3.0)
project(chess)

set(CMAKE_BINARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 14)

include_directories(
    src/
    inc/
)

set(
    SRC_FILES
    src/game/bitboard.cpp
    src/game/board.cpp
    src/game/elo.cpp
    src/game/eval.cpp
    src/game/hashpos.cpp
    src/utils/rle.cpp
)

add_library(
    chess
    ${SRC_FILES}
)

add_executable(
    chess_console
    src/main_console.cpp
)
target_link_libraries(
    chess_console
    chess
)

add_executable(
    chess_gui
    src/main_gui.cpp
)
target_link_libraries(
    chess_gui
    chess
)
