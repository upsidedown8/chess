cmake_minimum_required(VERSION 3.0)
project(chess_cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

include_directories(
	src/
)

set(
	SRC_FILES
	src/game/board.cpp
	#src/game/movegen.cpp
	src/game/utils.cpp
)

add_executable(
	chess_exec
	${SRC_FILES}
	src/main_console.cpp
)
