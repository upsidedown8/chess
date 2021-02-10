#include "movegen.hpp"

#include <stdexcept>
#include <assert.h>

#define ADD_MOVE_FLAGS(ptr, start, end, flags) \
    *ptr++ = CREATE_MOVE(start, end, flags);

#define ADD_MOVE(ptr, start, end) \
    ADD_MOVE_FLAGS(ptr, start, end, 0);

#define ADD_PROMOTION(ptr, start, end) \
    ADD_MOVE_FLAGS(ptr, start, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN); \
    ADD_MOVE_FLAGS(ptr, start, end, MOVETYPE_PROMOTION | MOVEPROMOTION_ROOK); \
    ADD_MOVE_FLAGS(ptr, start, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP); \
    ADD_MOVE_FLAGS(ptr, start, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);

using namespace chess_cpp;

/* -------------------------------------------------------------------------- */
/*                               Move Generation                              */
/* -------------------------------------------------------------------------- */
inline U64 magic_bishop_moves(int sq, U64 occupancy) {
    occupancy &= BISHOP_MASKS[sq];
    int idx = (BISHOP_MAGICS[sq]*occupancy)>>BISHOP_MAGIC_SHIFTS[sq];
    return BISHOP_MOVES[sq][idx];
}
inline U64 magic_rook_moves(int sq, U64 occupancy) {
    occupancy &= ROOK_MASKS[sq];
    int idx = (ROOK_MAGICS[sq]*occupancy)>>ROOK_MAGIC_SHIFTS[sq];
    return ROOK_MOVES[sq][idx];
}
inline U64 magic_queen_moves(int sq, U64 occupancy) {
    return magic_bishop_moves(sq, occupancy) | magic_rook_moves(sq, occupancy);
}

/* -------------------------------------------------------------------------- */
/*                                  Attackers                                 */
/* -------------------------------------------------------------------------- */
template<Colors FRIENDLY>
inline U64 find_enemy_attackers(int sq, Board &board, U64 occupancy) {
    int color = FRIENDLY == White ? WHITE : BLACK;
    Colors ENEMY = !FRIENDLY;

    // rooks, bishops and queens
    U64 bishopQueen = board.bitboards[ENEMY|Bishop] | board.bitboards[ENEMY|Queen];
    U64 rookQueen   = board.bitboards[ENEMY|Rook  ] | board.bitboards[ENEMY|Queen];

    U64 attackingPieces =
        (magic_bishop_moves(sq, occupancy) & bishopQueen) |
        (magic_rook_moves(sq, occupancy) & rookQueen);
    
    // knights
    attackingPieces |= (       KNIGHT_MOVES[sq] & board.bitboards[ENEMY | Knight]);
    attackingPieces |= (         KING_MOVES[sq] & board.bitboards[ENEMY | King  ]);
    attackingPieces |= (PAWN_ATTACKS[color][sq] & board.bitboards[ENEMY | Pawn  ]);

    return attackingPieces;
}
template<Colors FRIENDLY>
inline bool is_under_attack(int sq, Board &board, U64 occupancy) {
    Colors ENEMY = !FRIENDLY;
    int color = FRIENDLY == White ? WHITE : BLACK;

    if (       KNIGHT_MOVES[sq] & board.bitboards[ENEMY|Knight]) return true;
    if (         KING_MOVES[sq] & board.bitboards[ENEMY|King  ]) return true;
    if (PAWN_ATTACKS[color][sq] & board.bitboards[ENEMY|Pawn  ]) return true;

    U64 bishopQueen = board.bitboards[ENEMY|Bishop] | board.bitboards[ENEMY|Queen];
    if (magic_bishop_moves(sq, occupancy) & bishopQueen) return true;

    U64 rookQueen   = board.bitboards[ENEMY|Rook  ] | board.bitboards[ENEMY|Queen];
    return magic_rook_moves(sq, occupancy) & rookQueen;
}

