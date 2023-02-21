/**
 * movegen.h defines functions for generating legal and pseudo-legal moves
 * given a board state.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html
 */

#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "move.h"
#include "bitboard.h"
#include "square.h"
#include "position.h"

#define MAX_MOVES 218

/* The relevant occupancy squares are all rays for a piece minus
 * when the ray extends into the opposite edge of the board.
 * (Such a piece on the edge is not a blocker.)
 * See the link at the top of the file for an explanation of
 * how magic bitboards work.
 */
extern bitmask RAYS[8][64],
               BISHOP_RELEVANT_OCCUPANCY[NUM_SQUARES],
               ROOK_RELEVANT_OCCUPANCY[NUM_SQUARES],
               BISHOP_TABLE[NUM_SQUARES][1 << 9],
               ROOK_TABLE[NUM_SQUARES][1 << 12];

extern int BISHOP_BITS[NUM_SQUARES];

extern bitmask ROOK_TABLE[64][1 << 12]; // 12 possible rook blockers

/**
 * This function should be called on program start to initialize
 * the arrays for the magic bitboards. This should be called after
 * bitboardInit.
 */
void movegenInit();

/**
 * Private function.
 * This function recursively computes all possible blockers for any
 * bitmask of relevant occupied squares (masked blockers) for a rook,
 * and stores it in the magic table.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (The first call to the function
 * should initialize this to 0.)
 */
void _initMagicRook(bitmask bm, Square i);

/**
 * Private function.
 * This function recursively computes all possible blockers for any
 * bitmask of relevant occupied squares (masked blockers) for a bishop,
 * and stores it in the magic table.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (The first call to the function
 * should initialize this to 0.)
 * @param bits - The number of relevant occupied squares.
 */
void _initMagicBishop(bitmask bm, Square i, int bits);

/**
 * Private function.
 * This function returns the possible rook moves for a given
 * square and mask of blockers via magic bitboards.
 * @param square - The square the piece is on.
 * @param blockers - The bitmask of blockers for the position.
 * @return A bitmask of possible moves.
 */
bitmask _calculateRookMoves(Square square, bitmask blockers);

/**
 * Private function.
 * This function returns the possible bishop moves for a given
 * square and mask of blockers the piece is on.
 * @param square - The square the piece is on.
 * @param blockers - The bitmask of blockers for the position.
 * @return a bitmask of possible moves.
 */
bitmask _calculateBishopMoves(Square square, bitmask blockers);

/**
 * Private function.
 * This function counts the 1s in a bitmask.
 * @param bm - The bitmask to sum the bits of.
 * @return The count of 1s in the bitmask.
 */
int _sumBits(bitmask bm);

/**
 * This function generates psdeudo-legal moves from a given position.
 * That is, the moves obey the rules for move generation, but are not
 * checked for legality (if the king is in check).
 * @param state - Pointer to the current state.
 * @param numMoves - Output variable for the number of moves generated.
 * @return A pointer to a dynamically allocated array of moves.
 */
Move *generatePseudoLegalMoves(GameState *state, int *numMoves);

#endif // MOVEGEN_H_INCLUDED
