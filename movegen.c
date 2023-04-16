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
#include "position.h"
#include "move.h"

#include <stdlib.h>
#include <stdio.h>

bitmask BISHOP_RELEVANT_OCCUPANCY[NUM_SQUARES],
        ROOK_RELEVANT_OCCUPANCY[NUM_SQUARES],
        BISHOP_TABLE[NUM_SQUARES][1 << 9],
        ROOK_TABLE[NUM_SQUARES][1 << 12],
        KNIGHT_TABLE[NUM_SQUARES],
        KING_TABLE[NUM_SQUARES],  // attacks only
        PAWN_TABLE[2][NUM_SQUARES];  // attacks only

int ROOK_BITS[NUM_SQUARES],
    BISHOP_BITS[NUM_SQUARES];

void movegenInit() {
    int i;
    bitmask pos;

    // The edge of the ray is not a blocker
    for(i=0; i<NUM_SQUARES; i++) {
        ROOK_RELEVANT_OCCUPANCY[i] =
            (RAYS[NORTH][i] & ~RANK_8) |
            (RAYS[WEST][i]  & ~FILE_A) |
            (RAYS[SOUTH][i] & ~RANK_1) |
            (RAYS[EAST][i]  & ~FILE_H);
        ROOK_BITS[i] = sumBits(ROOK_RELEVANT_OCCUPANCY[i]);
        _initMagicRook(i, ROOK_RELEVANT_OCCUPANCY[i], 0);

        BISHOP_RELEVANT_OCCUPANCY[i] =  ~EDGES & (
            RAYS[NORTHWEST][i] |
            RAYS[SOUTHWEST][i] |
            RAYS[SOUTHEAST][i] |
            RAYS[NORTHEAST][i]);
        BISHOP_BITS[i] = sumBits(BISHOP_RELEVANT_OCCUPANCY[i]);
        _initMagicBishop(i, BISHOP_RELEVANT_OCCUPANCY[i], 0);

        pos = 1ULL << i;
        KNIGHT_TABLE[i] =
            shiftUp(shiftLeft(pos, 1), 2)    |
            shiftUp(shiftRight(pos, 1), 2)   |
            shiftUp(shiftLeft(pos, 2), 1)    |
            shiftUp(shiftRight(pos, 2), 1)   |
            shiftDown(shiftLeft(pos, 1), 2)  |
            shiftDown(shiftRight(pos, 1), 2) |
            shiftDown(shiftLeft(pos, 2), 1)  |
            shiftDown(shiftRight(pos, 2), 1);

        pos = 1ULL << i;
        KING_TABLE[i] =
            shiftUp(pos, 1)    |
            shiftDown(pos, 1)  |
            shiftLeft(pos, 1)  |
            shiftRight(pos, 1) |
            shiftUp(shiftLeft(pos, 1), 1)   |
            shiftUp(shiftRight(pos, 1), 1)  |
            shiftDown(shiftLeft(pos, 1), 1) |
            shiftDown(shiftRight(pos, 1), 1);

        pos = 1ULL << i;
        PAWN_TABLE[0][i] =
            shiftDown(shiftLeft(pos, 1), 1) |
            shiftDown(shiftRight(pos, 1), 1);
        PAWN_TABLE[1][i] =
            shiftUp(shiftLeft(pos, 1), 1) |
            shiftUp(shiftLeft(pos, 1), 1);

        // Queen moves calculated from rook and bishop
    }
}

void _initMagicRook(Square rookSquare, bitmask bm, int i) {
    hashRook(rookSquare, bm) = _calculateRookMoves(rookSquare, bm);
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicRook(rookSquare, bm & ~SQUARES[i], i + 1);
        }
    }
}

void _initMagicBishop(Square bishopSquare, bitmask bm, Square i) {
    hashBishop(bishopSquare, bm) = _calculateBishopMoves(bishopSquare, bm);
    for(; i<NUM_SQUARES; i++) {
        if(bm & SQUARES[i]) {
            _initMagicBishop(bishopSquare, bm & ~SQUARES[i], i + 1);
        }
    }
}

