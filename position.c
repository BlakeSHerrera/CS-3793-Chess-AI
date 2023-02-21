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

 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>

GameState positionFromFen(char *szFen) {
    int i, j, pieceNum[NUM_PIECES];
    char c;
    GameState state;
    state.prev = NULL;
    state.fenInfo = 0;

    for(i=0; i<NUM_PIECES; i++) {
        pieceNum[(int) PIECE_STR[i]] = i;
        state.bb[i] = NO_SQUARES;
    }

    // Set pieces
    for(i=0, j=0, c=szFen[0]; c!=' '; c=szFen[++i]) {
        if(c == '/') {
            // pass
        } else if('0' <= c && c <= '9') {
            j += c - '0' + 1;
        } else {
            // FEN is top to bottom
            state.bb[pieceNum[(int) c]] |= 1ULL << ((7-j/8)*8 + j%8);
            j++;
        }
    }
    state.bb[BLOCKERS] = NO_SQUARES;
    for(i=0; i<NUM_PIECES; i++) {
        state.bb[BLOCKERS] |= state.bb[i];
    }

    // Set turn
    state.fenInfo |= szFen[++i] == 'w';
    ++i;

    // Set castling rights
    for(c=szFen[++i]; c!=' '; c=szFen[++i]) {
        switch(c) {
        case 'K':
            state.fenInfo |= 1 << 1;
            break;
        case 'Q':
            state.fenInfo |= 1 << 2;
            break;
        case 'k':
            state.fenInfo |= 1 << 3;
            break;
        case 'q':
            state.fenInfo |= 1 << 4;
            break;
        default:
            break;
        }
    }

    // Set ep target
    c = szFen[++i];
    if(c != '-') {
        state.fenInfo |= 1 << 5;
        state.fenInfo |= ((c-'a')*8 + szFen[++i]-'1') << 6;
    }
    i += 2;

    // Set half move counter
    state.fenInfo |= atoi(szFen + i) << 12;
    while(szFen[++i] != ' ');
    ++i;

    // Set up full move counter
    state.fenInfo |= atoi(szFen + i) << 19;

    return state;
}

void posisitionToFen(GameState state, char *szFenBuffer) {
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
            if(state.bb[k] & 1 << ((7-j/8)*8 + j%8)) {
                if(blanks > 0) {
                    szFenBuffer[i++] = (char) (blanks + '0');
                    blanks = 0;
                    found++;
                }
            }
        }
        blanks += !found;
    }

    // Set turn
    szFenBuffer[i++] = ' ';
    szFenBuffer[i++] = state.fenInfo & 1 ? 'w' : 'b';

    // Set castling rights
    szFenBuffer[i++] = ' ';
    if(state.fenInfo & 0b11110) {
        for(j=1; j<6; j++) {
            if(state.fenInfo & (1 << j)) {
                szFenBuffer[i++] = "KQkq"[j - 1];
            }
        }
    } else {
        szFenBuffer[i++] = '-';
    }

    // Set ep target
    szFenBuffer[i++] = ' ';
    if(state.fenInfo & (1 << 5)) {
        szFenBuffer[i++] = "abcdefgh"[(state.fenInfo >> 6 & 63) % 8];
        szFenBuffer[i++] = "12345678"[(state.fenInfo >> 6 & 63) / 8];
    } else {
        szFenBuffer[i++] = '-';
    }

    // Set up move counters
    sprintf(szFenBuffer + i, " %d %d",
        state.fenInfo >> 12 & 127,
        state.fenInfo >> 18);
}

int is50MoveRule(GameState state) {
    return (state.fenInfo >> 12 & 127) >= 50;
}

int is3FoldRepetition(GameState state) {
    // TODO
    return 0;
}

int isIllegalPosition(GameState state) {
    // TODO
    return 0;
}
