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

 #include <stdlib.h>
 #include <string.h>

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
    // TODO
    return state;
}

void posisitionToFen(GameState state, char *szBuffer) {
    // TODO
}

int is50MoveRule(GameState state) {
    // TODO
    return 0;
}

int is3FoldRepetition(GameState state) {
    // TODO
    return 0;
}

int isIllegalPosition(GameState state) {
    // TODO
    return 0;
}
