/**
 * bitboard.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-02-16
 * @see bitboard.h
 */

#include "bitboard.h"
#include "piece.h"

bitmask FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
        RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
        LIGHT_SQUARES, DARK_SQUARES, ALL_SQUARES, NO_SQUARES, EDGES,
        FILES[8], RANKS[8], SQUARES[64],
        TOP_RANKS[9], BOTTOM_RANKS[9], LEFT_FILES[9], RIGHT_FILES[9];

void bitboardInit() {
    int i, j;

    // Calculate ranks, files, and squares
    for(i=0; i<8; i++) {
        FILES[i] = 0x0101010101010101ULL << i;
        RANKS[i] = 0x00000000000000FFULL << 8 * i;
    }

    for(i=0; i<9; i++) {
        TOP_RANKS[i] = NO_SQUARES;
        BOTTOM_RANKS[i] = NO_SQUARES;
        LEFT_FILES[i] = NO_SQUARES;
        RIGHT_FILES[i] = NO_SQUARES;
        for(j=0; j<i; j++) {
            TOP_RANKS[i] |= RANKS[7 - j];
            BOTTOM_RANKS[i] |= RANKS[j];
            LEFT_FILES[i] |= FILES[j];
            RIGHT_FILES[i] |= FILES[7 - j];
        }
    }

    for(i=0; i<64; i++) {
        SQUARES[i] = 1ULL << i;
    }

    FILE_A = FILES[0];
    FILE_B = FILES[1];
    FILE_C = FILES[2];
    FILE_D = FILES[3];
    FILE_E = FILES[4];
    FILE_F = FILES[5];
    FILE_G = FILES[6];
    FILE_H = FILES[7];

    RANK_1 = RANKS[0];
    RANK_2 = RANKS[1];
    RANK_3 = RANKS[2];
    RANK_4 = RANKS[3];
    RANK_5 = RANKS[4];
    RANK_6 = RANKS[5];
    RANK_7 = RANKS[6];
    RANK_8 = RANKS[7];

    NO_SQUARES = 0ULL;
    ALL_SQUARES = ~NO_SQUARES;
    DARK_SQUARES = 0xAA55AA55AA55AA55ULL;
    LIGHT_SQUARES = ~DARK_SQUARES;
    EDGES = FILE_A | FILE_H | RANK_1 | RANK_8;
}

bitmask shiftLeft(bitmask bm, int n) {
    return (bm & ~LEFT_FILES[n]) >> n;
}

bitmask shiftRight(bitmask bm, int n) {
    return (bm & ~RIGHT_FILES[n]) << n;
}

bitmask shiftUp(bitmask bm, int n) {
    return bm << 8 * n;
}

bitmask shiftDown(bitmask bm, int n) {
    return bm >> 8 * n;
}

bitmask getBlockers(bitmask *bb) {
    int i;
    bitmask blockers = NO_SQUARES;
    for(i=0; i<NUM_PIECES; i++) {
        blockers |= bb[i];
    }
    return blockers;
}
