cmake_minimum_required(VERSION 3.0)
project(chess_cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

include_directories(
	src/
)

add_executable(
	chess
	src/game/board.cpp
	src/game/defs.cpp
	src/main.cpp
)