/* -------------------------------------------------------------------------- */
/*                                 En Passant                                 */
/* -------------------------------------------------------------------------- */
template<Colors FRIENDLY>
bool validate_en_passant(Board &board, int kingPos, int start, int end, U64 occupancy) {
    // is_under_attack() only uses bitboards
    Colors ENEMY = !FRIENDLY;

    // update pawn bitboards
    clr_pos(board.bitboards[ENEMY|Pawn], end);
    clr_pos(board.bitboards[FRIENDLY|Pawn], start);
    set_pos(board.bitboards[FRIENDLY|Pawn], board.en_passant);
    // update global bitboards
    clr_pos(board.bitboards[ENEMY|All], end);
    clr_pos(board.bitboards[FRIENDLY|All], start);
    set_pos(board.bitboards[FRIENDLY|All], board.en_passant);

    clr_pos(occupancy, start);
    set_pos(occupancy, board.en_passant);
    clr_pos(occupancy, end);

    bool result = !is_under_attack<FRIENDLY>(kingPos, board, occupancy);

    // update pawn bitboards
    set_pos(board.bitboards[ENEMY|Pawn], end);
    set_pos(board.bitboards[FRIENDLY|Pawn], start);
    clr_pos(board.bitboards[FRIENDLY|Pawn], board.en_passant);
    // update global bitboards
    set_pos(board.bitboards[ENEMY|All], end);
    set_pos(board.bitboards[FRIENDLY|All], start);
    clr_pos(board.bitboards[FRIENDLY|All], board.en_passant);

    return result;
}

