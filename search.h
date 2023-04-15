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

/* A move score pair has two fields: a Move and an int
 */
typedef struct moveScorePair
{
    Move move;
    int score;
} moveScorePair;

/**
 * Gets a random legal move.
 * @param state - The current state of the game.
 * @return A random legal move.
 */
Move getRandomMove(GameState state);

/**
 * Finds the best move from a game state.
 * @param curState - The current state of the game.
 * @param alpha - -100000 intitally. Increases with recursive calls
 * @param beta - 100000 intitally. Decreases with recursive calls
 * @param abPrune - flag to enable alpha beta pruning.
 * @return A moveScorePair containing the best score and best move.
 */
moveScorePair minMax(GameState curState, int ply, int alpha, int beta, int abPrune);

/**
 * Counts all the pieces for a player.
 * @param b - the bitmask array representing the game board.
 * @param curPlayer - The player whose pieces will be counted
 * @return A the total count of all the pieces of one player.
 */
int pieceCount(bitmask *b, int curPlayer);

#endif // SQUARE_H_INCLUDED
