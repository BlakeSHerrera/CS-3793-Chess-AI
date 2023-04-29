/**
 * move.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-03-01
 * @see move.h
 */

#include "move.h"
#include "piece.h"

#include <math.h>
#include <stdio.h>

void toLAN(Move m, char *szBuffer) {
    szBuffer[0] = getFile(getSource(m)) + 'a';
    szBuffer[1] = getRank(getSource(m)) + '1';
    szBuffer[2] = getFile(getDestination(m)) + 'a';
    szBuffer[3] = getRank(getDestination(m)) + '1';
    szBuffer[4] = isPromotion(m) ? PIECE_STR[getPromotionPiece(m)] : '\0';
    szBuffer[5] = '\0';
}

GameState pushMove(GameState *state, Move m) {
    return pushMoveVerbose(state, getSource(m), getDestination(m),
        getMovedPiece(m), getCapturedPiece(m), isEP(m), isCastling(m),
        isPromotion(m) ? getPromotionPiece(m) : NUM_PIECES);
}

GameState pushMoveVerbose(GameState *state, Square source, Square destination,
        int movedPiece, int capturedPiece, int isEP, int isCastling, int promotion) {
    GameState nextState = *state;
    if(source == destination) {
        // null move
        setTurn(nextState, !getTurn(nextState));
        nextState.prev = state;
        setHasEPTarget(nextState, 0);
        setHalfMoveCounter(nextState, getHalfMoveCounter(nextState) + 1);
        setFullMoveCounter(nextState,
            getFullMoveCounter(nextState) + getTurn(nextState));
        return nextState;
    }

    // Remove source piece, place source piece
    nextState.bb[movedPiece] ^=
        1ULL << source | 1ULL << destination;
    nextState.bb[BLOCKERS] ^= 1ULL << source;

    if(promotion != NUM_PIECES) {
        nextState.bb[movedPiece] ^= 1ULL << destination;
        nextState.bb[promotion] ^= 1ULL << destination;
    }

    // Remove captured piece + check for EP
    nextState.bb[capturedPiece] &= ~(1ULL << destination);
    nextState.bb[BLOCKERS] |= 1ULL << destination;
    if(isEP) {
        nextState.bb[capturedPiece] &= ~(1ULL << (destination + 8 + getTurn(*state) * -16));
        nextState.bb[BLOCKERS] &= ~(1ULL << (destination + 8 + getTurn(*state) * -16));
    }

    // Check for castling, move rook
    if(isCastling) {
        switch(destination) {
        case C1:
            nextState.bb[W_ROOK] ^= (1ULL << A1) | (1ULL << D1);
            nextState.bb[BLOCKERS] ^= (1ULL << A1) | (1ULL << D1);
            break;
        case G1:
            nextState.bb[W_ROOK] ^= (1ULL << H1) | (1ULL << F1);
            nextState.bb[BLOCKERS] ^= (1ULL << H1) | (1ULL << F1);
            break;
        case C8:
            nextState.bb[B_ROOK] ^= (1ULL << A8) | (1ULL << D8);
            nextState.bb[BLOCKERS] ^= (1ULL << A8) | (1ULL << D8);
            break;
        case G8:
            nextState.bb[B_ROOK] ^= (1ULL << H8) | (1ULL << F8);
            nextState.bb[BLOCKERS] ^= (1ULL << H8) | (1ULL << F8);
            break;
        }
    }

    // Update turn, prev state, full move counter
    setTurn(nextState, !getTurn(nextState));
    nextState.prev = state;
    setFullMoveCounter(nextState,
        getFullMoveCounter(nextState) + getTurn(nextState));

    // Update castling rights
    if(movedPiece == W_KING || source == A1 || destination == A1) {
        setWCanCastleQ(nextState, 0);
    }
    if(movedPiece == W_KING || source == H1 || destination == H1) {
        setWCanCastleK(nextState, 0);
    }
    if(movedPiece == B_KING || source == A8 || destination == A8) {
        setBCanCastleQ(nextState, 0);
    }
    if(movedPiece == B_KING || source == H8 || destination == H8) {
        setBCanCastleK(nextState, 0);
    }

    // Update EP target
    if(movedPiece == W_PAWN && destination - source == 16) {
        setEPTarget(nextState, source + 8);
    } else if(movedPiece == B_PAWN && destination - source == -16) {
        setEPTarget(nextState, source - 8);
    } else {
        setHasEPTarget(nextState, 0);
    }

    // Update half move counter
    setHalfMoveCounter(nextState,
        movedPiece == W_PAWN ||
        movedPiece == B_PAWN ||
        capturedPiece != NUM_PIECES ?
        0 : getHalfMoveCounter(nextState) + 1);

    return nextState;
}

GameState pushLAN(GameState *state, const char *szLAN) {
    Square source, destination;
    int movedPiece, capturedPiece, isEP, castling, promotion;

    source = szLAN[0] - 'a' + 8 * (szLAN[1] - '1');
    destination = szLAN[2] - 'a' + 8 * (szLAN[3] - '1');
    movedPiece = getPieceFromSquare(*state, source);
    isEP = hasEPTarget(*state) && destination == getEPTarget(*state) && (
            movedPiece == W_PAWN || movedPiece == B_PAWN);
    capturedPiece = isEP ? W_PAWN + 6 * getTurn(*state) :
        getPieceFromSquare(*state, destination);
    castling = (movedPiece == W_KING || movedPiece == B_KING) &&
            abs(source - destination) == 2;
    promotion = szLAN[4] == '\0' ? NUM_PIECES :
        CHAR_TO_PIECE[(int) szLAN[4]] + getTurn(*state) * -6;

    return pushMoveVerbose(
        state, source, destination, movedPiece, capturedPiece,
        isEP, castling, promotion);
}
