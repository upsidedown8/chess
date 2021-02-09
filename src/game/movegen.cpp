#include "movegen.hpp"

#include <stdexcept>
#include <assert.h>

using namespace chess_cpp;

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
    U64 bishopQueen =
        board.bitboards[board.enemy_color() | Bishop] |
        board.bitboards[board.enemy_color() | Queen];
    U64 rookQueen =
        board.bitboards[board.enemy_color() | Rook] |
        board.bitboards[board.enemy_color() | Queen];
    U64 attackingPieces =
        (get_bishop_moves(square, occupancy) & bishopQueen) |
        (get_rook_moves(square, occupancy) & rookQueen);
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
    if (PAWN_ATTACKS[board.white_to_move?WHITE:BLACK][square] & board.bitboards[board.enemy_color() | Pawn])
        return true;

    // rooks, bishops and queens
    U64 bishopQueen =
        board.bitboards[board.enemy_color() | Bishop] |
        board.bitboards[board.enemy_color() | Queen];
    U64 rookQueen =
        board.bitboards[board.enemy_color() | Rook] |
        board.bitboards[board.enemy_color() | Queen];
    return
        (get_bishop_moves(square, occupancy) & bishopQueen) |
        (get_rook_moves(square, occupancy) & rookQueen);
}
inline bool is_under_attack(int square, Board &board) {
    return is_under_attack(
        square,
        board,
        board.bitboards[White | All] | board.bitboards[Black | All]
    );
}
inline bool test_en_passant(Board &board, int kingPos, int enPassantStart, int enPassantEnd) {
    clr_pos(board.bitboards[board.enemy_color() | Pawn], enPassantEnd);
    clr_pos(board.bitboards[board.active_color() | Pawn], enPassantStart);
    set_pos(board.bitboards[board.active_color() | Pawn], board.en_passant);
    board.pieces[board.en_passant] = board.active_color() | Pawn;
    board.pieces[enPassantStart] = None;
    board.pieces[enPassantEnd] = None;
    board.update_bitboards();
    bool result = !is_under_attack(kingPos, board);
    set_pos(board.bitboards[board.enemy_color() | Pawn], enPassantEnd);
    set_pos(board.bitboards[board.active_color() | Pawn], enPassantStart);
    clr_pos(board.bitboards[board.active_color() | Pawn], board.en_passant);
    board.pieces[board.en_passant] = None;
    board.pieces[enPassantStart] = board.active_color() | Pawn;
    board.pieces[enPassantEnd] = board.enemy_color() | Pawn;
    board.update_bitboards();
    return result;
}

