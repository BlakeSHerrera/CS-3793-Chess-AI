/**
 * config.h defines constants for use in the rest of the
 * program related to search and evaluation strategies.
 *
 * searchStrategy:
 *     0 - random move
 *     1 - minimax
 *     2 - minimax + quiescence search
 * pruning (switches):
 *     0 - no pruning
 *     1 - alpha beta pruning
 *     2 - null move pruning
 *     4 - forward pruning
 *     8 - transposition tables
 * evaluation:
 *     0 - piece * value count
 *     1 - custom function 1
 *     2 - custom function 2
 *
 * Other options:
 * forwardPruneN - forward pruning value
 * numThreads - number of threads
 *
 * @author Blake Herrera
 * @date 2023-04-13
 */

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdio.h>

#define RANDOM_MOVES 0
#define MINIMAX 1
#define MINIMAX_QUIESCENCE 2

#define NO_PRUNING 0
#define AB_PRUNING 1
#define NULL_PRUNING 2
#define FORWARD_PRUNING 4
#define TRASPOSITION_TABLES 8

#define PIECE_VALUE_EVAL 0
#define FUNC1 1
#define FUNC2 2

extern int searchStrategy, pruning, evaluation, searchDepth,
           forwardPruneN, numThreads;

#endif // CONFIG_H_INCLUDED