/* -------------------------------------------------------------------------- */
/*                                    Moves                                   */
/* -------------------------------------------------------------------------- */
template<Colors FRIENDLY, bool isLeftCapture>
void gen_pawn_captures(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 legalCaptures, int kingPos) {
    Colors ENEMY = !FRIENDLY;

    U64 enemy = board.bitboards[ENEMY|All];
    U64 pawns = board.bitboards[FRIENDLY|Pawn] & ~pinned;
    int end, offset = FRIENDLY == White ? (isLeftCapture ? 9 : 7) : (isLeftCapture ? -7 : -9);

    U64 fileMask = NOT_FILES[(isLeftCapture ? FileA : FileH)];
    U64 captures = FRIENDLY == White
        ? (pawns & fileMask) >> (isLeftCapture ? 9 : 7)
        : (pawns & fileMask) << (isLeftCapture ? 7 : 9);
    
    if (board.en_passant != not_on_board) {
        int enPassantEnd = board.en_passant + (FRIENDLY == White ? 8 : -8);
        if (is_set(captures, board.en_passant) &&
            validate_en_passant<FRIENDLY>(board, kingPos, offset+board.en_passant, enPassantEnd, occupancy))
            ADD_MOVE_FLAGS(ptr, offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT);
    }

    captures &= legalCaptures & enemy;

    U64 nonPromotionCaps = captures & NOT_RANKS[FRIENDLY == White ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        ADD_MOVE(ptr, end+offset, end);
    }

    U64 promotionCaps = captures & RANKS[FRIENDLY == White ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        ADD_PROMOTION(ptr, offset+end, end);
    }
}
template<Colors FRIENDLY>
void gen_pawn_pushes(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 blockers) {
    U64 pawns = board.bitboards[FRIENDLY|Pawn] & ~pinned;

    int end, offset = FRIENDLY == White ? 8 : -8;
    int doubleOffset = offset << 1;

    U64 pawnSingleMoves = (FRIENDLY == White ? pawns >> 8 : pawns << 8) & ~occupancy;

    U64 nonPromotionMoves = blockers & pawnSingleMoves & NOT_RANKS[FRIENDLY == White ? Rank8 : Rank1];
    while (nonPromotionMoves) {
        end = pop_lsb(nonPromotionMoves);
        ADD_MOVE(ptr, offset+end, end);
    }

    U64 promotionMoves = blockers & pawnSingleMoves & RANKS[FRIENDLY == White ? Rank8 : Rank1];
    while (promotionMoves) {
        end = pop_lsb(promotionMoves);
        ADD_PROMOTION(ptr, offset+end, end);
    }

    pawnSingleMoves &= RANKS[FRIENDLY == White ? Rank3 : Rank6];
    U64 pawnDoubleMoves = FRIENDLY == White
        ? pawnSingleMoves >> 8
        : pawnSingleMoves << 8;
    pawnDoubleMoves &= ~occupancy & blockers;
    while (pawnDoubleMoves) {
        end = pop_lsb(pawnDoubleMoves);
        ADD_MOVE(ptr, doubleOffset+end, end);
    }
}
template<Colors FRIENDLY>
void gen_pawn_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 legalCaptures, U64 blockers, int kingPos) {
    gen_pawn_captures<FRIENDLY, true>(board, ptr, occupancy, pinned, legalCaptures, kingPos);
    gen_pawn_captures<FRIENDLY, false>(board, ptr, occupancy, pinned, legalCaptures, kingPos);
    gen_pawn_pushes<FRIENDLY>(board, ptr, occupancy, pinned, blockers);
}
template<Colors FRIENDLY>
void gen_knight_moves(Board &board, Move *&ptr, U64 pinned, U64 movesMask) {
    U64 knights = board.bitboards[FRIENDLY|Knight] & ~pinned;
    U64 mask = movesMask & ~board.bitboards[FRIENDLY|All];

    int start, end;
    while (knights) {
        start = pop_lsb(knights);
        U64 moves = KNIGHT_MOVES[start] & mask;
        while (moves) {
            end = pop_lsb(moves);
            ADD_MOVE(ptr, start, end);
        }
    }
}
template<Colors FRIENDLY>
void gen_bishop_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask) {
    U64 bishops = board.bitboards[FRIENDLY|Bishop] & ~pinned;
    U64 mask = movesMask & ~board.bitboards[FRIENDLY|All];
    
    int start, end;
    while (bishops) {
        start = pop_lsb(bishops);
        U64 moves = magic_bishop_moves(start, occupancy) & mask;
        while (moves) {
            end = pop_lsb(moves);
            ADD_MOVE(ptr, start, end);
        }
    }
}
template<Colors FRIENDLY>
void gen_rook_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask) {
    U64 rooks = board.bitboards[FRIENDLY|Rook] & ~pinned;
    U64 mask = movesMask & ~board.bitboards[FRIENDLY|All];

    int start, end;
    while (rooks) {
        start = pop_lsb(rooks);
        U64 moves = magic_rook_moves(start, occupancy) & mask;
        while (moves) {
            end = pop_lsb(moves);
            ADD_MOVE(ptr, start, end);
        }
    }
}
template<Colors FRIENDLY>
void gen_queen_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask) {
    U64 queens = board.bitboards[FRIENDLY|Queen] & ~pinned;
    U64 mask = movesMask & ~board.bitboards[FRIENDLY|All];

    int start, end;
    while (queens) {
        start = pop_lsb(queens);
        U64 moves = magic_queen_moves(start, occupancy) & mask;
        while (moves) {
            end = pop_lsb(moves);
            ADD_MOVE(ptr, start, end);
        }
    }
}
template<Colors FRIENDLY>
void gen_king_moves(Board &board, Move *&ptr, U64 occupancy) {
    occupancy &= ~board.bitboards[FRIENDLY|King];
    int end, start = lsb_idx(board.bitboards[FRIENDLY|King]);

    U64 kingMoves = KING_MOVES[start] & ~board.bitboards[FRIENDLY|All];

    while (kingMoves) {
        end = pop_lsb(kingMoves);
        if (!is_under_attack<FRIENDLY>(end, board, occupancy))
            ADD_MOVE(ptr, start, end);
    }
}
template<Colors FRIENDLY>
void gen_castling_moves(Board &board, Move *&ptr, U64 occupancy) {
    if (FRIENDLY == White) {
        if (board.castling & WHITE_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileB | FileC | FileD]) ||
                is_under_attack<White>(d1, board, occupancy) ||
                is_under_attack<White>(c1, board, occupancy))) {
                    ADD_MOVE_FLAGS(ptr, e1, c1, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & WHITE_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileF | FileG]) ||
                is_under_attack<White>(f1, board, occupancy) ||
                is_under_attack<White>(g1, board, occupancy))) {
                    ADD_MOVE_FLAGS(ptr, e1, g1, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    } else {
        if (board.castling & BLACK_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileB | FileC | FileD]) ||
                is_under_attack<Black>(d8, board, occupancy) ||
                is_under_attack<Black>(c8, board, occupancy))) {
                    ADD_MOVE_FLAGS(ptr, e8, c8, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & BLACK_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileF | FileG]) ||
                is_under_attack<Black>(f8, board, occupancy) ||
                is_under_attack<Black>(g8, board, occupancy))) {
                    ADD_MOVE_FLAGS(ptr, e8, g8, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                                Pinned pieces                               */
/* -------------------------------------------------------------------------- */
template<Colors FRIENDLY, bool isBishop>
void gen_pinned_piece_moves(Move *&ptr, int pinnedPos, U64 occupancy, U64 mask) {
    U64 moves = (isBishop
        ? magic_bishop_moves(pinnedPos, occupancy)
        : magic_rook_moves(pinnedPos, occupancy))
        & mask;

    int end;
    while (moves) {
        end = pop_lsb(moves);
        ADD_MOVE(ptr, pinnedPos, end);
    }
}
template<Colors FRIENDLY>
void gen_pinned_pawn_pushes(Move *&ptr, int pinnedPos, U64 occupancy, U64 mask) {
    U64 pawns = SET_BIT_TABLE[pinnedPos];

    int end;

    // Single square moves
    U64 pawnSingleMoves = (FRIENDLY == White ? pawns >> 8 : pawns << 8) & ~occupancy;

    // Promotion moves
    U64 promotionMoves = pawnSingleMoves & RANKS[FRIENDLY == White ? Rank8 : Rank1] & mask;
    if (promotionMoves) {
        end = pop_lsb(promotionMoves);
        ADD_PROMOTION(ptr, pinnedPos, end);
    } else {
        U64 nonPromotionMoves = pawnSingleMoves & NOT_RANKS[FRIENDLY == White ? Rank8 : Rank1] & mask;
        if (nonPromotionMoves) {
            end = pop_lsb(nonPromotionMoves);
            ADD_MOVE(ptr, pinnedPos, end);
        }

        // Double square moves
        pawnSingleMoves &= RANKS[FRIENDLY == White ? Rank3 : Rank6];
        U64 pawnDoubleMoves = FRIENDLY == White ? pawnSingleMoves >> 8 : pawnSingleMoves << 8;
        pawnDoubleMoves &= mask & ~occupancy;
        if (pawnDoubleMoves) {
            end = pop_lsb(pawnDoubleMoves);
            ADD_MOVE(ptr, pinnedPos, end);
        }
    }
}
template<Colors FRIENDLY, bool isLeftCapture>
void gen_pinned_pawn_captures(Move *&ptr, int pinnedPos, U64 occupancy, U64 mask, int kingPos, Board &board) {
    Colors ENEMY = !FRIENDLY;
    
    U64 enemy = board.bitboards[ENEMY|All];
    U64 pawns = SET_BIT_TABLE[pinnedPos];
    int end, offset = FRIENDLY == White ? (isLeftCapture ? 9 : 7) : (isLeftCapture ? -7 : -9);
    
    U64 fileMask = NOT_FILES[(isLeftCapture ? FileA : FileH)];
    U64 captures = FRIENDLY == White
        ? (pawns & fileMask) >> (isLeftCapture ? 9 : 7)
        : (pawns & fileMask) << (isLeftCapture ? 7 : 9);

    captures &= mask;
    
    // Promotion captures
    U64 promotionCaps = captures & RANKS[FRIENDLY == White ? Rank8 : Rank1] & enemy;
    if (promotionCaps) {
        int end = pop_lsb(promotionCaps);
        ADD_PROMOTION(ptr, pinnedPos, end);
    } else {
        if (board.en_passant != not_on_board) {
            int enPassantEnd = board.en_passant + (FRIENDLY == White ? 8 : -8);
            if (is_set(captures, board.en_passant) &&
                validate_en_passant<FRIENDLY>(board, kingPos, pinnedPos, enPassantEnd, occupancy))
                    ADD_MOVE_FLAGS(ptr, pinnedPos, enPassantEnd, MOVETYPE_ENPASSANT);
        }

        U64 nonPromotionCaps = captures & NOT_RANKS[FRIENDLY == White ? Rank8 : Rank1] & enemy;
        if (nonPromotionCaps) {
            end = pop_lsb(nonPromotionCaps);
            ADD_MOVE(ptr, pinnedPos, end);
        }
    }
}
template<Colors FRIENDLY>
void gen_pinned_moves(Board &board, Move *&ptr, U64 occupancy, U64 legalCaptures, U64 blockers, int kingPos, int pinnedPos, int attackerPos) {
    U64 movesMask = legalCaptures | blockers;
    U64 pinMoveMask = (SLIDER_RANGE[attackerPos][kingPos] & ~board.bitboards[FRIENDLY|All]) | SET_BIT_TABLE[attackerPos];
    U64 mask = movesMask & pinMoveMask;

    switch (board.pieces[pinnedPos]&0b111) {
        case Pawn: {
            int kingR, kingF, pinnedR, pinnedF;
            calc_rf(kingPos, kingR, kingF);
            calc_rf(pinnedPos, pinnedR, pinnedF);

            if (kingR == pinnedR) {
                // pawns cannot move along a rank
                break;
            } else if (kingF == pinnedF) {
                // pinned down a file, so only 1 square or 2 squares forward 
                gen_pinned_pawn_pushes<FRIENDLY>(ptr, pinnedPos, occupancy, pinMoveMask & blockers);
                break;
            } else {
                assert(abs(kingR - pinnedR) == abs(kingF - pinnedF));
                gen_pinned_pawn_captures<FRIENDLY, true>(ptr, pinnedPos, occupancy, legalCaptures & pinMoveMask, kingPos, board);
                gen_pinned_pawn_captures<FRIENDLY, false>(ptr, pinnedPos, occupancy, legalCaptures & pinMoveMask, kingPos, board);
            }

            break;
        }
        case Knight: {
            // a pinned knight cannot move
            break;
        }
        case Bishop: {
            gen_pinned_piece_moves<FRIENDLY, true>(ptr, pinnedPos, occupancy, mask);
            break;
        }
        case Rook: {
            gen_pinned_piece_moves<FRIENDLY, false>(ptr, pinnedPos, occupancy, mask);
            break;
        }
        case Queen: {
            gen_pinned_piece_moves<FRIENDLY, true>(ptr, pinnedPos, occupancy, mask);
            gen_pinned_piece_moves<FRIENDLY, false>(ptr, pinnedPos, occupancy, mask);
            break;
        }

        default: {
            throw std::runtime_error("The King cannot be pinned");
        }
    }
}
template<Colors FRIENDLY, bool isBishop>
U64 gen_pin_attackers(Board &board, Move *&ptr, int kingPos, U64 occupancy, U64 checkers, U64 legalCaptures, U64 blockers) {
    Colors ENEMY = !FRIENDLY;
    
    int start, pinnedPos;

    U64 pieceMask =
        board.bitboards[ENEMY|Queen] |
        board.bitboards[ENEMY|(isBishop?Bishop:Rook)];
    U64 attackers =
        (isBishop
            ? magic_bishop_moves(kingPos, board.bitboards[ENEMY|All])
            : magic_rook_moves(kingPos, board.bitboards[ENEMY|All]))
        & pieceMask
        & ~checkers;
    U64 pinnedPieces = 0ULL;

    while (attackers) {
        start = pop_lsb(attackers);
        U64 occupied = SLIDER_RANGE[start][kingPos] & board.bitboards[FRIENDLY|All];
        pinnedPos = pop_lsb(occupied);
        // only one piece blocking therefore there is a pin
        if (!occupied) {
            gen_pinned_moves<FRIENDLY>(board, ptr, occupancy, legalCaptures, blockers, kingPos, pinnedPos, start);
            set_pos(pinnedPieces, pinnedPos);
        }
    }
    return pinnedPieces;
}
template<Colors FRIENDLY>
U64 gen_pinned_pieces(Board &board, Move *&ptr, int kingPos, U64 occupancy, U64 checkers, U64 legalCaptures, U64 blockers) {
    return
        gen_pin_attackers<FRIENDLY, true>(board, ptr, kingPos, occupancy, checkers, legalCaptures, blockers) |
        gen_pin_attackers<FRIENDLY, false>(board, ptr, kingPos, occupancy, checkers, legalCaptures, blockers);
}

/* -------------------------------------------------------------------------- */
/*                               Main generator                               */
/* -------------------------------------------------------------------------- */
template<Colors FRIENDLY>
void gen_legal_moves(Board &board, Move *&ptr) {
    Colors ENEMY = !FRIENDLY;
    
    U64 occupancy = board.bitboards[White|All] | board.bitboards[Black|All];
    U64 enemy = board.bitboards[ENEMY|All];
    int kingPos = lsb_idx(board.bitboards[FRIENDLY|King]);

    // always generate king moves first
    gen_king_moves<FRIENDLY>(board, ptr, occupancy);

    // calculate pieces giving check
    U64 attackingKing = find_enemy_attackers<FRIENDLY>(kingPos, board, occupancy);

    switch (count_occupied(attackingKing)) {
        // double check
        case 2: {
            // king moves are the only option, already calculated
            break;
        }
        // single check
        case 1: {
            int attackerPos = lsb_idx(attackingKing);

            U64 blockers = board.pieces[attackerPos]&0b110
                ? SLIDER_RANGE[kingPos][attackerPos]
                : 0ULL; // a knight cannot move out of a pin

            U64 movesMask = attackingKing | blockers;
            U64 pinned = gen_pinned_pieces<FRIENDLY>(board, ptr, kingPos, occupancy, attackingKing, attackingKing, blockers);

            gen_pawn_moves<FRIENDLY>(board, ptr, occupancy, pinned, attackingKing, blockers, kingPos);
            gen_knight_moves<FRIENDLY>(board, ptr, pinned, movesMask);
            gen_bishop_moves<FRIENDLY>(board, ptr, occupancy, pinned, movesMask);
            gen_rook_moves<FRIENDLY>(board, ptr, occupancy, pinned, movesMask);
            gen_queen_moves<FRIENDLY>(board, ptr, occupancy, pinned, movesMask);
            break;
        }
        // not in check - standard move generation
        case 0: {
            U64 pinned = gen_pinned_pieces<FRIENDLY>(board, ptr, kingPos, occupancy, 0ULL, FULL_BB, FULL_BB);

            gen_castling_moves<FRIENDLY>(board, ptr, occupancy);
            gen_pawn_moves<FRIENDLY>(board, ptr, occupancy, pinned, FULL_BB, FULL_BB, kingPos);
            gen_knight_moves<FRIENDLY>(board, ptr, pinned, FULL_BB);
            gen_bishop_moves<FRIENDLY>(board, ptr, occupancy, pinned, FULL_BB);
            gen_rook_moves<FRIENDLY>(board, ptr, occupancy, pinned, FULL_BB);
            gen_queen_moves<FRIENDLY>(board, ptr, occupancy, pinned, FULL_BB);
            break;
        }
        default: {
            throw std::runtime_error("Invalid number of attackers on the king");
        }
    }
}

int chess_cpp::gen_moves(Board &board, Move *arr) {
    Move *ptr = arr;

    if (board.white_to_move) {
        gen_legal_moves<White>(board, ptr);
    } else {
        gen_legal_moves<Black>(board, ptr);
    }

    return ptr - arr;
}
