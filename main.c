/**
 * main.c is the driver class which loads the engine.
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
#include "search.h"

int main(int argc, char **argv)
{
    bitboardInit();
    movegenInit();
    // findMagics();
    int numMoves;
    GameState currentState = positionFromFen(START_FEN);
    printBitboard(currentState.bb);
    Move *legalMoves = generateLegalMoves(&currentState, &numMoves);
    int curPlayer;
    Move bestMove;
    // replace with inCheckmate
    while (!wInCheck(currentState) && !bInCheck(currentState))
    // while(1)
    {

        curPlayer = getTurn(currentState);
        printf("curPlayer: %d\n", curPlayer);
        bestMove = minMax(currentState, 4, -100000, 100000, 1).move;
        printf("score is %d\n", minMax(currentState, 4, -100000, 100000, 1).score);
        currentState = pushMove(&currentState, bestMove);
        printBitboard(currentState.bb);
    }

    if (wInCheck(currentState))
    {
        printf("Black Won\n");
    }
    else if (bInCheck(currentState))
    {
        printf("White Won\n");
    }
    printBitboard(currentState.bb);
    free(legalMoves);
    // testGenerateLegalMoves();
    // uciCommunicate();

    return 0;
}