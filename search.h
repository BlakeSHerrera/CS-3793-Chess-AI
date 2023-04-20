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
typedef struct moveScoreLeaves
{
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
 * @param alpha - -100000 intitally. Increases with recursive calls
 * @param beta - 100000 intitally. Decreases with recursive calls
 * @param abPrune - flag to enable alpha beta pruning.
 * @param nullPrune - flag to enable null pruning.
 * @param forwardPrune - number of nodes to forward prune.
 * @return A moveScorePair containing the best score and best move.
 */
moveScoreLeaves miniMax(GameState curState, int ply, int alpha, int beta, int abPrune, int nullPrune, int forwardPrune);

/**
 * Counts all the pieces for a player.
 * @param g - the game state holding info about the game board.
 * @return A the total count of all the pieces of one player.
 */
int pieceCount(GameState *g);

#endif // SEARCH_H_INCLUDED
