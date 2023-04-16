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
    int whiteCount = sumBits(b[W_PAWN]) + sumBits(b[W_ROOK]) * 5 + sumBits(b[W_KNIGHT]) * 4 + sumBits(b[W_BISHOP]) * 4 + sumBits(b[W_QUEEN]) * 9 + sumBits(b[W_KING]) * 100;
    int blackCount = sumBits(b[B_PAWN]) + sumBits(b[B_ROOK]) * 5 + sumBits(b[B_KNIGHT]) * 4 + sumBits(b[B_BISHOP]) * 4 + sumBits(b[B_QUEEN]) * 9 + sumBits(b[B_KING]) * 100;
    return curPlayer ? whiteCount - blackCount : blackCount - whiteCount;
}

moveScorePair minMax(GameState curState, int ply, int alpha, int beta, int abPrune, int nullPrune, int forwardPrune)
{
    int count = 0;
    int bestScore = -10000;
    int score;
    int numMoves, moveCounter;
    Move bestMove = -1, nullMove = 0;
    Move *legalMoves = generateLegalMoves(&curState, &numMoves);
    GameState newState;
    moveScorePair pair;

    if (nullPrune && (getTurn(curState) ? !wInCheck(curState) : !bInCheck(curState)))
    {
        setSource(nullMove, LSB(curState.bb[B_KING - 6 * getTurn(curState)]) - 1);
        setDestination(nullMove, LSB(curState.bb[B_KING - 6 * getTurn(curState)]) - 1);
        setCapturedPiece(nullMove, 12);
        setMovedPiece(nullMove, 12);
        newState = pushMove(&curState, nullMove);
        if (ply == 0)
        {
            score = pieceCount(curState.bb, getTurn(curState));
        }
        else
        {
            score = -minMax(newState, ply - 1, alpha, beta, abPrune, nullPrune, forwardPrune).score;
        }
        bestScore = score;
        bestMove = nullMove;
    }

    if (forwardPrune && numMoves > forwardPrune)
    {
        // selection sort
        for (int i = 0; i < numMoves; i++)
        {
            int max_idx = i;
            for (int j = i + 1; j < numMoves; j++)
            {
                if (pieceCount(pushMove(&curState, legalMoves[j]).bb, 1 - getTurn(curState)) >= pieceCount(pushMove(&curState, legalMoves[max_idx]).bb, 1 - getTurn(curState)))
                {
                    max_idx = j;
                }
            }
            Move temp = legalMoves[max_idx];
            legalMoves[max_idx] = legalMoves[i];
            legalMoves[i] = temp;
        }
        numMoves -= forwardPrune;
    }

    for (moveCounter = 0; moveCounter < numMoves; moveCounter++)
    {
        newState = pushMove(&curState, legalMoves[moveCounter]);
        if (ply == 0)
        {
            score = pieceCount(newState.bb, getTurn(newState));
        }
        else
        {

            // get score from recursive call
            score = -minMax(newState, ply - 1, alpha, beta, abPrune, nullPrune, forwardPrune).score;
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
            alpha = alpha > score ? alpha : score;
        }
        else
        {
            beta = beta < score ? beta : score;
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
    if (nullPrune && nullMove == pair.move)
    {
        return minMax(curState, ply, alpha, beta, abPrune, 0, forwardPrune);
    }
    return pair;
}
