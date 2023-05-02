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
#include <time.h>
#include <error.h>

char buffer[256];

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

const unsigned long long int PERFT5[1][6] = {
    {0, 44, 1486, 62379, 2103487, 89941194}
}; // Only nodes are calculated

const unsigned long long int PERFT6[1][10] = {
    {1, 46, 2076, 89890, 3894594, 164075551, 6923051137ULL,
    287188994746ULL, 11923589843526ULL, 490154852788714ULL}
}; // Only nodes are calculated

/* ********************************************************
 * Helper functions                                       *
 **********************************************************/

void add(perftResults *a, perftResults *b) {
    bitmask *c = (bitmask *) a, *d = (bitmask *) b;
    int i;
    for(i=0; i<sizeof(perftResults)/sizeof(bitmask); i++) {
        c[i] += d[i];
    }
}

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
                if(bb[k] & 1ULL << (8 * i + j)) {
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

void printGameState(GameState state) {
    char szFen[255];
    printf("Turn, Castling (qkQK), EP, Half-move, Full-move\n"
           "%d, %d%d%d%d, %d, %d, %d\n",
           getTurn(state), !!bCanCastleQ(state),
           !!bCanCastleK(state), !!wCanCastleQ(state),
           !!wCanCastleK(state), hasEPTarget(state) ? getEPTarget(state) : -1,
           getHalfMoveCounter(state), getFullMoveCounter(state));
    printBitboard(state.bb);
    positionToFen(state, szFen);
    printf("%s\n", szFen);
    printf("Material: %f\n\n", state.material);
}

void printMove(Move m) {
    char szLAN[6];
    toLAN(m, szLAN);
    printf("%s ep:%d castle:%d\n", szLAN, isEP(m), isCastling(m));
}

void printMoveVerbose(GameState state, Move m) {
    // eg, Pb7xra8=Q, Kf1- g1 0-0, Pe5xpd6 ep
    printf("%c%s%c%c%s%s%c%s%s\n",
           PIECE_STR[getMovedPiece(m)],
           SQUARE_TO_STR[getSource(m)],
           getCapturedPiece(m) != NUM_PIECES ? 'x' : '-',
           getCapturedPiece(m) != NUM_PIECES ? PIECE_STR[getCapturedPiece(m)] : ' ',
           SQUARE_TO_STR[getDestination(m)],
           isPromotion(m) ? "=" : "",
           isPromotion(m) ? PIECE_STR[getPromotionPiece(m)] : ' ',
           isCastling(m) ? (getRank(getDestination(m)) == 2 ? "0-0-0" : "0-0") : "",
           isEP(m) ? "ep" : "");
}



/* ********************************************************
 * bitboard tests                                         *
 **********************************************************/

void testBitmasks() {
    int i, j;
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
    loop(NUM_SQUARES, SQUARES);

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

    printf("W Castle K\n");
    printBitmask(W_CASTLE_K);
    printf("W Castle Q\n");
    printBitmask(W_CASTLE_Q);
    printf("B Castle K\n");
    printBitmask(B_CASTLE_K);
    printf("B Castle Q\n");
    printBitmask(B_CASTLE_Q);

    for(j=0; j<8; j++) {
        printf("Ray casts #%d\n", j);
        loop(64, RAYS[j]);
    }
    #undef loop
}

void testSumBits() {
    #define test(e, x) printf("%d %d %s\n", e, sumBits(x), #x)
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



/* ********************************************************
 * position tests                                         *
 **********************************************************/

void testFromFen() {
    #define test(s) \
        printf("Testing FEN: %s\n", s); \
        state = positionFromFen(s); \
        printGameState(state); \
        printBitmask(state.bb[BLOCKERS])
    GameState state;
    state = positionFromFen(PERFT2_FEN);
    printGameState(state);
    printBitmask(state.bb[BLOCKERS]);
    printBitmask(hashRook(A1, state.bb[BLOCKERS]));
    //test(START_FEN);
    //test(PERFT2_FEN);
    //test(PERFT3_FEN);
    //test(PERFT4_FEN);
    //test(PERFT4_ALT_FEN);
    //test(PERFT5_FEN);
    //test(PERFT6_FEN);
    #undef test
}

void testToFen() {
    char szBuffer[256];
    printf("Expected followed by actual\n\n");

    positionToFen(positionFromFen(START_FEN), szBuffer);
    printf("Starting board\n%s\n%s\n\n", START_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT2_FEN), szBuffer);
    printf("PERFT2\n%s\n%s\n\n", PERFT2_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT3_FEN), szBuffer);
    printf("PERFT3\n%s\n%s\n\n", PERFT3_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT4_FEN), szBuffer);
    printf("PERFT4\n%s\n%s\n\n", PERFT4_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT4_ALT_FEN), szBuffer);
    printf("PERFT4 SLT\n%s\n%s\n\n", PERFT4_ALT_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT5_FEN), szBuffer);
    printf("PERFT5\n%s\n%s\n\n", PERFT5_FEN, szBuffer);

    positionToFen(positionFromFen(PERFT6_FEN), szBuffer);
    printf("PERFT6\n%s\n%s\n\n", PERFT6_FEN, szBuffer);
}



/* ********************************************************
 * These tests are for move.                              *
 **********************************************************/

