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

#define PERFT2_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PERFT3_FEN "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"
#define PERFT4_FEN "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define PERFT4_ALT_FEN "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 "
#define PERFT5_FEN "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
#define PERFT6_FEN "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

#define PERFT_NODES 0
#define PERFT_CAPTURES 1
#define PERFT_EP 2
#define PERFT_CASTLES 3
#define PERFT_PROMOTIONS 4
#define PERFT_CHECKS 5
#define PERFT_DISCOVERED_CHECKS 6
#define PERFT_DOUBLE_CHECKS 7
#define PERFT_CHECKMATES 8

typedef struct perftResults {
    unsigned long long int nodes;
    unsigned long long int captures;
    unsigned long long int ep;  // Combined with captures
    unsigned long long int castles;
    unsigned long long int promotions;
    unsigned long long int checks;
    unsigned long long int discoveredChecks;
    unsigned long long int doubleChecks;
    unsigned long long int checkmates;
    unsigned long long int other;
    double seconds;
} perftResults;

/* ********************************************************
 * These are helper functions.                            *
 **********************************************************/

/**
 * Adds two performance test results together.
 * @param a - The first to add.
 * @param b - The second to add.
 * @return The addition of the two results.
 */
perftResults addResults(perftResults a, perftResults b);

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

/**
 * Performs a performance test by generating and counting legal moves
 * and the associated time taken. Returns a performance test struct,
 * which includes the number of nodes and time taken to generate.
 * @param szFen - String representing the current FEN.
 * @param depth - How many ply to evaluate.
 * @return A struct of performance test results, which includes
 * the number of nodes and time taken to generate.
 */
perftResults performanceTest(const char *szFen, int depth);

/**
 * Private function.
 * Recursive helper to performanceTest.
 * Returns the number of leaf nodes.
 * @param state - Current game state.
 * @param depth - how many ply to evaluate.
 * @return The number of leaf nodes.
 */
perftResults _performanceTest(GameState state, int depth);

#endif // DEBUG_H_INCLUDED
