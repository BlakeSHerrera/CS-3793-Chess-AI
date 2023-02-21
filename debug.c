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
#include "magic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERFT_NODES 0
#define PERFT_CAPTURES 1
#define PERFT_EP 2
#define PERFT_CASTLES 3
#define PERFT_PROMOTIONS 4
#define PERFT_CHECKS 5
#define PERFT_DISCOVERED_CHECKS 6
#define PERFT_DOUBLE_CHECKS 7
#define PERFT_CHECKMATES 8


const unsigned long long int PERFT[9][10] = {
    {1, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860ULL,
    84998978956ULL, 2439530234167ULL},
    {0, 0, 0, 34, 1576, 82719, 2812008, 108329926, 3523740106ULL,
    125208536153ULL},
    {0, 0, 0, 0, 0, 258, 5248, 319617, 7187977, 319496827},
    {0, 0, 0, 0, 0, 0, 0, 883453, 23605205, 1784356000},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 17334376},
    {0, 0, 0, 12, 469, 27351, 809099, 33103848, 968981593,
    36095901903ULL},
    {0, 0, 0, 0, 0, 6, 329, 18026, 847039, 37101713},
    // Note doubled checks are not counted in discovered checks
    {0, 0, 0, 0, 0, 0, 46, 1628, 147215, 5547231},
    {0, 0, 0, 0, 8, 347, 10828, 435767, 9852036, 400191963}
};

const char *PERFT2_FEN =
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
const unsigned long long int PERFT2[9][7] = {
    {1, 48, 2039, 97862, 4085603, 193690690, 8031647685ULL},
    {0, 8, 351, 17102, 757163, 35043416, 1558445096},
    {0, 0, 1, 45, 1929, 73365, 3577504},
    {0, 2, 91, 3162, 128013, 4993637, 184513607},
    {0, 0, 3, 993, 25523, 3309887, 92238050},
    {0, 0, 0, 0, 42, 19883, 568417},
    {0, 0, 0, 0, 6, 2637, 54948},
    {0, 0, 0, 1, 43, 30171, 360003}
};

const char *PERFT3_FEN =
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
const unsigned long long int PERFT3[9][9] = {
    {1, 14, 191, 2812, 43238, 674624, 11030083, 17863361, 3009794393},
    {0, 1, 14, 209, 3348, 52051, 940350, 14519036, 267586558},
    {0, 0, 0, 2, 123, 1165, 33325, 294874, 8009239},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 7552, 140024, 6578076},
    {0, 2, 10, 267, 1680, 52950, 452473, 12797406, 135626805},
    {0, 0, 0, 3, 106, 1292, 26067, 370630, 7181487},
    {0, 0, 0, 0, 0, 3, 0, 3612, 1630},
    {0, 0, 0, 0, 17, 0, 2733, 87, 450410}
};

const char *PERFT4_FEN =
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const char *PERFT4_ALT_FEN =
    "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ";
const unsigned long long int PERFT4[9][7] = {
    {1, 6, 264, 9467, 422333, 15833292, 706045033},
    {0, 0, 87, 1021, 131393, 2046173, 210369132},
    {0, 0, 0, 4, 0, 6512, 212},
    {0, 0, 6, 0, 7795, 0, 10882006},
    {0, 0, 48, 120, 60032, 329464, 81102984},
    {0, 0, 10, 38, 15492, 200568, 26973664},
    {0, 0, 0, 0, 0, 0, 0},  // Not calculated
    {0, 0, 0, 0, 0, 0, 0},  // Not calculated
    {0, 0, 0, 22, 5, 50562, 81076}
};

const char *PERFT5_FEN =
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
const unsigned long long int PERFT5[1][6] = {
    {0, 44, 1486, 62379, 2103487, 89941194}
}; // Only nodes are calculated

const char *PERFT6_FEN =
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
const unsigned long long int PERFT6[1][10] = {
    {1, 46, 2076, 89890, 3894594, 164075551, 6923051137ULL,
    287188994746ULL, 11923589843526ULL, 490154852788714ULL}
}; // Only nodes are calculated

void printBitmask(bitmask bm) {
    int i, j;
    for(i=7; i>=0; i--) {
        for(j=0; j<8; j++) {
            printf("%c ", bm >> (8 * i + j) & 1 ? '1' : '.');
        }
        printf("\n");
    }
    printf("\n");
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
    printf("\n");
}

