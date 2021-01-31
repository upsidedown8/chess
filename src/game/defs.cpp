#include "defs.hpp"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>

/* -------------------------------------------------------------------------- */
/*                                   Tables                                   */
/* -------------------------------------------------------------------------- */
chess_cpp::U64 chess_cpp::RANKS[256];
chess_cpp::U64 chess_cpp::FILES[256];
chess_cpp::U64 chess_cpp::NOT_RANKS[256];
chess_cpp::U64 chess_cpp::NOT_FILES[256];

/* -------------------------------------------------------------------------- */
/*                                Attack tables                               */
/* -------------------------------------------------------------------------- */
chess_cpp::U64 chess_cpp::KNIGHT_MOVES[64];
chess_cpp::U64 chess_cpp::KING_MOVES[64];

/* -------------------------------------------------------------------------- */
/*                                   Magics                                   */
/* -------------------------------------------------------------------------- */
chess_cpp::U64 chess_cpp::ROOK_MASKS[NUM_SQUARES];
chess_cpp::U64 chess_cpp::BISHOP_MASKS[NUM_SQUARES];

chess_cpp::U64 chess_cpp::ROOK_MAGICS[NUM_SQUARES] {
    72075735983988992ULL,
    162164771226042368ULL,
    2774234964794286080ULL,
    9295447227374240800ULL,
    7133704077631881220ULL,
    5404321769049293056ULL,
    13871089051341160576ULL,
    4647732546161868928ULL,
    1154188151204364296ULL,
    281623304421378ULL,
    9585349132126560768ULL,
    324399945019818112ULL,
    1266654575591552ULL,
    294422971669283848ULL,
    9228016932324638976ULL,
    422213622698112ULL,
    18019346383143456ULL,
    13519870926790656ULL,
    6917743432679031040ULL,
    4611968593184169992ULL,
    12170978542791720968ULL,
    144159173373870084ULL,
    73228578216739328ULL,
    2199036100765ULL,
    56330731617533952ULL,
    148619063654883328ULL,
    4625232012420055168ULL,
    14988261623278407680ULL,
    1478588125675784194ULL,
    577024260602875912ULL,
    2468254118020653568ULL,
    144256209032118404ULL,
    40577751509369480ULL,
    6917564213158219778ULL,
    9007478444400656ULL,
    20839044434890752ULL,
    4611976300242928640ULL,
    4617878489423415312ULL,
    11278859869620225ULL,
    288230653210657060ULL,
    576531123197214720ULL,
    844699816624161ULL,
    4616198431329755136ULL,
    1513221569692893216ULL,
    12125942013883416584ULL,
    4613005570896036100ULL,
    72066394459734032ULL,
    1765429764459462660ULL,
    342291713626218624ULL,
    22518273021051200ULL,
    9464597434109056ULL,
    613052534176650752ULL,
    20547690614100224ULL,
    140746078552192ULL,
    45044801233552384ULL,
    27028749086179840ULL,
    290556685111457ULL,
    288865903000617090ULL,
    1161084417409045ULL,
    289075918041778209ULL,
    2522578810537804930ULL,
    1298444514277720065ULL,
    1143496522109444ULL,
    2305843716071555138ULL
};
chess_cpp::U64 chess_cpp::BISHOP_MAGICS[NUM_SQUARES] {
    1179020146311185ULL,
    145267478427205635ULL,
    4504158111531524ULL,
    9224516499644878888ULL,
    144680405855912002ULL,
    4619005622497574912ULL,
    1130315234418688ULL,
    5349125176573952ULL,
    6071010655858065920ULL,
    20310248111767713ULL,
    1297094009090539520ULL,
    4616233778910625860ULL,
    2305849615159678976ULL,
    74381998193642242ULL,
    1407684255942661ULL,
    2305862803678299144ULL,
    22535635693734016ULL,
    4503608284938884ULL,
    11259016393073153ULL,
    108650578499878976ULL,
    41095363813851170ULL,
    9232520132522148096ULL,
    70385943187776ULL,
    9227035893351617024ULL,
    1155182103739172867ULL,
    11530343153862181120ULL,
    2295791083930624ULL,
    1130297991168512ULL,
    281543712980996ULL,
    307513611096490433ULL,
    2289183226103316ULL,
    4612816874811392128ULL,
    4547891544985604ULL,
    3458958372559659520ULL,
    303473866573824ULL,
    1729558217427519744ULL,
    5633914760597520ULL,
    1441434463836899328ULL,
    20269028707403544ULL,
    149744981853258752ULL,
    2252933819802113ULL,
    1163074498090533888ULL,
    4681729134575680ULL,
    4621485970984798208ULL,
    367078571518203970ULL,
    72621098075685120ULL,
    1225544256278495744ULL,
    1411779381045761ULL,
    5333500077688291352ULL,
    4716913491968128ULL,
    148627764202701056ULL,
    1688850967695425ULL,
    17781710002178ULL,
    9243644149415084036ULL,
    218426849703891488ULL,
    9009415596316677ULL,
    1412882374067224ULL,
    279186509824ULL,
    20407489916899328ULL,
    4614113755159331840ULL,
    144119586390940160ULL,
    11547234118442230016ULL,
    5188151323463779840ULL,
    435758450535334272ULL
};