void gen_pawn_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 legalCaptures = FULL_BB, U64 blockers = FULL_BB) {
    U64 enemy = board.bitboards[board.enemy_color() | All];
    U64 pawns = board.bitboards[board.active_color() | Pawn] & ~pinned;

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
        *ptr++ = Move(offset+end, end, 0);
    }

    // Promotion moves
    // find any single square moves that end on the last rank
    U64 promotionMoves = blockers & pawnSingleMoves & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionMoves) {
        end = pop_lsb(promotionMoves);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
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
        *ptr++ = Move(doubleOffset+end, end, 0);
    }

    // Captures
    U64 enPassantCaptures;
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
    // Check for left capture En Passant
    if (board.en_passant != not_on_board) {
        enPassantCaptures = captures & SET_BIT_TABLE[board.en_passant];
        if (enPassantCaptures && test_en_passant(board, kingPos, offset+board.en_passant, enPassantEnd))
            *ptr++ = Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT);
    }

    captures &= legalCaptures & enemy;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        *ptr++ = Move(end+offset, end, 0);
    }
    // Promotion captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
    }

    // any pawn not on file H could potentially right capture
    offset = board.white_to_move
        ?  7
        : -9;
    captures = board.white_to_move
        ? (pawns & NOT_FILES[FileH]) >> 7
        : (pawns & NOT_FILES[FileH]) << 9;
    // Check for right capture En Passant
    if (board.en_passant != not_on_board) {
        enPassantCaptures = captures & SET_BIT_TABLE[board.en_passant];
        if (enPassantCaptures && test_en_passant(board, kingPos, offset+board.en_passant, enPassantEnd))
            *ptr++ = Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT);
    }
    captures &= legalCaptures & enemy;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        *ptr++ = Move(end+offset, end, 0);
    }
    // Promotion Captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
        *ptr++ = Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
    }
}
void gen_knight_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    // a pinned knight cannot move
    U64 knights = board.bitboards[board.active_color() | Knight] & ~pinned;
    int start;
    while (knights) {
        start = pop_lsb(knights);

        U64 knightsMoves = KNIGHT_MOVES[start] & nonFriendly & movesMask;

        while (knightsMoves)
            *ptr++ = Move(start, pop_lsb(knightsMoves), 0);
    }
}
void gen_bishop_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    // pinned moves are calculated elsewhere
    U64 bishops = board.bitboards[board.active_color() | Bishop] & ~pinned;
    U64 bishopMoves;
    int start;

    while (bishops) {
        start = pop_lsb(bishops);

        // generate legal moves
        bishopMoves = get_bishop_moves(start, occupancy) & nonFriendly & movesMask;

        while (bishopMoves)
            *ptr++ = Move(start, pop_lsb(bishopMoves), 0);
    }
}
void gen_rook_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    // pinned moves are calculated elsewhere
    U64 rooks = board.bitboards[board.active_color() | Rook] & ~pinned;
    U64 rookMoves;
    int start;

    while (rooks) {
        start = pop_lsb(rooks);

        rookMoves = get_rook_moves(start, occupancy) & nonFriendly & movesMask;

        while (rookMoves)
            *ptr++ = Move(start, pop_lsb(rookMoves), 0);
    }
}
void gen_queen_moves(Board &board, Move *&ptr, U64 occupancy, U64 pinned, U64 movesMask = FULL_BB) {
    U64 nonFriendly = ~board.bitboards[board.active_color() | All];
    // pinned moves are calculated elsewhere
    U64 queens = board.bitboards[board.active_color() | Queen] & ~pinned;
    U64 queenMoves;
    int start;

    while (queens) {
        start = pop_lsb(queens);

        queenMoves =
            get_rook_moves(start, occupancy) |
            get_bishop_moves(start, occupancy);
        queenMoves &= nonFriendly & movesMask;

        while (queenMoves)
            *ptr++ = Move(start, pop_lsb(queenMoves), 0);
    }
}
void gen_king_moves(Board &board, Move *&ptr, U64 occupancy) {
    U64 kings = board.bitboards[board.active_color() | King];
    occupancy &= ~kings;
    int start = pop_lsb(kings), end;

    U64 kingMoves = KING_MOVES[start] & ~board.bitboards[board.active_color() | All];

    while (kingMoves) {
        end = pop_lsb(kingMoves);
        if (!is_under_attack(end, board, occupancy))
            *ptr++ = Move(start, end, 0);
    }
}
void gen_castling(Board &board, Move *&ptr, U64 occupancy) {
    // a square is blocked if it has a piece on it or is under attack
    if (board.white_to_move) {
        if (board.castling & WHITE_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileB | FileC | FileD]) ||
                is_under_attack(e1, board) ||
                is_under_attack(d1, board) ||
                is_under_attack(c1, board))) {
                    *ptr++ = Move(e1, c1, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & WHITE_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank1] & FILES[FileF | FileG]) ||
                is_under_attack(e1, board) ||
                is_under_attack(f1, board) ||
                is_under_attack(g1, board))) {
                    *ptr++ = Move(e1, g1, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    }
    else {
        if (board.castling & BLACK_CASTLE_QS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileB | FileC | FileD]) ||
                is_under_attack(e8, board) ||
                is_under_attack(d8, board) ||
                is_under_attack(c8, board))) {
                    *ptr++ = Move(e8, c8, MOVETYPE_CASTLE | MOVECASTLE_QS);
            }
        }
        if (board.castling & BLACK_CASTLE_KS) {
            if (!((occupancy & RANKS[Rank8] & FILES[FileF | FileG]) ||
                is_under_attack(e8, board) ||
                is_under_attack(f8, board) ||
                is_under_attack(g8, board))) {
                    *ptr++ = Move(e8, g8, MOVETYPE_CASTLE | MOVECASTLE_KS);
            }
        }
    }
}

