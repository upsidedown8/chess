#include "movegen.hpp"

#include <stdexcept>

using namespace chess_cpp;

/* -------------------------------------------------------------------------- */
/*                                  MoveList                                  */
/* -------------------------------------------------------------------------- */
MoveList::MoveList()
    : ptr(moves) {}

size_t MoveList::size() {
    return ptr - moves;
}

/* -------------------------------------------------------------------------- */
/*                               Move Generation                              */
/* -------------------------------------------------------------------------- */
inline U64 get_bishop_moves(int sq, U64 occupancy) {
    occupancy &= BISHOP_MASKS[sq];
    int idx = (BISHOP_MAGICS[sq]*occupancy)>>BISHOP_MAGIC_SHIFTS[sq];
    return BISHOP_MOVES[sq][idx];
}
inline U64 get_rook_moves(int sq, U64 occupancy) {
    occupancy &= ROOK_MASKS[sq];
    int idx = (ROOK_MAGICS[sq]*occupancy)>>ROOK_MAGIC_SHIFTS[sq];
    return ROOK_MOVES[sq][idx];
}

/* -------------------------------------------------------------------------- */
/*                                  Attackers                                 */
/* -------------------------------------------------------------------------- */
inline U64 get_attackers(int square, Board &board, U64 occupancy) {
    int color = board.white_to_move ? WHITE : BLACK;
    // rooks, bishops and queens
    U64 attackingPieces =
        (get_bishop_moves(square, occupancy) |
         get_rook_moves(square, occupancy))
            &
        (board.bitboards[board.enemy_color() | Bishop] |
         board.bitboards[board.enemy_color() | Rook] |
         board.bitboards[board.enemy_color() | Queen]);
    // knights
    attackingPieces |=
        (KNIGHT_MOVES[square] & board.bitboards[board.enemy_color() | Knight]);
    // kings
    attackingPieces |=
        (KING_MOVES[square] & board.bitboards[board.enemy_color() | King]);
    // pawns
    attackingPieces |=
        (PAWN_ATTACKS[color][square] & board.bitboards[board.enemy_color() | Pawn]);
    return attackingPieces;
}
inline bool is_under_attack(int square, Board &board, U64 occupancy) {
    // knights
    if (KNIGHT_MOVES[square] & board.bitboards[board.enemy_color() | Knight])
        return true;
    // kings
    if (KING_MOVES[square] & board.bitboards[board.enemy_color() | King])
        return true;
    // pawns
    if (PAWN_ATTACKS[board.white_to_move?BLACK:WHITE][square] & board.bitboards[board.enemy_color() | Pawn])
        return true;

    // rooks, bishops and queens
    return
        (get_bishop_moves(square, occupancy) |
         get_rook_moves(square, occupancy))
            &
        (board.bitboards[board.enemy_color() | Bishop] |
         board.bitboards[board.enemy_color() | Rook] |
         board.bitboards[board.enemy_color() | Queen]);
}

