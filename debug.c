/**
 * debug.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see debug.h
 */

#include "bitboard.h"
#include "debug.h"
#include "move.h"
#include "movegen.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

void printBitmask(bitmask bm) {
    int i, j;
    for(i=7; i>=0; i--) {
        for(j=0; j<8; j++) {
            printf("%llu ", bm >> (8 * i + j) & 1);
        }
        printf("\n");
    }
}

// Function is somewhat inefficient to highlight bugs
void printBitboard(bitmask *bb) {
    int i, j, k, found;
    for(i=7; i>=0; i--) {
        for(j=0; j<8; j++) {
            found = 0;
            for(k=0; k<NUM_PIECES; k++) {
                if(bb[k] & 1 << (8 * i + j)) {
                    printf("%c", PIECE_STR[k]);
                    found = 1;
                }
            }
            if(!found) {
                printf(".");
            }
            printf(" ");
        }
        printf("\n");
    }
}
