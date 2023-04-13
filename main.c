/**
 * main.c is the driver class which loads the engine.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#include "bitboard.h"
#include "debug.h"
#include "move.h"
#include "movegen.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include "uci.h"
#include "magic.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    struct perftResults results;
    int i;

    bitboardInit();
    movegenInit();
    //findMagics();
    //testGenerateLegalMoves();
    //uciCommunicate();
    //return 0;
    //doPerft();
    //return 0;
    for(i=0; i<10; i++) {
        results = performanceTest(PERFT6_FEN, i);
        printf("Depth %d:\t%llu nodes,\t%lf seconds,\t%lf kn/s\n"
               "\t\tCaptures: %llu\tEP: %llu\tCastle: %llu\tPromotions:%llu\n\n",
                i, results.nodes, results.seconds, results.nodes / results.seconds / 1e3,
                results.captures, results.ep, results.castles, results.promotions);
    }
    return 0;
}