// pawn: pins
void gen_pawn_moves(Board &board, Move *&ptr, U64 occupancy, U64 legalCaptures = FULL_BB, U64 blockers = FULL_BB) {
    U64 enemy = board.bitboards[board.enemy_color() | All];
    U64 pawns = board.bitboards[board.active_color() | Pawn];
    
    int end;
    int offset = board.white_to_move
        ?  8
        : -8;
    int doubleOffset = offset << 1;

    // Single square moves
    // find any single square forward moves that do not collide
    U64 pawnSingleMoves = board.white_to_move
        ? pawns >> 8
        : pawns << 8;
    pawnSingleMoves &= ~occupancy;

    // Non-Promotion moves
    // find any single square moves that do not end on the last rank
    U64 nonPromotionMoves = blockers & pawnSingleMoves & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionMoves) {
        end = pop_lsb(nonPromotionMoves);
        *(ptr++) = Move(offset+end, end, 0);
    }

    // Promotion moves
    // find any single square moves that end on the last rank
    U64 promotionMoves = blockers & pawnSingleMoves & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionMoves) {
        end = pop_lsb(promotionMoves);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
    }

    // Double square moves
    // find pawns that originated from the start square, and are already validated for 1 square move.
    pawnSingleMoves &= RANKS[board.white_to_move ? Rank3 : Rank6];
    // of the single moves that were valid, move them one more square and check if they are valid.
    U64 pawnDoubleMoves = board.white_to_move
        ? pawnSingleMoves >> 8
        : pawnSingleMoves << 8;
    pawnDoubleMoves &= ~occupancy & blockers;
    while (pawnDoubleMoves) {
        end = pop_lsb(pawnDoubleMoves);
        *(ptr++) = Move(doubleOffset+end, end, 0);
    }

    // Captures
    U64 enPassantCaptures, enPassantSquare = 1ULL << board.en_passant;
    int enPassantEnd = board.en_passant + (board.white_to_move ? 8 : -8);
    U64 kingBitBoard = board.bitboards[board.active_color() | King];
    int kingPos = pop_lsb(kingBitBoard);

    U64 captures, nonPromotionCaps, promotionCaps;
    // any pawn not on file A could potentially left capture
    offset = board.white_to_move
        ?  9
        : -7;
    captures = board.white_to_move
        ? (pawns & NOT_FILES[FileA]) >> 9
        : (pawns & NOT_FILES[FileA]) << 7;
    captures &= legalCaptures;
    // Check for left capture En Passant
    enPassantCaptures = captures & enPassantSquare;
    if (enPassantCaptures) {
        clr_pos(board.bitboards[board.enemy_color() | Pawn], board.en_passant);
        clr_pos(board.bitboards[board.active_color() | Pawn], offset+board.en_passant);
        set_pos(board.bitboards[board.active_color() | Pawn], enPassantEnd);
        if (!is_under_attack(kingPos, board, occupancy)) // check for discovered attack
            *(ptr++) = Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT);
        set_pos(board.bitboards[board.enemy_color() | Pawn], board.en_passant);
        set_pos(board.bitboards[board.active_color() | Pawn], offset+board.en_passant);
        clr_pos(board.bitboards[board.active_color() | Pawn], enPassantEnd);
    }
    captures &= enemy;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        *(ptr++) = Move(end+offset, end, 0);
    }
    // Promotion captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
    }

    // any pawn not on file H could potentially right capture
    offset = board.white_to_move
        ?  7
        : -9;
    captures = board.white_to_move
        ? (pawns & NOT_FILES[FileH]) >> 7
        : (pawns & NOT_FILES[FileH]) << 9;
    captures &= legalCaptures;
    // Check for right capture En Passant
    enPassantCaptures = captures & enPassantSquare;
    if (enPassantCaptures) {
        clr_pos(board.bitboards[board.enemy_color() | Pawn], board.en_passant);
        clr_pos(board.bitboards[board.active_color() | Pawn], offset+board.en_passant);
        set_pos(board.bitboards[board.active_color() | Pawn], enPassantEnd);
        if (!is_under_attack(kingPos, board, occupancy)) // check for discovered attack
            *(ptr++) = Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT);
        set_pos(board.bitboards[board.enemy_color() | Pawn], board.en_passant);
        set_pos(board.bitboards[board.active_color() | Pawn], offset+board.en_passant);
        clr_pos(board.bitboards[board.active_color() | Pawn], enPassantEnd);
    }
    captures &= enemy;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        *(ptr++) = Move(end+offset, end, 0);
    }
    // Promotion Captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *(ptr++) = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
    }
}
// knights: pins
void gen_knight_moves(Board &board, Move *&ptr, U64 occupancy, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    U64 knights = board.bitboards[board.active_color() | Knight];
    int start;
    while (knights) {
        start = pop_lsb(knights);

        U64 knightsMoves = KNIGHT_MOVES[start] & nonFriendly & movesMask;

        while (knightsMoves)
            *(ptr++) = Move(start, pop_lsb(knightsMoves), 0);
    }
}
// bishops: pins
void gen_bishop_moves(Board &board, Move *&ptr, U64 occupancy, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    U64 bishops = board.bitboards[board.active_color() | Bishop];
    U64 bishopMoves;
    int start;

    while (bishops) {
        start = pop_lsb(bishops);

        // generate legal moves
        bishopMoves = get_bishop_moves(start, occupancy) & nonFriendly & movesMask;

        while (bishopMoves)
            *(ptr++) = Move(start, pop_lsb(bishopMoves), 0);
    }
}
// rooks: pins
void gen_rook_moves(Board &board, Move *&ptr, U64 occupancy, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    U64 rooks = board.bitboards[board.active_color() | Rook];
    U64 rookMoves;
    int start;

    while (rooks) {
        start = pop_lsb(rooks);

        rookMoves = get_rook_moves(start, occupancy) & nonFriendly & movesMask;

        while (rookMoves)
            *(ptr++) = Move(start, pop_lsb(rookMoves), 0);
    }
}
// queens: pins
void gen_queen_moves(Board &board, Move *&ptr, U64 occupancy, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    U64 queens = board.bitboards[board.active_color() | Queen];
    U64 queenMoves;
    int start;

    while (queens) {
        start = pop_lsb(queens);

        queenMoves =
            get_rook_moves(start, occupancy) |
            get_bishop_moves(start, occupancy);
        queenMoves &= nonFriendly & movesMask;

        while (queenMoves)
            *(ptr++) = Move(start, pop_lsb(queenMoves), 0);
    }
}
void gen_king_moves(Board &board, Move *&ptr, U64 occupancy) {
    U64 kings = board.bitboards[board.active_color() | King];
    int start = pop_lsb(kings), end;

    U64 kingMoves = KING_MOVES[start] & ~board.bitboards[board.active_color() | All];

    while (kingMoves) {
        end = pop_lsb(kingMoves);
        if (!is_under_attack(end, board, occupancy))
            *(ptr++) = Move(start, end, 0);
    }
}
void gen_castling(Board &board, Move *&ptr, U64 occupancy) {
    // a square is blocked if it has a piece on it or is under attack
    if (board.white_to_move) {
        if (board.castling & WHITE_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileB | FileC | FileD]) ||
                  is_under_attack(e1, board, occupancy) ||
                  is_under_attack(d1, board, occupancy) ||
                  is_under_attack(c1, board, occupancy))) {
                    *(ptr++) = Move(e1, c1, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & WHITE_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileF | FileG]) ||
                  is_under_attack(e1, board, occupancy) ||
                  is_under_attack(f1, board, occupancy) ||
                  is_under_attack(g1, board, occupancy))) {
                *(ptr++) = Move(e1, g1, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    }
    else {
        if (board.castling & BLACK_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileB | FileC | FileD]) ||
                  is_under_attack(e8, board, occupancy) ||
                  is_under_attack(d8, board, occupancy) ||
                  is_under_attack(c8, board, occupancy))) {
                *(ptr++) = Move(e8, c8, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & BLACK_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileF | FileG]) ||
                  is_under_attack(e8, board, occupancy) ||
                  is_under_attack(f8, board, occupancy) ||
                  is_under_attack(g8, board, occupancy))) {
                *(ptr++) = Move(e8, g8, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    }
}

