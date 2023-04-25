/**
 * evaluate.h contains functions associated with scoring a
 * single static position. Included are a simple piece * value
 * count and a custom defined function.
 * @author Blake Herrera
 * @date 2023-04-13
 */

#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

#include "piece.h"
#include "position.h"

/**
 * Returns the dot product between the count of pieces and their
 * respective value weights - the weighted sum of pieces.
 * @param state - The current game state.
 * @param pieceValues - An array of piece values.
 * @return The weighted sum of pieces.
 */
double pieceValueScore(GameState state, const double pieceValues[NUM_PIECES]);

/**
 * Counts all the pieces for a player.
 * @param g - the game state holding info about the game board.
 * @return A the total count of all the pieces for both players.
 */
double simplePieceValueCount(GameState g);

/**
 * Adds a small score for each pseudo-legal move each player can make.
 * Returns this plus the piece value sum.
 * @param state - The current game state.
 * @return A score based on weighted piece values and mobility.
 */
double valueAndMobility(GameState state);

#endif // EVALUATE_H_INCLUDED
