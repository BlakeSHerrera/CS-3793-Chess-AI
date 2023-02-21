/**
 * debug.h defines functions useful for debugging, such as printing
 * bit masks and board representations. The PERFT test is short for
 * PERFormance Test.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see https://www.chessprogramming.org/Perft
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "bitboard.h"
#include "square.h"

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

/**
 * Prints out the various bitmask constants.
 */
void testBitmasks();

/**
 * Prints out various tests for _sumBits in movegen.
 */
void testSumBits();

/**
 * Prints out various tests for shifting up, down, left, and right in
 * bitboard.
 */
void testShifts();

/**
 * Prints out various tests for _calculateRookMoves and
 * _calculateBishopMoves in movegen.
 */
void testMoveCalculation();

/**
 * Performs tests for the rays calculated in movegenInit.
 */
void testRays();

/**
 * Performs tests for the rook and bishop relevant occupancy
 * (masked blockers) calculated in movegenInit.
 */
void testRelevantOccupancy();

/**
 * Performs tests for the magic bitboard numbers to ensure perfect hashing.
 * Ensure movegenInit is called beforehand.
 * @note This function will reset the magic tables and invalidate them.
 */
void testMagic();

/**
 * Private function.
 * This function is a helper to testMagic.
 * @param bm - The bitmask of relevant blockers.
 * @param i - The square of the rook.
 */
void _testMagicRook(bitmask bm, Square i);

/**
 * Private function.
 * This function is a helper to testMagic.
 * @param bm - The bitmask of relevant blockers.
 * @param i - The square of the bishop.
 * @param bits - The number of 1 bits in i.
 */
void _testMagicBishop(bitmask bm, Square i, int bits);

/**
 * Prints various tests for bitboards.
 */
void testBitboards();

/**
 * Prints various tests for movegen.
 */
void testMovegen();

#endif // DEBUG_H_INCLUDED
