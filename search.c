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

Move getRandomMove(GameState state)
{
    int n;
    Move m, *moves = generateLegalMoves(&state, &n);
    m = moves[rand() % n];
    free(moves);
    return m;
}

int pieceCount(bitmask *b)
{
    int i;
    double score = 0;
    double weights[NUM_PIECES] = {1, 3, 3, 5, 9, 200, -1, -3, -3, -5, -9, -200};
    for (i = 0; i < NUM_PIECES; i++)
    {
        score += sumBits(b[i]) * weights[i];
    }
    return score;
}

moveScoreLeaves miniMax(GameState curState, int ply, int alpha, int beta, int abPrune, int nullPrune, int forwardPrune)
{
    double bestScore = getTurn(curState) ? -__DBL_MAX__ : __DBL_MAX__;
    double score = bestScore;
    int numMoves = 0, moveCounter;
    Move bestMove = -1, nullMove = 0;
    GameState newState, originalState = curState;
    Move *legalMoves = generateLegalMoves(&curState, &numMoves);
    moveScoreLeaves finalMoveInfo, temp;
    finalMoveInfo.leaves = 0, temp.leaves = 0;

    if (!ply)
    {
        finalMoveInfo.leaves = numMoves + nullPrune - (numMoves > forwardPrune ? forwardPrune : 0);
    }

    if (nullPrune && (getTurn(curState) ? !wInCheck(curState) : !bInCheck(curState)))
    {
        setSource(nullMove, LSB(curState.bb[B_KING - 6 * getTurn(curState)]) - 1);
        setDestination(nullMove, LSB(curState.bb[B_KING - 6 * getTurn(curState)]) - 1);
        setCapturedPiece(nullMove, 12);
        setMovedPiece(nullMove, 12);
        newState = pushMove(&curState, nullMove);
        if (ply == 0)
        {
            score = pieceCount(curState.bb);
        }
        else
        {
            temp = miniMax(newState, ply - 1, alpha, beta, abPrune, nullPrune, forwardPrune);
            finalMoveInfo.leaves += temp.leaves;
            score = temp.score;
        }
        bestScore = score;
        bestMove = nullMove;
    }

    if (forwardPrune) // numMoves > forwardPrune
    {
        // selection sort
        for (int i = 0; i < (forwardPrune < numMoves ? forwardPrune : numMoves); i++)
        {
            int max_idx = i;
            for (int j = i + 1; j < numMoves; j++)
            {
                if (pieceCount(pushMove(&curState, legalMoves[j]).bb) > pieceCount(pushMove(&curState, legalMoves[max_idx]).bb))
                {
                    max_idx = j;
                }
            }
            Move temp = legalMoves[max_idx];
            legalMoves[max_idx] = legalMoves[i];
            legalMoves[i] = temp;
        }
        numMoves = forwardPrune < numMoves ? forwardPrune : numMoves;
    }

    for (moveCounter = 0; moveCounter < numMoves; moveCounter++)
    {
        newState = pushMove(&curState, legalMoves[moveCounter]);
        if (ply == 0)
        {
            score = pieceCount(newState.bb);
        }
        else
        {
            // get score from recursive call
            temp = miniMax(newState, ply - 1, alpha, beta, abPrune, nullPrune, forwardPrune);
            finalMoveInfo.leaves += temp.leaves;
            score = temp.score;
        }
        if (((bestScore == -__DBL_MAX__ || score > bestScore) && getTurn(curState)) || ((bestScore == __DBL_MAX__ || score < bestScore) && !getTurn(curState)))
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
            finalMoveInfo.move = bestMove;
            finalMoveInfo.score = bestScore;
            free(legalMoves);
            return finalMoveInfo;
        }
    }
    free(legalMoves);
    if (bestMove != -1)
    {
        finalMoveInfo.move = bestMove;
        finalMoveInfo.score = bestScore;
    }
    else
    {
        finalMoveInfo.move = -1;
        finalMoveInfo.score = getTurn(newState) ? -__DBL_MAX__ : __DBL_MAX__;
    }
    // if the null move was the best move, research without null pruning
    if (nullPrune && nullMove == finalMoveInfo.move)
    {
        temp = miniMax(originalState, ply, alpha, beta, abPrune, 0, forwardPrune);
        temp.leaves += finalMoveInfo.leaves;
        return temp;
    }
    return finalMoveInfo;
}