MoveList chess_cpp::gen_moves(Board &board) {
    U64 occupancy = board.bitboards[White | All] | board.bitboards[Black | All];
    MoveList moves;

    // ==> calculate pinned pieces <==

    // always generate king moves first
    gen_king_moves(board, moves.ptr, occupancy);
    
    // calculate pieces giving check
    U64 enemy = board.bitboards[board.enemy_color() | All];
    U64 kingBitBoard = board.bitboards[board.active_color() | King];
    int kingPos = pop_lsb(kingBitBoard);
    U64 occupancyNoKing = occupancy & ~board.bitboards[board.active_color() | King];
    U64 attackingKing = get_attackers(kingPos, board, occupancyNoKing);

    switch (count_occupied(attackingKing)) {
        // double check
        case 2: {
            // king moves are the only option, already calculated
            break;
        }
        // single check
        case 1: {
            U64 captures = attackingKing;

            int attackerPos = pop_lsb(attackingKing);
            U64 blockers = (board.pieces[attackerPos]&0b111) <= Knight
                ? 0ULL
                : SLIDER_RANGE[kingPos][attackerPos];

            U64 movesMask = captures | blockers;

            gen_pawn_moves(board, moves.ptr, occupancy, captures, blockers);
            gen_knight_moves(board, moves.ptr, occupancy, movesMask);
            gen_bishop_moves(board, moves.ptr, occupancy, movesMask);
            gen_rook_moves(board, moves.ptr, occupancy, movesMask);
            gen_queen_moves(board, moves.ptr, occupancy, movesMask);
            break;
        }
        // not in check - standard move generation
        case 0: {
            gen_castling(board, moves.ptr, occupancy);
            gen_pawn_moves(board, moves.ptr, occupancy);
            gen_knight_moves(board, moves.ptr, occupancy);
            gen_bishop_moves(board, moves.ptr, occupancy);
            gen_rook_moves(board, moves.ptr, occupancy);
            gen_queen_moves(board, moves.ptr, occupancy);
            break;
        }
        default: {
            throw std::runtime_error("Invalid number of attackers on the king");
        }
    }

    return moves;
}
