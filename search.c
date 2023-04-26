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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

Move getRandomMove(GameState state) {
    int n;
    Move moveBuffer[MAX_MOVES];
    generateLegalMoves(&state, moveBuffer, &n);
    return moveBuffer[rand() % n];
}

moveScoreLeaves miniMax(GameState curState, int ply, double alpha, double beta, int nullPrune) {
    double bestScore = getTurn(curState) ? -__DBL_MAX__ : __DBL_MAX__;
    int numMoves = 0, moveCounter;
    Move bestMove = -1;
    GameState newState, originalState = curState, tempMove[2];
    Move legalMoves[MAX_MOVES];
    moveScoreLeaves finalMoveInfo, temp;

    pthread_testcancel();
    generateLegalMoves(&curState, legalMoves, &numMoves);

    if (numMoves == 0) {
        finalMoveInfo.leaves = 1;
        if (getTurn(curState) ? wInCheck(curState) : bInCheck(curState)) {
            finalMoveInfo.score = bestScore;
        } else {
            finalMoveInfo.score = 0;
        }
        return finalMoveInfo;
    }

    if (!ply) {
        finalMoveInfo.leaves = 1;
        finalMoveInfo.score = evaluationFunction(curState);
        return finalMoveInfo;
    }

    finalMoveInfo.leaves = 0, temp.leaves = 0;

    if (nullPrune && (getTurn(curState) ? !wInCheck(curState) : !bInCheck(curState))) {
        newState = pushMove(&curState, NULL_MOVE);
        temp = miniMax(newState, ply - 1, alpha, beta, nullPrune);
        finalMoveInfo.leaves += temp.leaves;
        bestScore = temp.score; // set alpha if white, beta if black ...
        bestMove = NULL_MOVE;
        if(getTurn(curState)) {
            alpha = bestScore > alpha ? bestScore : alpha;
        } else {
            beta = bestScore < beta ? bestScore : beta;
        }
    }

    /* TODO: this code block can be optimized by performing the
     * ab search on each node as it becomes sorted.
     */
    if (pruning & FORWARD_PRUNING) {  // numMoves > forwardPrune
        // selection sort
        int stop = forwardPruneN < numMoves ? forwardPruneN : numMoves;
        for (int i = 0; i < stop; i++) {
            int max_idx = i;
            for (int j = i + 1; j < numMoves; j++) {
                tempMove[0] = pushMove(&curState, legalMoves[j]);
                tempMove[1] = pushMove(&curState, legalMoves[max_idx]);
                if (evaluationFunction(tempMove[0]) > evaluationFunction(tempMove[1])) {
                    max_idx = j;
                }
            }
            Move temp = legalMoves[max_idx];
            legalMoves[max_idx] = legalMoves[i];
            legalMoves[i] = temp;
        }
        numMoves = stop;
    }

    for (moveCounter = 0; moveCounter < numMoves; moveCounter++) {
        newState = pushMove(&curState, legalMoves[moveCounter]);
        // get score from recursive call
        temp = miniMax(newState, ply - 1, alpha, beta, nullPrune);

        finalMoveInfo.leaves += temp.leaves;
        // if other player will be put in check, take this path
        if (temp.move == -1) {
            finalMoveInfo.move = legalMoves[moveCounter];
            finalMoveInfo.score = temp.score;
            return finalMoveInfo;
        }
        if (getTurn(curState) ? temp.score >= bestScore : temp.score <= bestScore) {
            bestScore = temp.score;
            bestMove = legalMoves[moveCounter];
        }

        // TODO ab pruning
        if (!(pruning & AB_PRUNING)) {
            continue;
        }
        if (getTurn(curState)) {
            alpha = alpha > temp.score ? alpha : temp.score;
        } else {
            beta = beta < temp.score ? beta : temp.score;
        }

        if (beta <= alpha) {
            finalMoveInfo.move = bestMove;
            finalMoveInfo.score = getTurn(curState) ? alpha : beta;
            return finalMoveInfo;
        }
    }

    if (bestMove != -1) {
        finalMoveInfo.move = bestMove;
        finalMoveInfo.score = bestScore;
    } else {
        finalMoveInfo.move = -1;
        finalMoveInfo.score = getTurn(newState) ? -__DBL_MAX__ : __DBL_MAX__;
    }
    // if the null move was the best move, research without null pruning
    if (NULL_MOVE == finalMoveInfo.move) {
        temp = miniMax(originalState, ply, alpha, beta, 0);
        temp.leaves += finalMoveInfo.leaves;
        return temp;
    }
    return finalMoveInfo;
}
