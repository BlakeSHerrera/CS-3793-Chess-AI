/**
 * search.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Hezron Perez, Blake Herrera
 * @date 2023-03-12
 * @see piece.h
 */

#include "search.h"
#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "config.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

Move getRandomMove(GameState state) {
    int n;
    Move moveBuffer[MAX_MOVES];
    generateLegalMoves(&state, moveBuffer, &n);
    return moveBuffer[rand() % n];
}

moveScoreLeaves miniMax(GameState curState, int ply, double alpha, double beta, double prevScore) {
    double staticScore, bestScore, jScore;
    int numMoves, i, j, stop, maxIdx, turn;
    Move bestMove = -1,
         secondBestMove = -1,
         legalMoves[MAX_MOVES],
         tempMove;
    moveScoreLeaves finalMoveInfo, temp;

    pthread_testcancel();
    generateLegalMoves(&curState, legalMoves, &numMoves);
    staticScore = evaluationFunction(curState);
    turn = getTurn(curState);

    if(numMoves == 0) {
        finalMoveInfo.leaves = 1;
        if (turn ? wInCheck(curState) : bInCheck(curState)) {
            finalMoveInfo.score = turn ? -DBL_MAX : DBL_MAX;
        } else {
            finalMoveInfo.score = 0;
        }
        return finalMoveInfo;
    }

    if(ply <= -quiescenceMaxDepth ||
       (ply <= 0 && (searchStrategy != MINIMAX_QUIESCENCE ||
            fabs(staticScore - prevScore) < quiescenceCutoff))) {
        finalMoveInfo.leaves = 1;
        finalMoveInfo.score = staticScore;
        return finalMoveInfo;
    }

    finalMoveInfo.leaves = 0;
    temp.leaves = 0;

    if(pruning & NULL_PRUNING &&
       (turn ? !wInCheck(curState) : !bInCheck(curState))) {
        temp = miniMax(pushMove(&curState, NULL_MOVE),
                       ply - 1, alpha, beta, staticScore);
        finalMoveInfo.leaves += temp.leaves;
        bestMove = NULL_MOVE;
        if(turn) {
            if(temp.score > alpha) {
                alpha = temp.score;
            }
        } else if(temp.score < beta) {
            beta = temp.score;
        }
        // TODO what should happen here if alpha >= beta?
    }

    // TODO: optimize by saving pushed state
    stop = (pruning & FORWARD_PRUNING) && forwardPruneN < numMoves ? forwardPruneN : numMoves;
    for(i=0; i<stop; i++) {
        if(pruning & FORWARD_PRUNING) {
            maxIdx = i;
            bestScore = evaluationFunction(pushMove(&curState, legalMoves[maxIdx]));
            for(j=i+1; j<numMoves; j++) {
                jScore = evaluationFunction(pushMove(&curState, legalMoves[j]));
                if(turn ? jScore > bestScore : jScore < bestScore) {
                    maxIdx = j;
                    bestScore = jScore;
                }
            }
            tempMove = legalMoves[maxIdx];
            legalMoves[maxIdx] = legalMoves[i];
            legalMoves[i] = tempMove;
        }

        // get score from recursive call
        temp = miniMax(pushMove(&curState, legalMoves[i]),
                       ply - 1, alpha, beta, staticScore);

        finalMoveInfo.leaves += temp.leaves;

        if(turn) {
            if(temp.score > alpha) {
                alpha = temp.score;
                secondBestMove = bestMove;
                bestMove = legalMoves[i];
            }
        } else if(temp.score < beta) {
            beta = temp.score;
            secondBestMove = bestMove;
            bestMove = legalMoves[i];
        }

        if((pruning & AB_PRUNING) && beta <= alpha) {
            finalMoveInfo.move = bestMove;
            finalMoveInfo.score = turn ? alpha : beta;
            return finalMoveInfo;
        }
     }

    finalMoveInfo.move = NULL_MOVE == bestMove ? secondBestMove : bestMove;
    finalMoveInfo.score = turn ? alpha : beta;
    return finalMoveInfo;
}
