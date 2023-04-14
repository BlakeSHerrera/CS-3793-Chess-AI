/**
 * search.c implements the functions declared in the
 * associated header file.
 * @author Blake Herrera
 * @date 2023-04-13
 */

#include "search.h"
#include "move.h"
#include "position.h"
#include "movegen.h"

#include <stdlib.h>
#include <stdio.h>

Move getRandomMove(GameState state) {
    int n;
    Move m, *moves = generateLegalMoves(&state, &n);
    m = moves[rand() % n];
    free(moves);
    return m;
}
