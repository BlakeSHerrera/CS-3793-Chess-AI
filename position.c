/**
 * position.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see position.h
 */

#include "position.h"
#include "piece.h"
#include "bitboard.h"
#include "square.h"
#include "movegen.h"
#include "magic.h"
#include "evaluate.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GameState positionFromFen(const char *szFen) {
    const char *a, *b, *c, *d, *e;
    int i=0;
    #define loop() for(; szFen[i] != ' '; i++) {}
    loop(); a = szFen + ++i;  // turn
    loop(); b = szFen + ++i;  // castling rights
    loop(); c = szFen + ++i;  // ep target
    loop(); d = szFen + ++i;  // half move counter
    loop(); e = szFen + ++i;  // full move counter
    #undef loop
    return positionFromFenParts(szFen, a, b, c, d, e);
}

GameState positionFromFenParts(const char *szBoard, const char *szTurn,
        const char *szCastlingRights, const char *szEPTarget,
        const char *szHalfMoveCounter, const char *szFullMoveCounter) {
    int i, j, pieceNum[255];
    char c;
    GameState state;
    state.prev = NULL;
    state.fenInfo = 0;
    for(i=0; i<NUM_PIECES; i++) {
        pieceNum[(int) PIECE_STR[i]] = i;
        state.bb[i] = NO_SQUARES;
    }

    // Set pieces
    for(i=0, j=0, c=szBoard[0];
            c!=' ' && c!='\0';
            c=szBoard[++i]) {
        if(c == '/') {
            // pass
        } else if('0' <= c && c <= '9') {
            j += c - '0';
        } else {
            // FEN is top to bottom
            state.bb[pieceNum[(int) c]] |= 1ULL << ((7-j/8)*8 + j%8);
            j++;
        }
    }
    state.bb[BLOCKERS] = NO_SQUARES;
    for(j=0; j<NUM_PIECES; j++) {
        state.bb[BLOCKERS] |= state.bb[j];
    }
    // Set turn
    setTurn(state, szTurn[0] == 'w');
    // Set castling rights
    for(i=0, c=szCastlingRights[i++];
            c!=' ' && c!='\0';
            c=szCastlingRights[i++]) {
        switch(c) {
        case 'K':
            setWCanCastleK(state, 1);
            break;
        case 'Q':
            setWCanCastleQ(state, 1);
            break;
        case 'k':
            setBCanCastleK(state, 1);
            break;
        case 'q':
            setBCanCastleQ(state, 1);
            break;
        default:
            break;
        }
    }
    // Set ep target
    if(szEPTarget[0] != '-') {
        setHasEPTarget(state, 1);
        setEPTarget(state, szEPTarget[0]-'a' + 8 * (szEPTarget[1]-'1'));
    }
    // Set half and full move counters
    setHalfMoveCounter(state, atoi(szHalfMoveCounter));
    setFullMoveCounter(state, atoi(szFullMoveCounter));

    // Set material
    setMaterialScore(&state);

    return state;
}

void positionToFen(GameState state, char *szFenBuffer) {
    int i, j, k, blanks, found;

    // Set pieces
    for(i=j=blanks=0; j<64; j++) {
        if(j && !(j%8)) {
            if(blanks) {
                szFenBuffer[i++] = (char) (blanks + '0');
                blanks = 0;
            }
            szFenBuffer[i++] = '/';
        }
        for(k=found=0; k<NUM_PIECES; k++) {
            if(state.bb[k] & 1ULL << ((7 - j/8) * 8 + j%8)) {
                if(blanks > 0) {
                    szFenBuffer[i++] = (char) (blanks + '0');
                    blanks = 0;
                }
                found++;
                szFenBuffer[i++] = PIECE_STR[k];
            }
        }
        blanks += !found;
    }
    if(blanks != 0) {
        szFenBuffer[i++] = '0' + blanks;
    }

    // Set turn
    szFenBuffer[i++] = ' ';
    szFenBuffer[i++] = getTurn(state) ? 'w' : 'b';

    // Set castling rights
    szFenBuffer[i++] = ' ';
    if(getCastlingRights(state)) {
        if(wCanCastleK(state)) {
            szFenBuffer[i++] = 'K';
        }
        if(wCanCastleQ(state)) {
            szFenBuffer[i++] = 'Q';
        }
        if(bCanCastleK(state)) {
            szFenBuffer[i++] = 'k';
        }
        if(bCanCastleQ(state)) {
            szFenBuffer[i++] = 'q';
        }
    } else {
        szFenBuffer[i++] = '-';
    }

    // Set ep target
    szFenBuffer[i++] = ' ';
    if(hasEPTarget(state)) {
        szFenBuffer[i++] = "abcdefgh"[getEPTarget(state) % 8];
        szFenBuffer[i++] = "12345678"[getEPTarget(state) / 8];
    } else {
        szFenBuffer[i++] = '-';
    }

    // Set up move counters
    sprintf(szFenBuffer + i, " %d %d",
        getHalfMoveCounter(state),
        getFullMoveCounter(state));
}

