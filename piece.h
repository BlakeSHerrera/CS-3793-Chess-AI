/**
 * piece.h enumerates the types of pieces and has functionality
 * for converting to and from strings.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#define NUM_PIECES 12

#define NO_PIECE (-1)

#define W_PAWN 0
#define W_KNIGHT 1
#define W_BISHOP 2
#define W_ROOK 3
#define W_QUEEN 4
#define W_KING 5
#define B_PAWN 6
#define B_KNIGHT 7
#define B_BISHOP 8
#define B_ROOK 9
#define B_QUEEN 10
#define B_KING 11

#define BLOCKERS 12

#define PIECE_STR "PNBRQKpnbrqk"

typedef char Piece;

extern Piece CHAR_TO_PIECE[128];

#endif // PIECE_H_INCLUDED
