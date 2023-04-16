/**
 * main.c is the driver class which loads the engine.
 * It calls the initializers for other files before
 * communicating with the UCI program.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "debug.h"
#include "move.h"
#include "movegen.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include "uci.h"
#include "magic.h"
#include "config.h"
#include "search.h"
#include "evaluate.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(clock());
    bitboardInit();
    movegenInit();
    // findMagics();
    int numMoves;
    GameState currentState = positionFromFen(START_FEN);
    printBitboard(currentState.bb);
    Move *legalMoves = generateLegalMoves(&currentState, &numMoves);
    int curPlayer;
    Move bestMove;

    while (numMoves && !is50MoveRule(currentState))
    {
        curPlayer = getTurn(currentState);
        printf("curPlayer: %d\n", curPlayer);
        bestMove = minMax(currentState, 3, -100000, 100000, 1, 1, 0).move;
        printf("score is %d\n", minMax(currentState, 3, -100000, 100000, 1, 1, 0).score);
        currentState = pushMove(&currentState, bestMove);
        printBitboard(currentState.bb);
        legalMoves = generateLegalMoves(&currentState, &numMoves);
    }
    if (is50MoveRule(currentState))
    {
        printf("Draw\n");
    }
    else if (wInCheck(currentState))
    {
        printf("Black Won\n");
    }
    else
    {
        printf("White Won\n");
    }
    printBitboard(currentState.bb);
    free(legalMoves);
    // testGenerateLegalMoves();
    // uciCommunicate();

    return 0;
}