void gen_pinned_moves(Board &board, Move *&ptr, U64 occupancy, U64 legalCaptures, U64 blockers, int kingPos, int pinnedPos, int attackerPos) {
    U64 movesMask = legalCaptures | blockers;
    U64 pinMoveMask = (SLIDER_RANGE[attackerPos][kingPos] & ~board.bitboards[board.active_color() | All]) | SET_BIT_TABLE[attackerPos];
    U64 enemy = board.bitboards[board.enemy_color() | All];

    switch (board.pieces[pinnedPos]&0b111) {
        case Pawn: {
            U64 pawns = SET_BIT_TABLE[pinnedPos];

            int kingR, kingF, pinnedR, pinnedF;
            calc_rf(kingPos, kingR, kingF);
            calc_rf(pinnedPos, pinnedR, pinnedF);

            if (kingR == pinnedR) {
                // pawns cannot move along a rank
                break;
            } else if (kingF == pinnedF) {
                // pinned down a file, so only 1 square or 2 squares forward 

                // Single square moves
                U64 pawnSingleMoves = (board.white_to_move ? pawns >> 8 : pawns << 8) & ~occupancy;
                U64 nonPromotionMoves = pawnSingleMoves & NOT_RANKS[board.white_to_move ? Rank8 : Rank1] & pinMoveMask & blockers;
                if (nonPromotionMoves) *ptr++ = Move(pinnedPos, pop_lsb(nonPromotionMoves), 0);

                // Promotion moves
                U64 promotionMoves = pawnSingleMoves & RANKS[board.white_to_move ? Rank8 : Rank1] & pinMoveMask & blockers;
                if (promotionMoves) {
                    int end = pop_lsb(promotionMoves);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
                }

                // Double square moves
                // find pawns that originated from the start square, and are already validated for 1 square move.
                pawnSingleMoves &= RANKS[board.white_to_move ? Rank3 : Rank6];
                // of the single moves that were valid, move them one more square and check if they are valid.
                U64 pawnDoubleMoves = board.white_to_move ? pawnSingleMoves >> 8 : pawnSingleMoves << 8;
                pawnDoubleMoves &= pinMoveMask & blockers & ~occupancy;
                if (pawnDoubleMoves) *ptr++ = Move(pinnedPos, pop_lsb(pawnDoubleMoves), 0);
                break;
            } else {
                assert(abs(kingR - pinnedR) == abs(kingF - pinnedF));

                // pinned down a diagonal, only captures
                U64 captures, nonPromotionCaps, promotionCaps;
                U64 enPassantCaptures;
                int enPassantEnd = board.en_passant + (board.white_to_move ? 8 : -8);

                captures = board.white_to_move ? (pawns & NOT_FILES[FileA]) >> 9 : (pawns & NOT_FILES[FileA]) << 7;
                captures &= legalCaptures & pinMoveMask;                
                // Check for left capture En Passant
                if (board.en_passant != not_on_board) {
                    enPassantCaptures = captures & SET_BIT_TABLE[board.en_passant];
                    if (enPassantCaptures && test_en_passant(board, kingPos, pinnedPos, enPassantEnd))
                        *ptr++ = Move(pinnedPos, enPassantEnd, MOVETYPE_ENPASSANT);
                }
                captures &= enemy;
                // Non-Promotion captures
                nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
                if (nonPromotionCaps) *ptr++ = Move(pinnedPos, pop_lsb(nonPromotionCaps), 0);
                // Promotion captures
                promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
                if (promotionCaps) {
                    int end = pop_lsb(promotionCaps);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
                }
                captures = board.white_to_move
                    ? (pawns & NOT_FILES[FileH]) >> 7
                    : (pawns & NOT_FILES[FileH]) << 9;
                captures &= legalCaptures & pinMoveMask;
                // Check for right capture En Passant
                if (board.en_passant != not_on_board) {
                    enPassantCaptures = captures & SET_BIT_TABLE[board.en_passant];
                    if (enPassantCaptures && test_en_passant(board, kingPos, pinnedPos, enPassantEnd))
                        *ptr++ = Move(pinnedPos, enPassantEnd, MOVETYPE_ENPASSANT);
                }
                captures &= enemy;
                // Non-Promotion captures
                nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
                if (nonPromotionCaps) *ptr++ = Move(pinnedPos, pop_lsb(nonPromotionCaps), 0);
                // Promotion Captures
                promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
                if (promotionCaps) {
                    int end = pop_lsb(promotionCaps);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP);
                    *ptr++ = Move(pinnedPos, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN);
                }
            }
            break;
        }
        case Knight: {
            // a pinned knight cannot move
            break;
        }
        case Bishop: {
            U64 bishopMoves = get_bishop_moves(pinnedPos, occupancy) & movesMask & pinMoveMask;

            while (bishopMoves)
                *ptr++ = Move(pinnedPos, pop_lsb(bishopMoves), 0);
            
            break;
        }
        case Rook: {
            U64 rookMoves = get_rook_moves(pinnedPos, occupancy) & movesMask & pinMoveMask;

            while (rookMoves)
                *ptr++ = Move(pinnedPos, pop_lsb(rookMoves), 0);
            
            break;
        }
        case Queen: {
            U64 queenMoves = (get_rook_moves(pinnedPos, occupancy) | get_bishop_moves(pinnedPos, occupancy)) & movesMask & pinMoveMask;

            while (queenMoves)
                *ptr++ = Move(pinnedPos, pop_lsb(queenMoves), 0);
            break;
        }
        case King:
        default: {
            throw std::runtime_error("The King cannot be pinned");
        }
    }
}

