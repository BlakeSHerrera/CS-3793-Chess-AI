/**
 * search.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Hezron Perez
 * @date 2023-03-12
 * @see piece.h
 */

#include "search.h"
#include "bitboard.h"
#include "move.h"
#include "movegen.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Move getRandomMove(GameState state) {
    int n;
    Move m, *moves = generateLegalMoves(&state, &n);
    m = moves[rand() % n];
    free(moves);
    return m;
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

    // if (nullMovePrune && ply > 2)
    // {
    //     // Do null move pruning
    //     score = -minMax(curState, ply - 2, -beta, -beta + 1, abPrune, false).score;
    //     if (score >= beta)
    //     {
    //         free(legalMoves);
    //         pair.move = -1;
    //         pair.score = beta;
    //         return pair;
    //     }
    // }

    for (moveCounter = 0; moveCounter < numMoves; moveCounter++)
    {
        newState = pushMove(&curState, legalMoves[moveCounter]);
        if (ply == 1)
        {
            score = pieceCount(newState.bb, getTurn(newState));
        }
        else
        {
            // // Check forward pruning
            // if (forwardPrune && (score + 2000) <= alpha)
            // {
            //     free(legalMoves);
            //     pair.move = legalMoves[moveCounter];
            //     pair.score = score;
            //     return pair;
            // }

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
