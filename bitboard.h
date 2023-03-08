/**
 * bitboard.h contains useful macros for dealing with bitboards which
 * improves readability.
 *
 * A bitboard consists of 13 bitmasks (one for each piece and one representing all
 * occupied squares on the board for efficiency). Additionally there is extra info
 * typically found in the FEN such as castling rights, en passant target square,
 * whose turn it is, and the full and half-move counters.
 * A bitmask is a 64-bit unsigned integer which represents specific squares
 * on the board - a mask can be used on light squares, individual files,
 * ranks, squares, and even pieces.
 *
 * This compact representation makes for quick integration into modern
 * hardware which uses 64 bit registers. Additionally, the bitwise
 * operators used to perform these masks are very fast.
 * @author Blake Herrera
 * @date 2023-02-15
 * @todo Flip, Mirror, Rotate
 */

#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include "piece.h"
#include "square.h"

// Define directions for ray casting
#define NORTH 0
#define NORTHWEST 1
#define WEST 2
#define SOUTHWEST 3
#define SOUTH 4
#define SOUTHEAST 5
#define EAST 6
#define NORTHEAST 7

// Least Significant Bit
#define LSB __builtin_ffsll

typedef unsigned long long int bitmask;

extern bitmask FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
               RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
               LIGHT_SQUARES, DARK_SQUARES, ALL_SQUARES, NO_SQUARES, EDGES,
               FILES[8], RANKS[8], SQUARES[NUM_SQUARES],
               TOP_RANKS[9], BOTTOM_RANKS[9], LEFT_FILES[9], RIGHT_FILES[9],
               W_CASTLE_K, W_CASTLE_Q, B_CASTLE_K, B_CASTLE_Q,
               RAYS[8][NUM_SQUARES];

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

/**
 * This function sums the bits in a bitmask.
 * @param bm - The bitmask to sum.
 * @return A count of the number of 1 bits in the bitmask.
 */
int sumBits(bitmask bm);

#endif // BITBOARD_H_INCLUDED
