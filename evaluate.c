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

#include <stdio.h>

const double simplePieceValues[NUM_PIECES] = {
     1.0,  3.0,  3.0,  5.0,  9.0,  200.0,
    -1.0, -3.0, -3.0, -5.0, -9.0, -200.0
};

double pieceValueScore(GameState state, const double pieceValues[NUM_PIECES]) {
    int i;
    double score = 0;
    for(i=0; i<NUM_PIECES; i++) {
        score += sumBits(state.bb[i]) * pieceValues[i];
    }
    return score;
}

double simplePieceValueCount(GameState g) {
    return pieceValueScore(g, simplePieceValues);
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
    return simplePieceValueCount(state) + mobility * mobilityFactor;
    #undef INITIATIVE

    //return (wMoves - bMoves) * mobilityFactor + simplePieceValueCount(state);
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
    return simplePieceValueCount(state) + mobility * mobilityFactor;
    #undef INITIATIVE

    //return (wMoves - bMoves) * mobilityFactor + simplePieceValueCount(state);
}
