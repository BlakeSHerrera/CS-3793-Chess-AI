/**
 * debug.h defines functions useful for debugging, such as printing
 * bit masks and board representations. PERFT is short for
 * PERFormance Test, and is used to ensure move generation is done
 * correctly.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see https://www.chessprogramming.org/Perft
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "bitboard.h"
#include "square.h"
#include "position.h"
#include "move.h"

/* ********************************************************
 * These are helper functions.                            *
 **********************************************************/

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
 * Prints a game state to stdout. A1 is on the bottom left.
 * @param state - The GameState to print.
 */
void printGameState(GameState state);

/**
 * Prints a move to stdout.
 * @param m - The move to print.
 */
void printMove(Move m);

/**
 * Prints more information about a move to stdout.
 * @param state - The current game state.
 * @param m - The move to print.
 */
void printMoveVerbose(GameState state, Move m);



/* ********************************************************
 * These tests are for bitboard.                          *
 **********************************************************/

/**
 * Prints out the various bitmask constants and tables.
 */
void testBitmasks();

/**
 * Prints out various tests for summing bits.
 */
void testSumBits();

/**
 * Prints out various tests for shifting up, down, left, and right in
 * bitboard.
 */
void testShifts();



/* ********************************************************
 * These tests are for position.                          *
 **********************************************************/

 /**
 * Prints various tests for converting a FEN to a position.
 */
void testFromFen();

/**
 * Prints various tests for converting a FEN from a position.
 */
void testToFen();

/**
 * Prints various tests for testing if a square or bitmask
 * is attacked or not. Also applies to testing for checks.
 */
void testAttacks();

/**
 * Prints various tests for game ending states (checkmate,
 * stalemate, 50 move rule, 3-fold repetition, and
 * insufficient material).
 */
void testGameEnd();


/* ********************************************************
 * These tests are for move.                              *
 **********************************************************/

/**
 * Prints various tests for pushing long algebraic notation.
 */
void testPushLAN();


/* ********************************************************
 * These tests are for movegen.                           *
 **********************************************************/

/**
 * Prints out various tests for _calculateRookMoves and
 * _calculateBishopMoves in movegen.
 */
void testMoveCalculation();

/**
 * Performs tests for the rook and bishop relevant occupancy
 * (masked blockers) calculated in movegenInit.
 */
void testRelevantOccupancy();

/**
 * Performs tests for the king and knight moves
 * calculated in movegenInit.
 */
void testKingKnightMoves();

/**
 * Performs tests for the magic bitboard numbers to ensure perfect hashing.
 * Ensure movegenInit is called beforehand.
 * @note This function will reset the magic tables and invalidate them.
 */
void testMagic();

/**
 * Private function.
 * This function recursively computes all possible blockers for any
 * bitmask of relevant occupied squares (masked blockers) for a rook,
 * and checks for collisions in the magic table.
 * @param rookSquare - The current square of the rook.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (Initially 0)
 * @return The number of collisions.
 */
int _testMagicRook(Square rookSquare, bitmask bm, int i);

/**
 * Private function.
 * This function recursively computes all possible blockers for any
 * bitmask of relevant occupied squares (masked blockers) for a bishop,
 * and checks for collisions in the magic table.
 * @param bishopSquare - The current square of the bishop.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (Initially 0)
 * @param bits - The number of relevant occupied squares.
 * @return The number of collisions.
 */
int _testMagicBishop(Square bishopSquare, bitmask bm, int i);

/**
 * Prints various tests for generating pseudo-legal moves.
 */
void testGeneratePseudoLegalMoves();

/**
 * Prints various tests for generating legal moves.
 */
void testGenerateLegalMoves();

#endif // DEBUG_H_INCLUDED
