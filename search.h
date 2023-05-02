/**
 * search.h contains useful macros for readability. A square on the board
 * is indexed to an array index.
 * @author Hezron Perez
 * @date 2023-04-12
 */

#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "move.h"
#include "position.h"

/* A move score leaves struct has three fields:
 * a Move
 * the best score for this move (double)
 * the number of leaves for this move (long)
 */
typedef struct moveScoreLeaves {
    Move move;
    double score;
    unsigned long leaves;
} moveScoreLeaves;

/**
 * Gets a random legal move.
 * @param state - The current state of the game.
 * @return A random legal move.
 */
Move getRandomMove(GameState state);

/**
 * Finds the best move from a game state.
 * @param curState - The current state of the game.
 * @param alpha - -INFINITY initially. Increases with recursive calls
 * @param beta - INFINITY initially. Decreases with recursive calls
 * @param prevScore - static evaluation of the preceding position
 * @return A moveScoreLeaves containing the best score and best move.
 */
moveScoreLeaves miniMax(GameState curState, int ply, double alpha, double beta, double prevScore);

#endif // SEARCH_H_INCLUDED
