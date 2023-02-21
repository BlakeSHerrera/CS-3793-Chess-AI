/**
 * movegen.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see movegen.h
 */

#include "movegen.h"
#include "square.h"
#include "bitboard.h"
#include "debug.h"
#include "magic.h"

#include <stdlib.h>

bitmask RAYS[8][64],
        BISHOP_RELEVANT_OCCUPANCY[NUM_SQUARES],
        ROOK_RELEVANT_OCCUPANCY[NUM_SQUARES],
        BISHOP_TABLE[64][1 << 9],
        ROOK_TABLE[64][1 << 12];

int BISHOP_BITS[NUM_SQUARES];

void movegenInit() {
    int i, j, k;
    bitmask pos;

    // Calculate ray casts
    for(i=0; i<8; i++) { // direction
        for(j=0; j<64; j++) { // square
            RAYS[i][j] = 0ULL;
            for(k=1; k<8; k++) { // shifts
                pos = SQUARES[j];
                if(i == NORTH || i == NORTHEAST || i == NORTHWEST)
                    pos = shiftUp(pos, k);
                if(i == WEST || i == NORTHWEST || i == SOUTHWEST)
                    pos = shiftLeft(pos, k);
                if(i == SOUTH || i == SOUTHWEST || i == SOUTHEAST)
                    pos = shiftDown(pos, k);
                if(i == EAST || i == SOUTHEAST || i == NORTHEAST)
                    pos = shiftRight(pos, k);
                RAYS[i][j] |= pos;
            }
        }
    }

    // The edge of the ray is not a blocker
    for(i=0; i<NUM_SQUARES; i++) {
        ROOK_RELEVANT_OCCUPANCY[i] =
            (RAYS[NORTH][i] & ~RANK_8) |
            (RAYS[WEST][i]  & ~FILE_A) |
            (RAYS[SOUTH][i] & ~RANK_1) |
            (RAYS[EAST][i]  & ~FILE_H);
        BISHOP_RELEVANT_OCCUPANCY[i] =  ~EDGES & (
            RAYS[NORTHWEST][i] |
            RAYS[SOUTHWEST][i] |
            RAYS[SOUTHEAST][i] |
            RAYS[NORTHEAST][i]);
        BISHOP_BITS[i] = _sumBits(BISHOP_RELEVANT_OCCUPANCY[i]);
        // Calculate magic bitboards
        //_initMagicRook(ROOK_RELEVANT_OCCUPANCY[i], 0, 0);
        //_initMagicBishop(BISHOP_RELEVANT_OCCUPANCY[i], 0, 0, BISHOP_BITS[i]);
    }
}

void _initMagicRook(bitmask bm, Square i) {
    ROOK_TABLE[i][ROOK_MAGIC_NUMS[i] * bm >> 52] = _calculateRookMoves(i, bm);
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicRook(bm & ~SQUARES[i], i + 1);
        }
    }
}

void _initMagicBishop(bitmask bm, Square i, int bits) {
    BISHOP_TABLE[i][BISHOP_MAGIC_NUMS[i] * bm >> (64 - bits)] = _calculateBishopMoves(i, bm);
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicBishop(bm & ~SQUARES[i], i + 1, bits);
        }
    }
}

bitmask _calculateRookMoves(Square square, bitmask blockers) {
    int row, col, r, c, i;
    bitmask attacks = NO_SQUARES;
    row = square / 8;
    col = square % 8;

    for(r=row+1; r<8; r++) {
        i = r * 8 + col;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
    }
    for(r=row-1; r>=0; r--) {
        i = r * 8 + col;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
    }
    for(c=col+1; c<8; c++) {
        i = row * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
    }
    for(c=col-1; c>=0; c--) {
        i = row * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
    }
    return attacks;
}

bitmask _calculateBishopMoves(Square square, bitmask blockers) {
    int row, col, r, c, i;
    bitmask attacks = NO_SQUARES;
    row = square / 8;
    col = square % 8;

    for(r=row+1, c=col+1; r<8 && c<8; ) {
        i = r * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
        r++; c++;
    }
    for(r=row+1, c=col-1; r<8 && c>=0; ) {
        i = r * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
        r++; c--;
    }
    for(r=row-1, c=col+1; r>=0 && c<8; ) {
        i = r * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
        r--; c++;
    }
    for(r=row-1, c=col-1; r>=0 && c>=0; ) {
        i = r * 8 + c;
        attacks |= SQUARES[i];
        if(blockers & SQUARES[i])
            break;
        r--; c--;
    }
    return attacks;
}

int _sumBits(bitmask bm) {
    int i;
    for(i=0; bm; i+=bm&1, bm>>=1);
    return i;
}

Move *generatePseudoLegalMoves(GameState *state, int *numMoves) {
    int i;
    Move *moves = malloc(sizeof(Move) * MAX_MOVES);

    // Rook moves

    // TODO
    return moves;
}

