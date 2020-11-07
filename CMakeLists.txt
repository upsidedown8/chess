cmake_minimum_required(VERSION 3.0)
project(chess)

set(CMAKE_BINARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 14)

include_directories(
    src/
    inc/
)

add_library(
    chess
    
)
