/**
 * main.c is the driver class which loads the engine.
 * It calls the initializers for other files before
 * communicating with the UCI program.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "uci.h"
#include "config.h"
#include "bitboard.h"
#include "movegen.h"
#include "evaluate.h"
#include "debug.h"

int main(int argc, char **argv) {
    int i;
    const double defaultPieceValues[13] = {
        1, 3, 3, 5, 9, 999, -1, -3, -3, -5, -9, -999, 0
    };
    double (*evaluationFuncs[NUM_EVALUATION_FUNCS])(GameState) = {
        materialEval, valueAndInfluence, valueAndMobility
    };

    searchStrategy = MINIMAX;
    pruning = AB_PRUNING;
    evaluation = MATERIAL_EVAL;
    evaluationFunction = materialEval;
    forwardPruneN = 999;
    numThreads = 1;
    maxSearchDepth = 99;
    mobilityFactor = 0.1;
    timeUseFraction = 0.05;
    quiescenceCutoff = 1.0;
    quiescenceMaxDepth = 3;
    for(i=0; i<NUM_PIECES+1; i++) {
        pieceValues[i] = defaultPieceValues[i];
    }
    for(i=0; i<NUM_EVALUATION_FUNCS; i++) {
        evaluationFunctions[i] = evaluationFuncs[i];
    }

    #define is(s) !strcmp(argv[i], s)
    for(i=1; i<argc; i++) {
        if(is("-searchStrategy")) {
            searchStrategy = atoi(argv[++i]);
        } else if(is("-pruning")) {
            pruning = atoi(argv[++i]);
        } else if(is("-evaluation")) {
            evaluation = atoi(argv[++i]);
            switch(evaluation) {
            case MATERIAL_EVAL:
                evaluationFunction = materialEval;
                break;
            case MATERIAL_AND_INFLUENCE:
                evaluationFunction = valueAndInfluence;
                break;
            case MATERIAL_AND_MOBILITY:
                evaluationFunction = valueAndMobility;
                break;
            default:
                break;
            }
        } else if(is("-forwardPruneN")) {
            forwardPruneN = atoi(argv[++i]);
        } else if(is("-numThreads")) {
            numThreads = atoi(argv[++i]);
        } else if(is("-maxSearchDepth")) {
            maxSearchDepth = atoi(argv[++i]);
        } else if(is("-mobilityFactor")) {
            mobilityFactor = atof(argv[++i]);
        } else if(is("-timeUseFraction")) {
            timeUseFraction = atof(argv[++i]);
        } else if(is("-quiescenceCutoff")) {
            quiescenceCutoff = atof(argv[++i]);
        } else if(is("-quiescenceMaxDepth")) {
            quiescenceMaxDepth = atoi(argv[++i]);
        } else if(is("-pieceValues")) {

        } else {
            fprintf(stderr, "Unknown command line arg: %s\n", argv[i]);
        }
    }
    #undef is

    srand(clock());
    bitboardInit();
    movegenInit();
    //findMagics();

    uciCommunicate();
    return 0;
}
