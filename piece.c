/**
 * piece.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-03-07
 * @see piece.h
 */

#include "piece.h"

Piece CHAR_TO_PIECE[128] = {
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, W_BISHOP, 12, 12, 12, 12, 12, 12, 12, 12, W_KING, 12,
    12, W_KNIGHT, 12, W_PAWN, W_QUEEN, W_ROOK, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, B_BISHOP, 12, 12, 12,
    12, 12, 12, 12, 12, B_KING, 12, 12, B_KNIGHT, 12, B_PAWN,
    B_QUEEN, B_ROOK, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12};
