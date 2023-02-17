/**
 * bitboard.h contains useful macros for dealing with bitboards which
 * improves readability.
 *
 * A bitboard consists of 12 bitmasks (one for each piece).
 * A bitmask is a 64-bit unsigned integer which represents specific squares
 * on the board - a mask can be used on light squares, individual files,
 * ranks, squares, and even pieces.
 *
 * This compact representation makes for quick integration into modern
 * hardware which uses 64 bit registers. Additionally, the bitwise
 * operators used to perform these masks are very fast.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include "piece.h"

#include <stdint.h>

// Define directions for ray casting
#define NORTH 0
#define NORTHWEST 1
#define WEST 2
#define SOUTHWEST 3
#define SOUTH 4
#define SOUTHEAST 5
#define EAST 6
#define NORTHEAST 7

typedef uint64_t bitmask;

extern bitmask FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
               RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
               LIGHT_SQUARES, DARK_SQUARES, ALL_SQUARES, NO_SQUARES, EDGES,
               FILES[8], RANKS[8], SQUARES[64],
               TOP_RANKS[8], BOTTOM_RANKS[8], LEFT_FILES[8], RIGHT_FILES[8];

/**
 * The initializer function should be called once on program start to
 * populate the table values.
 */
void bitboardInit();

/**
 * This function shifts a bitmask down n ranks, excluding overflows.
 * @param bm - The bitmask to shift.
 * @param n - The number of squares to shift.
 * @return - A bitmask shifted down n ranks.
 */
bitmask shiftDown(bitmask bm, int n);

/**
 * This function shifts a bitmask up n ranks, excluding overflows.
 * @param bm - The bitmask to shift.
 * @param n - The number of squares to shift.
 * @return - A bitmask shifted up n ranks.
 */
bitmask shiftUp(bitmask bm, int n);

/**
 * This function shifts a bitmask left n ranks, excluding overflows.
 * @param bm - The bitmask to shift.
 * @param n - The number of squares to shift.
 * @return - A bitmask shifted left n ranks.
 */
bitmask shiftLeft(bitmask bm, int n);

/**
 * This function shifts a bitmask right n ranks, excluding overflows.
 * @param bm - The bitmask to shift.
 * @param n - The number of squares to shift.
 * @return - A bitmask shifted right n ranks.
 */
bitmask shiftRight(bitmask bm, int n);

/**
 * This function finds all the occupied squares on a board and returns
 * it as a bitmask.
 * @param bb - The bitboard to parse.
 * @return - A bitmask of all occupied squares.
 */
bitmask getBlockers(bitmask *bb);

#endif // BITBOARD_H_INCLUDED