void testBitmasks() {
    int i;
    #define loop(n, x) for(i=0; i<(n); i++) printBitmask((x)[i])
    printf("Testing bitboard init...\n");

    printf("\nFiles A-H:\n");
    printBitmask(FILE_A);
    printBitmask(FILE_B);
    printBitmask(FILE_C);
    printBitmask(FILE_D);
    printBitmask(FILE_E);
    printBitmask(FILE_F);
    printBitmask(FILE_G);
    printBitmask(FILE_H);

    printf("\nFiles A-H (array):\n");
    loop(8, FILES);

    printf("\nLeftmost files:\n");
    loop(9, LEFT_FILES);

    printf("\nRightmost files:\n");
    loop(9, RIGHT_FILES);

    printf("\nRanks 1-8...\n");
    printBitmask(RANK_1);
    printBitmask(RANK_2);
    printBitmask(RANK_3);
    printBitmask(RANK_4);
    printBitmask(RANK_5);
    printBitmask(RANK_6);
    printBitmask(RANK_7);
    printBitmask(RANK_8);

    printf("\nRanks 1-8 (array):\n");
    loop(8, RANKS);

    printf("\nBottommost ranks\n");
    loop(9, BOTTOM_RANKS);

    printf("\nTopmost ranks\n");
    loop(9, TOP_RANKS);

    printf("\nSquares a1-h8:\n");
    loop(64, SQUARES);

    printf("\nSets of Squares\n");
    printf("Dark Squares\n");
    printBitmask(DARK_SQUARES);
    printf("Light Squares\n");
    printBitmask(LIGHT_SQUARES);
    printf("All squares\n");
    printBitmask(ALL_SQUARES);
    printf("No squares\n");
    printBitmask(NO_SQUARES);
    printf("Edges\n");
    printBitmask(EDGES);

    #undef loop
}

void testSumBits() {
    #define test(e, x) printf("%d %d %s\n", e, _sumBits(x), #x);

    test(8, FILE_A);
    test(8, FILE_B);
    test(8, RANK_1);
    test(16, TOP_RANKS[2]);
    test(32, LIGHT_SQUARES);
    test(32, DARK_SQUARES);
    test(0, NO_SQUARES);
    test(64, ALL_SQUARES);
    test(28, EDGES);
    test(32, EDGES ^ LIGHT_SQUARES);
    test(32, EDGES ^ DARK_SQUARES);

    #undef test
}

void testShifts() {
    int i;
    #define test(f, n) \
        printBitmask(f(DARK_SQUARES, n))
    #define loop for(i=0; i<9; i++)
    printf("Left shifts\n");
    loop test(shiftLeft, i);
    printf("\nRight shifts\n");
    loop test(shiftRight, i);
    printf("\nUp shifts\n");
    loop test(shiftUp, i);
    printf("\nDown shifts\n");
    loop test(shiftDown, i);
    #undef test
    #undef loop
}

void testMoveCalculation() {
    #define test(x, y) \
        printf("%s %s\n", #x, #y);\
        printBitmask(_calculateRookMoves(x, y))
    printf("Rook tests\n");
    test(A1, NO_SQUARES);
    test(H8, NO_SQUARES);
    test(C3, NO_SQUARES);
    test(C3, SQUARES[C5]);
    test(C3, SQUARES[F3]);
    test(H8, SQUARES[H5]);
    test(H8, SQUARES[B8]);
    test(E4, DARK_SQUARES);
    test(E4, LIGHT_SQUARES);
    test(E4, EDGES);

    #undef test
    #define test(x, y) \
        printf("%s %s\n", #x, #y);\
        printBitmask(_calculateBishopMoves(x, y))
    printf("\nBishop tests\n");
    test(A1, LIGHT_SQUARES);
    test(B3, DARK_SQUARES);
    test(D4, LIGHT_SQUARES);
    test(E4, DARK_SQUARES);
    test(E4, LIGHT_SQUARES);
    test(E4, SQUARES[F3] | SQUARES[C2] | SQUARES[G6] | SQUARES[B7]);
    #undef test
}

void testRays() {
    int i, j;
    for(i=0; i<8; i++) {
        printf("Direction %d - a1 to h8:\n", i);
        for(j=0; j<NUM_SQUARES; j++) {
            printBitmask(RAYS[i][j]);
            printf("\n");
        }
    }
}

void testRelevantOccupancy() {
    int i;
    printf("Rook, then Bishop relevant occupancy - a1 to h8:\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printBitmask(ROOK_RELEVANT_OCCUPANCY[i]);
        printf("\n");
        printBitmask(BISHOP_RELEVANT_OCCUPANCY[i]);
        printf("\nBishop bits: %d\n", BISHOP_BITS[i]);
    }
}

void testMagic() {
    int i, j;

    // Zero out tables
    memset(ROOK_TABLE, 0, sizeof(bitmask) * NUM_SQUARES * (1 << 12));
    memset(ROOK_TABLE, 0, sizeof(bitmask) * NUM_SQUARES * (1 << 9));

    for(i=0; i<NUM_SQUARES; i++) {
        for(j=0; j<(1<<12); j++) {

        }
        for(j=0; j<(1<<9); j++) {

        }
    }
}

void _testMagicRook(bitmask bm, Square i) {
    int j = ROOK_MAGIC_NUMS[i] * bm >> 52;
    if(ROOK_TABLE[i][j]++) {
        printf("Rook magic failure at i=%d j=%d\n", i, j);
    }
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicRook(bm & ~SQUARES[i], i + 1);
        }
    }
}

void _testMagicBishop(bitmask bm, Square i, int bits) {
    int j = BISHOP_MAGIC_NUMS[i] * bm >> (64 - bits);
    if(BISHOP_TABLE[i][j]++) {
        printf("Bishop magic failure at i=%d j=%d\n", i, j);
    }
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicBishop(bm & ~SQUARES[i], i + 1, bits);
        }
    }
}

void testBitboards() {
    int i;

    printBitmask(0);
    for(i=0; i<64; i++) {
        printBitmask(1ULL << i);
    }
}

void testMovegen() {
    // TODO
}
