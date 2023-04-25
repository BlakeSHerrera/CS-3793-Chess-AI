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

/* The maximum number of legal moves is 218, but the maximum number
 * of pseudo-legal moves is higher. It has a hard upper bound of 321.
 * The true upper bound is not immediately obvious.
 */
#define MAX_MOVES 321

/* The relevant occupancy squares are all rays for a piece minus
 * when the ray extends into the opposite edge of the board.
 * (Such a piece on the edge is not a blocker.)
 * See the link at the top of the file for an explanation of
 * how magic bitboards work.
 */
extern bitmask BISHOP_RELEVANT_OCCUPANCY[NUM_SQUARES],
               ROOK_RELEVANT_OCCUPANCY[NUM_SQUARES],
               BISHOP_TABLE[NUM_SQUARES][1 << 9],
               ROOK_TABLE[NUM_SQUARES][1 << 12],
               KNIGHT_TABLE[NUM_SQUARES],
               KING_TABLE[NUM_SQUARES],  // attacks only
               PAWN_TABLE[2][NUM_SQUARES];  // attacks only

extern int ROOK_BITS[NUM_SQUARES],
           BISHOP_BITS[NUM_SQUARES];

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
 * @param rookSquare - The current square of the rook.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (Initially 0)
 */
void _initMagicRook(Square rookSquare, bitmask bm, int i);

/**
 * Private function.
 * This function recursively computes all possible blockers for any
 * bitmask of relevant occupied squares (masked blockers) for a bishop,
 * and stores it in the magic table.
 * @param bishopSquare - The current square of the bishop.
 * @param bm - The bitmask of relevant occupied squares.
 * @param i - The current square index. (Initially 0)
 */
void _initMagicBishop(Square bishopSquare, bitmask bm, Square i);

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
 * This function generates psdeudo-legal moves from a given position.
 * That is, the moves obey the rules for move generation, but are not
 * checked for legality (if the king is in check).
 * @param state - Pointer to the current state.
 * @param moveBuffer - Output buffer array for the list of moves.
 * @param numMoves - Output variable for the number of moves generated.
 */
void generatePseudoLegalMoves(GameState *state, Move moveBuffer[MAX_MOVES], int *numMoves);

/**
 * This function generates legal moves from a given position.
 * @param state - Pointer to the current state.
 * @param moveBuffer - Output buffer array for the list of moves.
 * @param numMoves - Output variable for the number of moves generated.
 */
void generateLegalMoves(GameState *state, Move moveBuffer[MAX_MOVES], int *numMoves);

/**
 * This function plays a move on a game state, and returns the new state.
 * The move is not checked for legality.
 * @param state - A pointer to the current state.
 * @param m - The move to play.
 * @return A new state with the move played.
 */
GameState pushMove(GameState *state, Move m);

#endif // MOVEGEN_H_INCLUDED
