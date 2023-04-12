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

/* A move score pair has two fields: a Move and an int
 */
typedef struct moveScorePair
{
    Move move;
    int score;
} moveScorePair;

/**
 * Finds the best move from a game state.
 * @param curState - The current state of the game.
 * @param alpha - -100000 intitally. Increases with recursive calls
 * @param beta - 100000 intitally. Decreases with recursive calls
 * @param abPrune - flag to enable alpha beta pruning.
 * @return A moveScorePair containing the best score and best move.
 */
moveScorePair minMax(GameState curState, int ply, int alpha, int beta, int abPrune);

/**
 * Counts all the pieces for a player.
 * @param b - the bitmask array representing the game board.
 * @param curPlayer - The player whose pieces will be counted
 * @return A the total count of all the pieces of one player.
 */
int pieceCount(bitmask *b, int curPlayer);

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

int pieceCount(bitmask *b, int curPlayer)
{
    return sumBits(b[W_PAWN]) + sumBits(b[W_ROOK]) * 5 + sumBits(b[W_KNIGHT]) * 4 + sumBits(b[W_BISHOP]) * 4 + sumBits(b[W_QUEEN]) * 9 + sumBits(b[W_KING]) * 100 ? curPlayer : sumBits(b[B_PAWN]) + sumBits(b[B_ROOK]) * 5 + sumBits(b[B_KNIGHT]) * 4 + sumBits(b[B_BISHOP]) * 4 + sumBits(b[B_QUEEN]) * 9 + sumBits(b[B_KING]) * 100;
}

moveScorePair minMax(GameState curState, int ply, int alpha, int beta, int abPrune)
{
    int bestScore = -10000;
    int score;
    int numMoves, moveCounter;
    Move bestMove = -1;
    Move *legalMoves = generateLegalMoves(&curState, &numMoves);
    GameState newState;
    moveScorePair pair;
    for (moveCounter = 0; moveCounter < numMoves; moveCounter++)
    {
        newState = pushMove(&curState, legalMoves[moveCounter]);
        if (ply == 1)
        {
            score = pieceCount(newState.bb, getTurn(newState));
        }
        else
        {
            // get score from recursive call
            score = -minMax(newState, ply - 1, alpha, beta, abPrune).score;
        }
        if (bestScore == -10000 || score > bestScore)
        {
            bestScore = score;
            bestMove = legalMoves[moveCounter];
        }
        if (!abPrune)
        {
            continue;
        }
        if (getTurn(newState))
        {
            alpha = alpha ? alpha > score : score;
        }
        else
        {
            beta = beta ? beta < score : score;
        }
        if (beta <= alpha)
        {
            pair.move = bestMove;
            pair.score = bestScore;
            return pair;
        }
    }
    free(legalMoves);
    if (bestMove != -1)
    {
        pair.move = bestMove;
        pair.score = bestScore;
    }
    else
    {
        pair.move = -1;
        pair.score = -10000;
    }
    return pair;
}