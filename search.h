/**
 * search.h provides functions related to searching the game tree
 * including pruning techniques such as alpha-beta pruning,
 * null move pruning, forward pruning, and transposition tables.
 * Also implemented is quiescence search.
 * @author Blake Herrera
 * @date 2023-04-13
 */

#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "move.h"
#include "position.h"

Move getRandomMove(GameState state);

#endif // SEARCH_H_INCLUDED
