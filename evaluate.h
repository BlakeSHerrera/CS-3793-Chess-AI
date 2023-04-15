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

double pieceValueScore(GameState state, double pieceValues[NUM_PIECES]);

#endif // EVALUATE_H_INCLUDED