chess_cpp::U8 chess_cpp::ROOK_MAGIC_SHIFTS[NUM_SQUARES];
chess_cpp::U8 chess_cpp::BISHOP_MAGIC_SHIFTS[NUM_SQUARES];

chess_cpp::U64 chess_cpp::ROOK_MOVES[NUM_SQUARES][4096];
chess_cpp::U64 chess_cpp::BISHOP_MOVES[NUM_SQUARES][4096];

const chess_cpp::U8 LSB_64_TABLE[NUM_SQUARES] {
    63, 30,  3, 32, 25, 41, 22, 33,
    15, 50, 42, 13, 11, 53, 19, 34,
    61, 29,  2, 51, 21, 43, 45, 10,
    18, 47,  1, 54,  9, 57,  0, 35,
    62, 31, 40,  4, 49,  5, 52, 26,
    60,  6, 23, 44, 46, 27, 56, 16,
     7, 39, 48, 24, 59, 14, 12, 55,
    38, 28, 58, 20, 37, 17, 36,  8
};

// tables
chess_cpp::U64 SET_BIT_TABLE[NUM_SQUARES];
chess_cpp::U64 CLEAR_BIT_TABLE[NUM_SQUARES];

/* -------------------------------------------------------------------------- */
/*                            Initialize all tables                           */
/* -------------------------------------------------------------------------- */
chess_cpp::U64 gen_rook_mask(int start) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1; r>=1; r--) result |= (1ULL<<chess_cpp::calc_pos(r,file));
    for (int r=rank+1; r<=6; r++) result |= (1ULL<<chess_cpp::calc_pos(r,file));
    for (int f=file-1; f>=1; f--) result |= (1ULL<<chess_cpp::calc_pos(rank,f));
    for (int f=file+1; f<=6; f++) result |= (1ULL<<chess_cpp::calc_pos(rank,f));

    return result;
}
chess_cpp::U64 gen_bishop_mask(int start) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1, f=file-1; r>=1&&f>=1; r--, f--) result |= (1ULL<<chess_cpp::calc_pos(r,f));
    for (int r=rank+1, f=file-1; r<=6&&f>=1; r++, f--) result |= (1ULL<<chess_cpp::calc_pos(r,f));
    for (int r=rank-1, f=file+1; r>=1&&f<=6; r--, f++) result |= (1ULL<<chess_cpp::calc_pos(r,f));
    for (int r=rank+1, f=file+1; r<=6&&f<=6; r++, f++) result |= (1ULL<<chess_cpp::calc_pos(r,f));

    return result;
}
chess_cpp::U64 gen_rook_moves(int start, chess_cpp::U64 occupancy) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1; r>=0; r--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,file));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,file)))
            break;
    }
    for (int r=rank+1; r<=7; r++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,file));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,file)))
            break;
    }
    for (int f=file-1; f>=0; f--) {
        result |= (1ULL<<chess_cpp::calc_pos(rank,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(rank,f)))
            break;
    }
    for (int f=file+1; f<=7; f++) {
        result |= (1ULL<<chess_cpp::calc_pos(rank,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(rank,f)))
            break;
    }

    return result;
}
chess_cpp::U64 gen_bishop_moves(int start, chess_cpp::U64 occupancy) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1, f=file-1; r>=0&&f>=0; r--, f--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank+1, f=file-1; r<=7&&f>=0; r++, f--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank-1, f=file+1; r>=0&&f<=7; r--, f++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank+1, f=file+1; r<=7&&f<=7; r++, f++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }

    return result;
}
chess_cpp::U64 random_u64() {
    return
       ((rand() & 0xffffULL)      ) |
       ((rand() & 0xffffULL) << 16) |
       ((rand() & 0xffffULL) << 32) |
       ((rand() & 0xffffULL) << 48);
}
chess_cpp::U64 idx_to_u64(int idx, chess_cpp::U64 mask) {
    chess_cpp::U64 result = 0;

    int i = 0;
    chess_cpp::U8 pos;
    while (idx&&mask) {
        pos = chess_cpp::pop_lsb(mask);
        if (idx & (1<<i))
            result |= SET_BIT_TABLE[pos];
        i++;
    }

    return result;
}