U64 gen_pinned_pieces(Board &board, Move *&ptr, int kingPos, U64 occupancy, U64 enemy, U64 attackingKing, U64 legalCaptures, U64 blockers) {
    // calculate pinned pieces
    U64 bishopQueen =
        board.bitboards[board.enemy_color() | Bishop] |
        board.bitboards[board.enemy_color() | Queen];
    U64 rookQueen =
        board.bitboards[board.enemy_color() | Rook] |
        board.bitboards[board.enemy_color() | Queen];

    U64 bishopPinnedPieces, rookPinnedPieces;
    U64 occupied, pinMoveMask, friendly = board.bitboards[board.active_color() | All];
    int start, pinnedPos;
    
    // pieces with a line on the king that are not attacking it
    bishopPinnedPieces = 0ULL;
    U64 bishopAttackers = (get_bishop_moves(kingPos, enemy) & bishopQueen);
    bishopAttackers &= ~attackingKing;
    while (bishopAttackers) {
        start = pop_lsb(bishopAttackers);
        U64 occupied = SLIDER_RANGE[start][kingPos] & friendly;
        pinnedPos = pop_lsb(occupied);
        // only one piece blocking therefore there is a pin
        if (!occupied) {
            gen_pinned_moves(board, ptr, occupancy, legalCaptures, blockers, kingPos, pinnedPos, start);
            set_pos(bishopPinnedPieces, pinnedPos);
        }
    }

    // pieces with a line on the king that are not attacking it
    rookPinnedPieces = 0ULL;
    U64 rookAttackers = (get_rook_moves(kingPos, enemy) & rookQueen);
    rookAttackers &= ~attackingKing;
    while (rookAttackers) {
        start = pop_lsb(rookAttackers);
        U64 occupied = SLIDER_RANGE[start][kingPos] & friendly;
        pinnedPos = pop_lsb(occupied);
        // only one piece blocking therefore there is a pin
        if (!occupied) {
            gen_pinned_moves(board, ptr, occupancy, legalCaptures, blockers, kingPos, pinnedPos, start);
            set_pos(rookPinnedPieces, pinnedPos);
        }
    }
    return bishopPinnedPieces | rookPinnedPieces;
}

int chess_cpp::gen_moves(Board &board, Move *arr) {
    Move *ptr = arr;
    U64 occupancy = board.bitboards[White | All] | board.bitboards[Black | All];

    // always generate king moves first
    gen_king_moves(board, ptr, occupancy);

    // calculate pieces giving check
    U64 enemy = board.bitboards[board.enemy_color() | All];
    U64 kingBitBoard = board.bitboards[board.active_color() | King];
    int kingPos = pop_lsb(kingBitBoard);
    U64 attackingKing = get_attackers(kingPos, board, occupancy);

    switch (count_occupied(attackingKing)) {
        // double check
        case 2: {
            // king moves are the only option, already calculated
            break;
        }
        // single check
        case 1: {
            U64 captures = attackingKing;

            U64 tmp = attackingKing;
            int attackerPos = pop_lsb(tmp);
            U64 blockers = (board.pieces[attackerPos]&0b111) <= Knight
                ? 0ULL
                : SLIDER_RANGE[kingPos][attackerPos];

            U64 movesMask = captures | blockers;

            U64 pinned = gen_pinned_pieces(board, ptr, kingPos, occupancy, enemy, attackingKing, captures, blockers);

            gen_pawn_moves(board, ptr, occupancy, pinned, captures, blockers);
            gen_knight_moves(board, ptr, occupancy, pinned, movesMask);
            gen_bishop_moves(board, ptr, occupancy, pinned, movesMask);
            gen_rook_moves(board, ptr, occupancy, pinned, movesMask);
            gen_queen_moves(board, ptr, occupancy, pinned, movesMask);
            break;
        }
        // not in check - standard move generation
        case 0: {
            U64 pinned = gen_pinned_pieces(board, ptr, kingPos, occupancy, enemy, attackingKing, FULL_BB, FULL_BB);

            gen_castling(board, ptr, occupancy);
            gen_pawn_moves(board, ptr, occupancy, pinned);
            gen_knight_moves(board, ptr, occupancy, pinned);
            gen_bishop_moves(board, ptr, occupancy, pinned);
            gen_rook_moves(board, ptr, occupancy, pinned);
            gen_queen_moves(board, ptr, occupancy, pinned);
            break;
        }
        default: {
            throw std::runtime_error("Invalid number of attackers on the king");
        }
    }

    return ptr - arr;
}
