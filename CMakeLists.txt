cmake_minimum_required(VERSION 3.0)
project(chess_cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

include_directories(
	src/
)

add_library(
	chess_lib
	src/game/board.cpp
	src/game/defs.cpp
	src/game/move.cpp
	src/game/movegen.cpp
)

add_executable(
	chess
	src/main.cpp
)
target_link_libraries(
	chess
	chess_lib
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(
	tests
	test/tests.cpp
)
target_link_libraries(
	tests
	chess_lib
	${GTEST_LIBRARIES}
    pthread
)