chess_cpp::U64 find_magic(int start, bool isBishop) {
    // Credit: https://www.chessprogramming.org/Looking_for_Magics

    chess_cpp::U64 moves[4096];
    chess_cpp::U64 indexed_mask[4096];
    chess_cpp::U64 mask = isBishop
        ? chess_cpp::BISHOP_MASKS[start]
        : chess_cpp::ROOK_MASKS[start];
    int numBits = chess_cpp::count_occupied(mask);
    int numPositions = 1<<numBits;
    int shift = 64 - numBits;

    // pregenerate the indexed masks and the moves
    for (int idx = 0; idx < 4096; idx++) {
        indexed_mask[idx] = idx_to_u64(idx, mask);
        moves[idx] = isBishop
            ? gen_bishop_moves(start, indexed_mask[idx])
            : gen_rook_moves(start, indexed_mask[idx]);
    }

    // loop until a magic is found
    chess_cpp::U64 moves_map[4096];
    while (true) {
        // reset the moves map
        std::fill_n(moves_map, 4096, 0);

        chess_cpp::U64 magic = random_u64() & random_u64() & random_u64();
        if (chess_cpp::count_occupied((mask*magic)&chess_cpp::RANKS[chess_cpp::Rank1]) < numBits/2)
            continue;

        // verify the magic
        bool verified = true;
        for (int idx = 0; idx < 4096; idx++) {
            int key = (indexed_mask[idx]*magic) >> shift;

            // check for invalid collision
            if (!moves_map[key]) {
                moves_map[key] = moves[idx];
            } else if (moves_map[key] != moves[idx]) {
                verified = false;
                break;
            }
        }

        if (verified) return magic;
    }
}
void find_all_magics() {
    srand(time(NULL));

    // rook magics
    std::cout << "===============\n";
    std::cout << "  Rook magics  \n";
    std::cout << "===============\n";
    std::cout << "chess_cpp::U64 rookMagics[NUM_SQUARES] = {\n";
    for (int i = 0; i < NUM_SQUARES; i++) {
        chess_cpp::ROOK_MAGICS[i] = find_magic(i, false);
        std::cout << "    " << chess_cpp::ROOK_MAGICS[i] << "ULL";
        if (i < 63) std::cout << ",";
        std::cout << std::endl;
    }
    std::cout << "};\n";
    // bishop magics
    std::cout << "===============\n";
    std::cout << " Bishop magics \n";
    std::cout << "===============\n";
    std::cout << "chess_cpp::U64 bishopMagics[NUM_SQUARES] = {\n";
    for (int i = 0; i < NUM_SQUARES; i++) {
        chess_cpp::BISHOP_MAGICS[i] = find_magic(i, true);
        std::cout << "    " << chess_cpp::BISHOP_MAGICS[i] << "ULL";
        if (i < 63) std::cout << ",";
        std::cout << std::endl;
    }
    std::cout << "};\n";
}

chess_cpp::U64 gen_king_moves(int start) {
    chess_cpp::U64 result = 0;
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if ((dx != 0 || dy != 0) &&
                rank+dx >= 0 && rank+dx <= 7 &&
                file+dy >= 0 && file+dy <= 7) {

                chess_cpp::set_pos(result, chess_cpp::calc_pos(rank+dx, file+dy));
            }
        }
    }
    return result;
}
chess_cpp::U64 gen_knight_moves(int start) {
    chess_cpp::U64 result = 0;
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    int moveVectors[8][2] {
        { -1,  2 },
        { -2,  1 },
        {  1,  2 },
        {  2,  1 },
        { -1, -2 },
        { -2, -1 },
        {  1, -2 },
        {  2, -1 }
    };
    for (int i = 0; i < 8; i++) {
        int dx = moveVectors[i][0];
        int dy = moveVectors[i][1];
        if ((dx != 0 || dy != 0) &&
            rank+dx >= 0 && rank+dx <= 7 &&
            file+dy >= 0 && file+dy <= 7) {

            chess_cpp::set_pos(result, chess_cpp::calc_pos(rank+dx, file+dy));
        }
    }
    return result;
}

