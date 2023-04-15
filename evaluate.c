/**
 * evaluate.c provides implementation for the functioned
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-04-13
 */

#include "evaluate.h"
#include "piece.h"
#include "position.h"

const double simplePieceValues[NUM_PIECES] = {
     1.0,  3.0,  3.0,  5.0,  9.0,  200.0,
    -1.0, -3.0, -3.0, -5.0, -9.0, -200.0
};

double pieceValueScore(GameState state, double pieceValues[NUM_PIECES]) {
    int i;
    double score;
    for(i=0; i<NUM_PIECES; i++) {
        score += sumBits(state.bb[i]) * pieceValues[i];
    }
    return i;
}
