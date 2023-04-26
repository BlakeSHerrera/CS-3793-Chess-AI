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

int main(int argc, char **argv) {
    int i;
    searchStrategy = MINIMAX;
    pruning = AB_PRUNING;
    evaluation = VALUE_AND_MOBILITY;
    evaluationFunction = valueAndMobility;
    forwardPruneN = 999;
    numThreads = 1;
    maxSearchDepth = 99;
    mobilityFactor = 0.1;
    timeUseFraction = 0.05;

    #define is(s) !strcmp(argv[i], s)
    for(i=1; i<argc; i++) {
        if(is("-searchStrategy")) {
            searchStrategy = atoi(argv[++i]);
        } else if(is("-pruning")) {
            pruning = atoi(argv[++i]);
        } else if(is("-evaluation")) {
            evaluation = atoi(argv[++i]);
            switch(evaluation) {
            case PIECE_VALUE_EVAL:
                evaluationFunction = simplePieceValueCount;
                break;
            case VALUE_AND_INFLUENCE:
                evaluationFunction = valueAndInfluence;
                break;
            case VALUE_AND_MOBILITY:
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