int is50MoveRule(GameState state) {
    return getHalfMoveCounter(state) >= 100;
}

int is3FoldRepetition(GameState state) {
    // TODO
    return 0;
}

int isIllegalPosition(GameState state) {
    return sumBits(state.bb[W_KING]) != 1 ||
        sumBits(state.bb[B_KING]) != 1 ||
        state.bb[W_PAWN] | state.bb[B_PAWN] | RANK_1 | RANK_8 ||
        getTurn(state) ? bInCheck(state) : wInCheck(state);
}

#define u(x) shiftUp((x), 1)
#define d(x) shiftDown((x), 1)
#define l(x) shiftLeft((x), 1)
#define r(x) shiftRight((x), 1)
int bAttacks(GameState state, Square sq) {
    return
        state.bb[B_PAWN] & (
            u(l(1ULL << sq)) |
            u(r(1ULL << sq))) ||
        KNIGHT_TABLE[sq] & state.bb[B_KNIGHT] ||
        KING_TABLE[sq] & state.bb[B_KING] ||
        hashRook(sq, state.bb[BLOCKERS]) &
            (state.bb[B_ROOK] | state.bb[B_QUEEN]) ||
        hashBishop(sq, state.bb[BLOCKERS]) &
            (state.bb[B_BISHOP] | state.bb[B_QUEEN]);
}

int wAttacks(GameState state, Square sq) {
    return
        state.bb[W_PAWN] & (
            d(l(1ULL << sq)) |
            d(r(1ULL << sq))) ||
        KNIGHT_TABLE[sq] & state.bb[W_KNIGHT] ||
        KING_TABLE[sq] & state.bb[W_KING] ||
        hashRook(sq, state.bb[BLOCKERS]) &
            (state.bb[W_ROOK] | state.bb[W_QUEEN]) ||
        hashBishop(sq, state.bb[BLOCKERS]) &
            (state.bb[W_BISHOP] | state.bb[W_QUEEN]);
}
#undef u
#undef d
#undef l
#undef r

int wInCheck(GameState state) {
    return bAttacks(state, LSB(state.bb[W_KING]) - 1);
}

int bInCheck(GameState state) {
    return wAttacks(state, LSB(state.bb[B_KING]) - 1);
}

bitmask getAllAttacks(GameState state, int white) {
    int square;
    bitmask bm, attacks = 0ULL;
    white = !!white;

    for(bm = state.bb[B_ROOK - 6 * white] |
            state.bb[B_QUEEN - 6 * white];
            bm;
            bm &= ~(1ULL << square)) {
        square = LSB(bm) - 1;
        attacks |= hashRook(square, state.bb[BLOCKERS]);
    }

    for(bm = state.bb[B_BISHOP - 6 * white] |
            state.bb[B_QUEEN - 6 * white];
            bm;
            bm &= ~(1ULL << square)) {
        square = LSB(bm) - 1;
        attacks |= hashBishop(square, state.bb[BLOCKERS]);
    }

    for(bm = state.bb[B_KNIGHT - 6 * white];
            bm;
            bm &= ~(1ULL << square)) {
        square = LSB(bm) - 1;
        attacks |= KNIGHT_TABLE[square];
    }

    for(bm = state.bb[B_KING - 6 * white];
            bm;
            bm &= ~(1ULL << square)) {
        square = LSB(bm) - 1;
        attacks |= KING_TABLE[square];
    }

    for(bm = state.bb[B_PAWN - 6 * white];
            bm;
            bm &= ~(1ULL << square)) {
        square = LSB(bm) - 1;
        attacks |= PAWN_TABLE[white][square];
    }

    return attacks;
}

int getPieceFromSquare(GameState state, Square square) {
    return getPieceFromBitmask(state, 1ULL << square);
}

int getPieceFromBitmask(GameState state, bitmask square) {
    int i;
    for(i=0; i<NUM_PIECES; i++) {
        if(state.bb[i] & square) {
            return i;
        }
    }
    return i;
}
