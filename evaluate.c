/**
 * evaluate.c provides implementation for the functioned
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-04-13
 */

#include "evaluate.h"
#include "piece.h"
#include "position.h"
#include "config.h"
#include "move.h"
#include "movegen.h"
#include "debug.h"

void setMaterialScore(GameState *state) {
    int i;
    state->material = 0;
    for(i=0; i<NUM_PIECES; i++) {
        state->material += sumBits(state->bb[i]) * pieceValues[i];
    }
}

double materialEval(GameState state) {
    return state.material;
}

double valueAndInfluence(GameState state) {
    int wMoves, bMoves, turn = getTurn(state);
    Move moveBuffer[MAX_MOVES];
    double mobility;

    setTurn(state, 1);
    generatePseudoLegalMoves(&state, moveBuffer, &wMoves);
    setTurn(state, 0);
    generatePseudoLegalMoves(&state, moveBuffer, &bMoves);


    // Make the engine prefer to have more moves?
    #define INITIATIVE 1.00001
    if(turn) {
        mobility = wMoves * INITIATIVE - bMoves;
    } else {
        mobility = wMoves - bMoves * INITIATIVE;
    }
    return state.material + mobility * mobilityFactor;
    #undef INITIATIVE
}

double valueAndMobility(GameState state) {
    int wMoves, bMoves, turn = getTurn(state);
    Move moveBuffer[MAX_MOVES];
    double mobility;

    setTurn(state, 1);
    generateLegalMoves(&state, moveBuffer, &wMoves);
    setTurn(state, 0);
    generateLegalMoves(&state, moveBuffer, &bMoves);

    // Make the engine prefer to have more moves?
    #define INITIATIVE 1.00001
    if(turn) {
        mobility = wMoves * INITIATIVE - bMoves;
    } else {
        mobility = wMoves - bMoves * INITIATIVE;
    }
    return state.material + mobility * mobilityFactor;
    #undef INITIATIVE
}
