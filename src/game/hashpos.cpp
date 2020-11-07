#include "game/hashpos.hpp"

HashPos::HashPos() {
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        for (int j = 0; j < 12; j++) {
            // ensure the random number fills all 64 bits
            randTable[i][j] =
                ((U64)rand() & 0x7fff)       +
                ((U64)rand() & 0x7fff) << 15 + 
                ((U64)rand() & 0x7fff) << 30 + 
                ((U64)rand() & 0x7fff) << 45 +
                ((U64)rand() & 0xf)    << 60;
        }
    }
}

U64 HashPos::hash(Board &board) {
    U64 h = 0ULL;
    for (uchar i = 0; i < NUMBER_OF_SQUARES; i++) {
        if (BitBoardUtils::isSet(board.PIECES, i)) {
            int j = the piece at board[i];
            h ^= randTable[i][j];
        }
    }
    return h;
}
