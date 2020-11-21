#include "game/board.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
    Board board;

    std::cout << board.to_string() << std::endl;

    return 0;
}