void testPushLAN() {
    #define next(str) \
        states[i + 1] = pushLAN(states + i, str); \
        printf("%s\n", str); \
        printGameState(states[++i])
    int i=0;
    GameState states[20];
    states[0] = positionFromFen(START_FEN);
    printGameState(states[0]);

    next("e2e4"); next("d7d5");
    next("e4d5"); next("c7c6");
    next("d5c6"); next("g8f6");
    next("c6b7"); next("g7g6");
    next("b7a8q"); next("f8g7");
    next("d2d4"); next("e8g8");
    next("d4d5"); next("e7e5");
    next("d5e6");

    #undef next
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

void testKingKnightMoves() {
    int i;
    printf("King moves - a1 to h8:\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printBitmask(KING_TABLE[i]);
    }
    printf("Knight moves - a1 to h8:\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printBitmask(KNIGHT_TABLE[i]);
    }
}

void testRelevantOccupancy() {
    int i;
    printf("Rook relevant occupancy - a1 to h8:\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printBitmask(ROOK_RELEVANT_OCCUPANCY[i]);

    }
    printf("Bishop relevant occupancy - a1 to h8:\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printf("Bishop bits: %d\n", BISHOP_BITS[i]);
        printBitmask(BISHOP_RELEVANT_OCCUPANCY[i]);
    }
}



/* ********************************************************
 * These tests are for magic.                             *
 **********************************************************/

void testMagic() {
    int i, rookCollisions = 0, bishopCollisions = 0;

    // Zero out tables
    memset(ROOK_TABLE, -1, sizeof(bitmask) * NUM_SQUARES * (1 << 12));
    memset(BISHOP_TABLE, -1, sizeof(bitmask) * NUM_SQUARES * (1 << 9));

    for(i=0; i<NUM_SQUARES; i++) {
        rookCollisions += _testMagicRook(
            i, ROOK_RELEVANT_OCCUPANCY[i], 0);
        bishopCollisions += _testMagicBishop(
            i, BISHOP_RELEVANT_OCCUPANCY[i], 0);
    }
    printf("Rook collisions: %d\nBishop collisions: %d\n",
           rookCollisions, bishopCollisions);
}

int _testMagicRook(Square rookSquare, bitmask bm, int i) {
    bitmask prevHash, calculated;
    prevHash = hashRook(rookSquare, bm);
    calculated = _calculateRookMoves(rookSquare, bm);
    // if there was a prev hash and current is different
    if(~prevHash && calculated != prevHash) return 1;
    hashRook(rookSquare, bm) = calculated;
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i] && _testMagicRook(rookSquare, bm & ~SQUARES[i], i + 1)) return 1;
    }
    return 0;
}

int _testMagicBishop(Square bishopSquare, bitmask bm, int i) {
    bitmask prevHash, calculated;
    prevHash = hashBishop(bishopSquare, bm);
    calculated = _calculateBishopMoves(bishopSquare, bm);
    if(~prevHash && calculated != prevHash) return 1;
    hashBishop(bishopSquare, bm) = calculated;
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i] && _testMagicBishop(bishopSquare, bm & ~SQUARES[i], i + 1)) return 1;
    }
    return 0;
}



/* ********************************************************
 * These tests are for movegen.                           *
 **********************************************************/

void testGeneratePseudoLegalMoves() {
    Move moveBuffer[MAX_MOVES];
    int numMoves, i;
    GameState state;
    #define test(s) \
        state = positionFromFen(s); \
        generatePseudoLegalMoves(&state, moveBuffer, &numMoves); \
        printf("Num moves: %d\n", numMoves); \
        for(i=0; i<numMoves; i++) { \
            printMove(moveBuffer[i]); \
            printMoveVerbose(state, moveBuffer[i]); \
        }
    test(START_FEN);
    test(PERFT2_FEN);
    test(PERFT3_FEN);
    test(PERFT4_FEN);
    test(PERFT4_ALT_FEN);
    test(PERFT5_FEN);
    test(PERFT6_FEN);
    #undef test
}

void testGenerateLegalMoves() {
    Move moveBuffer[MAX_MOVES];
    int numMoves, i;
    GameState state;
    #define test(s, verbose) \
        printf("Testing FEN: %s\n", s); \
        state = positionFromFen(s); \
        generateLegalMoves(&state, moveBuffer, &numMoves); \
        printf("Num moves: %d\n", numMoves); \
        if(verbose) { \
            for(i=0; i<numMoves; i++) { \
                printMove(moveBuffer[i]); \
                printMoveVerbose(state, moveBuffer[i]); \
            } \
        } \
        printf("\n");
    test(START_FEN, 0);
    test(PERFT2_FEN, 0);
    test(PERFT3_FEN, 0);
    test(PERFT4_FEN, 0);
    test(PERFT4_ALT_FEN, 0);
    test(PERFT5_FEN, 0);
    test(PERFT6_FEN, 0);
    #undef test
}

perftResults performanceTest(const char *szFen, int depth) {
    clock_t start;
    perftResults results;
    GameState state = positionFromFen(szFen);
    start = clock();
    results = _performanceTest(state, depth);
    results.seconds = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    return results;
}

perftResults _performanceTest(GameState state, int depth) {
    int numMoves, i;
    perftResults res, accumulator = {0};
    Move moveBuffer[MAX_MOVES];
    if(depth <= 0) {
        accumulator.nodes = 1;
        return accumulator;
    }

    generateLegalMoves(&state, moveBuffer, &numMoves);
    for(i=0; i<numMoves; i++) {
        if(depth == 1) {
            if(isEP(moveBuffer[i])) {
                accumulator.ep++;
                accumulator.captures++;
            } else if(getCapturedPiece(moveBuffer[i]) < NUM_PIECES) {
                accumulator.captures++;
            } else if(isCastling(moveBuffer[i])) {
                accumulator.castles++;
            } else {
                accumulator.other++;
            }
            if(isPromotion(moveBuffer[i])) {
                accumulator.promotions++;
            }
            accumulator.nodes++;
        } else {
            res = _performanceTest(pushMove(&state, moveBuffer[i]), depth - 1);
            add(&accumulator, &res);
        }
    }
    return accumulator;
}
