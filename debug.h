/**
 * debug.h defines functions useful for debugging, such as printing
 * bit masks and board representations.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "bitboard.h"

/**
 * Prints an 8x8 bitmask to stdout. A1 is on the bottom left.
 * @param bm - The bitmask to print.
 */
void printBitmask(bitmask bm);

/**
 * Prints a bitboard to stdout. A1 is on the bottom left.
 * @param bb - The Bitboard to print.
 */
void printBitboard(bitmask *bb);

#endif // DEBUG_H_INCLUDED