void chess_cpp::init() {
    // set bit table & clear bit table
    for (size_t i = 0; i < NUM_SQUARES; i++) {
        SET_BIT_TABLE[i] = 1ULL << i;
        CLEAR_BIT_TABLE[i] = ~SET_BIT_TABLE[i];
    }

    // ranks
    for (int i = 0; i < 8; i++)
        RANKS[1<<i] = 0xffULL<<(8*(7-i));
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 8; j++)
            if (i & (1<<j))
                RANKS[i] |= RANKS[1<<j];
        NOT_RANKS[i] = ~RANKS[i];
    }

    // files
    for (int i = 0; i < 8; i++)
        FILES[1<<i] = 0x0101010101010101UL<<i;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 8; j++)
            if (i & (1<<j))
                FILES[i] |= FILES[1<<j];
        NOT_FILES[i] = ~FILES[i];
    }

    // rook masks
    for (int i = 0; i < NUM_SQUARES; i++)
        ROOK_MASKS[i] = gen_rook_mask(i);

    // bishop masks
    for (int i = 0; i < NUM_SQUARES; i++)
        BISHOP_MASKS[i] = gen_bishop_mask(i);
    
    // rook magic shifts
    for (int i = 0; i < NUM_SQUARES; i++)
        ROOK_MAGIC_SHIFTS[i] = 64 - count_occupied(ROOK_MASKS[i]);

    // bishop magic shifts
    for (int i = 0; i < NUM_SQUARES; i++)
        BISHOP_MAGIC_SHIFTS[i] = 64 - count_occupied(BISHOP_MASKS[i]);

    // used to generate new magics
    // find_all_magics();

    for (int sq = 0; sq < NUM_SQUARES; sq++) {
        // generate rook move tables
        U64 rookMask = ROOK_MASKS[sq];
        for (int idx = (1<<count_occupied(rookMask))-1; idx >= 0; idx--) {
            U64 indexed_mask = idx_to_u64(idx, rookMask);
            int key = (ROOK_MAGICS[sq]*indexed_mask) >> ROOK_MAGIC_SHIFTS[sq];
            ROOK_MOVES[sq][key] = gen_rook_moves(sq, indexed_mask);
        }

        // generate bishop move tables
        U64 bishopMask = BISHOP_MASKS[sq];
        for (int idx = (1<<count_occupied(rookMask))-1; idx >= 0; idx--) {
            U64 indexed_mask = idx_to_u64(idx, bishopMask);
            int key = (BISHOP_MAGICS[sq]*indexed_mask) >> BISHOP_MAGIC_SHIFTS[sq];
            BISHOP_MOVES[sq][key] = gen_bishop_moves(sq, indexed_mask);
        }
    }

    // generate knight moves
    for (int sq = 0; sq < NUM_SQUARES; sq++)
        KNIGHT_MOVES[sq] = gen_knight_moves(sq);

    // generate king moves
    for (int sq = 0; sq < NUM_SQUARES; sq++)
        KING_MOVES[sq] = gen_king_moves(sq);

}

/* -------------------------------------------------------------------------- */
/*                                   Strings                                  */
/* -------------------------------------------------------------------------- */
std::string chess_cpp::bb_to_string(U64 bitboard) {
    U64 mask = 1;

    std::stringstream ss;

    for (int i = 0; i < 8; i++) {
        ss << (8-i) << ' ';
        for (int j = 0; j < 8; j++) {
            ss << (bitboard & mask ? 'x' : '.') << ' ';
            mask <<= 1;
        }
        ss << std::endl;
    }
    ss << "  a b c d e f g h";

    return ss.str();
}

/* -------------------------------------------------------------------------- */
/*                            Rank/File to Position                           */
/* -------------------------------------------------------------------------- */
chess_cpp::U8 chess_cpp::calc_pos(int rank, int file) {
    return rank * 8 + file;
}
void chess_cpp::calc_rf(U8 pos, int &rank, int &file) {
    file = pos % 8;
    rank = pos / 8;
}

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
bool chess_cpp::is_set(const U64 &board, U8 pos) {
    return board & SET_BIT_TABLE[pos];
}
void chess_cpp::set_pos(U64 &board, U8 pos) {
    board |= SET_BIT_TABLE[pos];
}
void chess_cpp::clr_pos(U64 &board, U8 pos) {
    board &= CLEAR_BIT_TABLE[pos];
}
chess_cpp::U8 chess_cpp::count_occupied(U64 board) {
    U8 count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
chess_cpp::U8 chess_cpp::pop_lsb(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}
