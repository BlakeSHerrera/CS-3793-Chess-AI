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
 * Sets the material score of the given state.
 * @param state - Pointer to the current game state.
 */
void setMaterialScore(GameState *state);

/**
 * Returns a state's material count.
 * @param state - The current state of the board.
 * @return The state's material count.
 */
double materialEval(GameState state);

/**
 * Adds a small score for each pseudo-legal move each player can make.
 * Returns this plus the material value.
 * @param state - The current game state.
 * @return A score based on weighted piece values and influence.
 */
double valueAndInfluence(GameState state);

/**
 * Adds a small score for each legal move each player could make
 * (if it were their turn to move).
 * Returns this plus the material value.
 * @param state - The current game state.
 * @return A score based on weighted piece values and mobility.
 */
double valueAndMobility(GameState state);

#endif // EVALUATE_H_INCLUDED
