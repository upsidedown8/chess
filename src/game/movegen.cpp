#include "movegen.hpp"

using namespace chess_cpp;

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

U64 gen_pawn_moves(Board &board, std::vector<Move> &moves, U64 occupancy) {
    U64 attacks = 0ULL;
    U64 enemy = board.bitboards[board.enemy_color() | All];
    U64 friendly = board.bitboards[board.active_color() | All];
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
    U64 nonPromotionMoves = pawnSingleMoves & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionMoves) {
        end = pop_lsb(nonPromotionMoves);
        moves.push_back(Move(offset+end, end, 0));
    }

    // Promotion moves
    // find any single square moves that end on the last rank
    U64 promotionMoves = pawnSingleMoves & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionMoves) {
        end = pop_lsb(promotionMoves);
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN));
    }

    // Double square moves
    // find pawns that originated from the start square, and are already validated for 1 square move.
    pawnSingleMoves &= RANKS[board.white_to_move ? Rank3 : Rank6];
    // of the single moves that were valid, move them one more square and check if they are valid.
    U64 pawnDoubleMoves = board.white_to_move
        ? pawnSingleMoves >> 8
        : pawnSingleMoves << 8;
    pawnDoubleMoves &= ~occupancy;
    while (pawnDoubleMoves) {
        end = pop_lsb(pawnDoubleMoves);
        moves.push_back(Move(doubleOffset+end, end, 0));
    }

    // Captures
    U64 enPassantCaptures, enPassantSquare = 1ULL << board.en_passant;
    int enPassantEnd = board.en_passant + (board.white_to_move ? 8 : -8);

    U64 captures, nonPromotionCaps, promotionCaps;
    // any pawn not on file A could potentially left capture
    offset = board.white_to_move
        ?  9
        : -7;
    captures = board.white_to_move
        ? (pawns & NOT_FILES[FileA]) >> 9
        : (pawns & NOT_FILES[FileA]) << 7;
    // Check for left capture En Passant
    enPassantCaptures = captures & enPassantSquare;
    if (enPassantCaptures) {
        moves.push_back(Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT));
    }
    captures &= enemy;
    attacks |= captures;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        moves.push_back(Move(end+offset, end, 0));
    }
    // Promotion captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN));
    }

    // any pawn not on file H could potentially right capture
    offset = board.white_to_move
        ?  7
        : -9;
    captures = board.white_to_move
        ? (pawns & NOT_FILES[FileH]) >> 7
        : (pawns & NOT_FILES[FileH]) << 9;
    // Check for right capture En Passant
    enPassantCaptures = captures & enPassantSquare;
    if (enPassantCaptures) {
        moves.push_back(Move(offset+board.en_passant, enPassantEnd, MOVETYPE_ENPASSANT));
    }
    captures &= enemy;
    // Non-Promotion captures
    nonPromotionCaps = captures & NOT_RANKS[board.white_to_move ? Rank8 : Rank1];
    while (nonPromotionCaps) {
        end = pop_lsb(nonPromotionCaps);
        moves.push_back(Move(end+offset, end, 0));
    }
    // Promotion Captures
    promotionCaps = captures & RANKS[board.white_to_move ? Rank8 : Rank1];
    while (promotionCaps) {
        end = pop_lsb(promotionCaps);
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_PAWN));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_KNIGHT));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_BISHOP));
        moves.push_back(Move(offset+end, end, MOVETYPE_PROMOTION | MOVEPROMOTION_QUEEN));
    }

    return attacks;
}
U64 gen_knight_moves(Board &board, std::vector<Move> &moves, U64 occupancy) {
    U64 attacks = 0ULL;
    U64 friendly = board.bitboards[board.active_color() | All];
    U64 knights = board.bitboards[board.active_color() | Knight];
    int start, end;
    while (knights) {
        start = pop_lsb(knights);

        U64 knightsMoves = KNIGHT_MOVES[start] & ~friendly;
        attacks |= knightsMoves;

        while (knightsMoves) {
            end = pop_lsb(knightsMoves);
            moves.push_back(Move(start, end, 0));
        }
    }
    return attacks;
}
U64 gen_bishop_moves(Board &board, std::vector<Move> &moves, U64 occupancy) {
    U64 attacks = 0ULL;
    U64 friendly = board.bitboards[board.active_color() | All];
    U64 bishops = board.bitboards[board.active_color() | Bishop];
    int start, end;
    while (bishops) {
        start = pop_lsb(bishops);

        U64 bishopMoves = get_bishop_moves(start, occupancy) & ~friendly;
        attacks |= bishopMoves;

        while (bishopMoves) {
            end = pop_lsb(bishopMoves);
            moves.push_back(Move(start, end, 0));
        }
    }
    return attacks;
}
U64 gen_rook_moves(Board &board, std::vector<Move> &moves, U64 occupancy) {
    U64 attacks = 0ULL;
    U64 friendly = board.bitboards[board.active_color() | All];
    U64 rooks = board.bitboards[board.active_color() | Rook];
    int start, end;
    while (rooks) {
        start = pop_lsb(rooks);

        U64 rooksMoves = get_rook_moves(start, occupancy) & ~friendly;
        attacks |= rooksMoves;

        while (rooksMoves) {
            end = pop_lsb(rooksMoves);
            moves.push_back(Move(start, end, 0));
        }
    }
    return attacks;
}
U64 gen_queen_moves(Board &board, std::vector<Move> &moves, U64 occupancy) {
    U64 attacks = 0ULL;
    U64 friendly = board.bitboards[board.active_color() | All];
    U64 queens = board.bitboards[board.active_color() | Queen];
    int start, end;
    while (queens) {
        start = pop_lsb(queens);

        U64 queenMoves =
            get_rook_moves(start, occupancy) | 
            get_bishop_moves(start, occupancy);
        queenMoves &= ~friendly;
        attacks |= queenMoves;

        while (queenMoves) {
            end = pop_lsb(queenMoves);
            moves.push_back(Move(start, end, 0));
        }
    }
    return attacks;
}
U64 gen_king_moves(Board &board, std::vector<Move> &moves) {
    U64 attacks = 0ULL;
    U64 friendly = board.bitboards[board.active_color() | All];
    U64 kings = board.bitboards[board.active_color() | King];
    int start, end;
    while (kings) {
        start = pop_lsb(kings);

        U64 kingMoves = KING_MOVES[start] & ~friendly;
        attacks |= kingMoves;

        while (kingMoves) {
            end = pop_lsb(kingMoves);
            moves.push_back(Move(start, end, 0));
        }
    }
    return attacks;
}
U64 gen_castling(Board &board, std::vector<Move> &moves, U64 occupancy) {
    if (board.white_to_move) {
        if (board.castling & WHITE_CASTLE_QS) {
            if (!(occupancy & RANKS[Rank1] & FILES[FileB | FileC | FileD])) {
                moves.push_back(Move(e1, c1, MOVETYPE_CASTLE | MOVECASTLE_QS));
            }
        }
        if (board.castling & WHITE_CASTLE_KS) {
            if (!(occupancy & RANKS[Rank1] & FILES[FileF | FileG])) {
                moves.push_back(Move(e1, g1, MOVETYPE_CASTLE | MOVECASTLE_KS));
            }
        }
    }
    else {
        if (board.castling & BLACK_CASTLE_QS) {
            if (!(occupancy & RANKS[Rank8] & FILES[FileB | FileC | FileD])) {
                moves.push_back(Move(e8, c8, MOVETYPE_CASTLE | MOVECASTLE_QS));
            }
        }
        if (board.castling & BLACK_CASTLE_KS) {
            if (!(occupancy & RANKS[Rank8] & FILES[FileF | FileG])) {
                moves.push_back(Move(e8, g8, MOVETYPE_CASTLE | MOVECASTLE_KS));
            }
        }
    }

    return 0ULL;
}

U64 chess_cpp::gen_moves(Board &board, std::vector<Move> &moves) {
    U64 occupancy = board.bitboards[White | All] | board.bitboards[Black | All];
    U64 attacks = 0ULL;

    attacks |= gen_pawn_moves(board, moves, occupancy);
    attacks |= gen_knight_moves(board, moves, occupancy);
    attacks |= gen_bishop_moves(board, moves, occupancy);
    attacks |= gen_rook_moves(board, moves, occupancy);
    attacks |= gen_queen_moves(board, moves, occupancy);
    attacks |= gen_king_moves(board, moves);
    attacks |= gen_castling(board, moves, occupancy);

    return attacks;
}