bitmask _calculateRookMoves(Square square, bitmask blockers) {
    int row, col, r, c, i;
    bitmask attacks = NO_SQUARES;
    row = getRank(square);
    col = getFile(square);

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

Move *generatePseudoLegalMoves(GameState *state, int *numMoves) {
    int i=0, j, turn, squareSource, squareDestination;
    bitmask movesMask, piecesMask, blockers, bmSource, bmDestination;
    Move nextMove;
    Move *moves = malloc(sizeof(Move) * MAX_MOVES);
    if(moves == NULL) {
        fprintf(stderr, "Error when mallocing moves.\n");
        exit(1);
    }
    blockers = state->bb[BLOCKERS];
    turn = getTurn(*state);

    #define repeat(piece) \
        movesMask = 0ULL; \
        for(piecesMask = state->bb[piece]; piecesMask; piecesMask &= ~bmSource)

    #define setNextMoves(piece) \
        nextMove = squareSource; \
        setMovedPiece(nextMove, piece); \
        for(; movesMask; movesMask &= ~bmDestination) { \
            squareDestination = LSB(movesMask) - 1; \
            bmDestination = 1ULL << squareDestination; \
            setDestination(nextMove, squareDestination); \
            setCapturedPieceType(bmDestination); \
            moves[i++] = nextMove; \
        }

    #define setCapturedPieceType(bmDest) \
        for(j=0; j<NUM_PIECES; j++) { \
            if(state->bb[j] & (bmDest)) { \
                break; \
            } \
        } \
        setCapturedPiece(nextMove, j)

    repeat(B_ROOK - NUM_PIECES / 2 * turn) {
        squareSource = LSB(piecesMask) - 1;
        bmSource = 1ULL << squareSource;
        movesMask = hashRook(squareSource, blockers);
        setNextMoves(B_ROOK - NUM_PIECES / 2 * turn);
    }

    repeat(B_KNIGHT - NUM_PIECES / 2 * turn) {
        squareSource = LSB(piecesMask) - 1;
        bmSource = 1ULL << squareSource;
        movesMask = KNIGHT_TABLE[squareSource];
        setNextMoves(B_KNIGHT - NUM_PIECES / 2 * turn);
    }

    repeat(B_BISHOP - NUM_PIECES / 2 * turn) {
        squareSource = LSB(piecesMask) - 1;
        bmSource = 1ULL << squareSource;
        movesMask = hashBishop(squareSource, blockers);
        setNextMoves(B_BISHOP - NUM_PIECES / 2 * turn);
    }

    repeat(B_QUEEN - NUM_PIECES / 2 * turn) {
        squareSource = LSB(piecesMask) - 1;
        bmSource = 1ULL << squareSource;
        movesMask = hashRook(squareSource, blockers);
        movesMask |= hashBishop(squareSource, blockers);
        setNextMoves(B_QUEEN - NUM_PIECES / 2 * turn);
    }

    if(turn) {  // White's turn
        repeat(W_KING) {
            squareSource = LSB(piecesMask) - 1;
            bmSource = 1ULL << squareSource;
            movesMask = KING_TABLE[squareSource];
            setNextMoves(W_KING);

            setCapturedPiece(nextMove, NUM_PIECES);  // No capture
            if(squareSource == E1) {
                setIsCastling(nextMove, 1);
                if(!(W_CASTLE_K & blockers) && wCanCastleK(*state)) {
                    setDestination(nextMove, G1);
                    moves[i++] = nextMove;
                }
                if(!(W_CASTLE_Q & blockers) && wCanCastleQ(*state)) {
                    setDestination(nextMove, C1);
                    moves[i++] = nextMove;
                }
                setIsCastling(nextMove, 0);
            }
        }

        repeat(W_PAWN) {
            squareSource = LSB(piecesMask) - 1;
            bmSource = 1ULL << squareSource;
            squareDestination = squareSource + 8;
            bmDestination = 1ULL << squareDestination;
            if(!(blockers & bmDestination)) {
                movesMask |= bmDestination;
                if(bmSource & RANK_2 && !(blockers & (bmDestination << 8))) {
                    movesMask |= bmDestination << 8;
                    // TODO set next EP target square in move?
                }
            }
            if(getFile(squareSource) != 7 &&
               (blockers & (bmDestination << 1) || (hasEPTarget(*state) &&
               squareDestination + 1 == getEPTarget(*state)))) {
                movesMask |= bmDestination << 1;
            }
            if(getFile(squareSource) != 0 &&
               (blockers & (bmDestination >> 1) || (hasEPTarget(*state) &&
               squareDestination - 1 == getEPTarget(*state)))) {
                movesMask |= bmDestination >> 1;
            }

            nextMove = squareSource;
            setMovedPiece(nextMove, W_PAWN);
            for(; movesMask; movesMask &= ~bmDestination) {
                squareDestination = LSB(movesMask) - 1;
                bmDestination = 1ULL << squareDestination;
                setDestination(nextMove, squareDestination);
                setIsEP(nextMove, hasEPTarget(*state) && getEPTarget(*state) == squareDestination);
                setCapturedPieceType(bmDestination);
                if(isEP(nextMove)) {
                    setCapturedPiece(nextMove, B_PAWN);
                }
                if(bmSource & RANK_7) {
                    setIsPromotion(nextMove, 1);
                    for(j=W_KNIGHT; j<=W_QUEEN; j++) {
                        setPromotionPiece(nextMove, j);
                        moves[i++] = nextMove;
                    }
                    setIsPromotion(nextMove, 0);
                } else {
                    moves[i++] = nextMove;
                }
            }
        }
    } else {
        repeat(B_KING) {
            squareSource = LSB(piecesMask) - 1;
            bmSource = 1ULL << squareSource;
            movesMask = KING_TABLE[squareSource];
            setNextMoves(B_KING);
            if(squareSource == E8) {
                setIsCastling(nextMove, 1);
                if(!(B_CASTLE_K & blockers) && bCanCastleK(*state)) {
                    setDestination(nextMove, G8);
                    setCapturedPiece(nextMove, NUM_PIECES);
                    moves[i++] = nextMove;
                }
                if(!(B_CASTLE_Q & blockers) && bCanCastleQ(*state)) {
                    setDestination(nextMove, C8);
                    setCapturedPiece(nextMove, NUM_PIECES);
                    moves[i++] = nextMove;
                }
                setIsCastling(nextMove, 0);
            }
        }

        repeat(B_PAWN) {
            squareSource = LSB(piecesMask) - 1;
            bmSource = 1ULL << squareSource;
            squareDestination = squareSource - 8;
            bmDestination = 1ULL << squareDestination;
            if(!(blockers & bmDestination)) {
                movesMask |= bmDestination;
                if(bmSource & RANK_7 && !(blockers & (bmDestination >> 8))) {
                    movesMask |= bmDestination >> 8;
                    // TODO set next EP target square in move?
                }
            }
            if(getFile(squareSource) != 7 &&
               (blockers & bmDestination << 1 || (hasEPTarget(*state) &&
               squareDestination + 1 == getEPTarget(*state)))) {
                movesMask |= bmDestination << 1;
            }
            if(getFile(squareSource) != 0 &&
               (blockers & bmDestination >> 1 || (hasEPTarget(*state) &&
               squareDestination - 1 == getEPTarget(*state)))) {
                movesMask |= bmDestination >> 1;
            }

            nextMove = squareSource;
            setMovedPiece(nextMove, B_PAWN);
            for(; movesMask; movesMask &= ~bmDestination) {
                squareDestination = LSB(movesMask) - 1;
                bmDestination = 1ULL << squareDestination;
                setDestination(nextMove, squareDestination);
                setIsEP(nextMove, hasEPTarget(*state) && getEPTarget(*state) == squareDestination);
                setCapturedPieceType(bmDestination);
                if(isEP(nextMove)) {
                    setCapturedPiece(nextMove, W_PAWN);
                }
                if(bmSource & RANK_2) {
                    setIsPromotion(nextMove, 1);
                    for(j=B_KNIGHT; j<=B_QUEEN; j++) {
                        setPromotionPiece(nextMove, j);
                        moves[i++] = nextMove;
                    }
                    setIsPromotion(nextMove, 0);
                } else {
                    moves[i++] = nextMove;
                }
            }
        }
    }

    #undef repeat
    #undef setNextMoves
    #undef setCapturedPieceType

    *numMoves = i;
    return moves;
}

Move *generateLegalMoves(GameState *state, int *numMoves) {
    int i, n, m;
    GameState nextState;
    Move *moves = generatePseudoLegalMoves(state, &n);
    for(i=0; i<n; i++) {
        m = moves[i];
        if(getMovedPiece(m) / 6 == getCapturedPiece(m) / 6) {
            // Same color capture
            moves[i--] = moves[--n];
            continue;
        }
        if(isCastling(m)) {
            #define test(sq1, sq2, func1, func2) \
                case sq1: \
                    if(func1(*state, sq2) || func2(*state)) { \
                        moves[i--] = moves[--n]; \
                        continue; \
                    } \
                    break;
            // No castling out of or through check
            switch(getDestination(m)) {
            test(C1, D1, bAttacks, wInCheck);
            test(G1, F1, bAttacks, wInCheck);
            test(C8, D8, wAttacks, bInCheck);
            test(G8, F8, wAttacks, bInCheck);
            default:
                break;
            }
            #undef test
        }

        nextState = pushMove(state, m);

        if((getTurn(nextState) && bInCheck(nextState)) ||
           (!getTurn(nextState) && wInCheck(nextState))) {
            // player put self in check
            moves[i--] = moves[--n];
            continue;
        }
    }
    *numMoves = n;
    return moves;
}

