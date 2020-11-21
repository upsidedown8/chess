#if !defined(CHESS_GAME_BOARD_HPP)
#define CHESS_GAME_BOARD_HPP

#include <cstdint>
#include <string>

typedef uint64_t U64;
typedef unsigned char uchar;

#define FILE_1 0x0101010101010101UL
#define FILE_2 0x0202020202020202UL
#define FILE_3 0x0404040404040404UL
#define FILE_4 0x0808080808080808UL
#define FILE_5 0x1010101010101010UL
#define FILE_6 0x2020202020202020UL
#define FILE_7 0x4040404040404040UL
#define FILE_8 0x8080808080808080UL

#define RANK_A 0x00000000000000ffUL
#define RANK_B 0x000000000000ff00UL
#define RANK_C 0x0000000000ff0000UL
#define RANK_D 0x00000000ff000000UL
#define RANK_E 0x000000ff00000000UL
#define RANK_F 0x0000ff0000000000UL
#define RANK_G 0x00ff000000000000UL
#define RANK_H 0xff00000000000000UL

#define NUM_SQUARES 64

enum Pieces {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    EMPTY
};

enum Colors {
    WHITE,
    BLACK,
    ALL,
    NONE
};

class Board {
private:
    // Zobrist hashing
    U64 m_rand_table[64][12];
    U64 m_set_bit_table[NUM_SQUARES];
    U64 m_clr_bit_table[NUM_SQUARES];

    U64 hash();

    char get_notation(Pieces piece, Colors color);
    void from_notation(char c, Pieces *piece, Colors *color);

    void init_tables();
    void zero_boards();
    void update_bitboards();
    void from_string(const std::string &str);

    bool is_set(const U64 &board, uchar pos);

    void set_pos(U64 &board, uchar pos);
    void clr_pos(U64 &board, uchar pos);

    uchar count_occupied(U64 board);
    uchar pop_lsb(U64 &board);

    std::string rle_encode(const std::string& text);
    std::string rle_decode(const std::string& text);

public:
    U64 m_bitboards[3][6];
    Pieces m_pieces[64];

    size_t m_en_passant, m_fifty_move;
    size_t m_king_pos[2];

    Colors m_current_move;

    Board();
    Board(const std::string &str);

    void reset();

    std::string to_string();
};

#endif // CHESS_GAME_BOARD_HPP
