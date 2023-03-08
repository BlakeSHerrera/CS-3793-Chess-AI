/**
 * move.h contains useful macros and typedefs for readability.
 * It also defines various functions that may be useful in determining
 * attributes of a move, such as if a move is a capture or en passant.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "square.h"
#include "piece.h"
#include "position.h"

// TODO optimize operations?
#define getSource(move) \
    ((move) & 0b111111)
#define getDestination(move) \
    ((move) >> 6 & 0b111111)
#define getPromotionPiece(move) \
    ((move) >> 12 & 0b1111)
#define isSpecial(move) \
    ((move) >> 16 & 0b111)
#define isPromotion(move) \
    ((move) >> 16 & 1)
#define isEP(move) \
    ((move) >> 17 & 1)
#define isCastling(move) \
    ((move) >> 18 & 1)
#define getMovedPiece(move) \
    ((move) >> 19 & 0b1111)
#define getCapturedPiece(move) \
    ((move) >> 23 & 0b1111)

#define setSource(move, square) \
    move = (move & ~63) | (square)
#define setDestination(move, square) \
    move = (move & ~(63 << 6)) | ((square) << 6)
#define setPromotionPiece(move, piece) \
    move = (move & ~(0b1111 << 12)) | ((piece) << 12)
#define setIsPromotion(move, flag) \
    move = (move & ~(1 << 16)) | ((flag) << 16)
#define setIsEP(move, flag) \
    move = (move & ~(1 << 17)) | ((flag) << 17)
#define setIsCastling(move, flag) \
    move = (move & ~(1 << 18)) | ((flag) << 18)
#define setMovedPiece(move, piece) \
    move = (move & ~(0b1111 << 19)) | ((piece) << 19)
#define setCapturedPiece(move, piece) \
    move = (move & ~(0b1111 << 23)) | ((piece) << 23)

/* A move uses the following bits for storage:
 * bits 0-5: source square (0 to 63)
 * bits 6-11: dest square (0 to 63)
 * bits 12-15: promotion piece type
 * bit 16: is promotion?
 * bit 17: is EP?
 * bit 18: is castling?
 * bits 19-22: moved piece type
 * bits 23-26: captured piece type (NUM_PIECES = no capture)
 * Other bits may be used later.
 */
typedef int Move;

/**
 * Converts a move to long algebraic notation. (e.g. e7e8q)
 * @param m - The move to parse.
 * @param szBuffer - The buffer to store the result in.
 * Needs to hold a maximum of 6 chars.
 */
void toLAN(Move m, char *szBuffer);

/**
 * Pushes a move onto the current board, and returns a new board.
 * @param state - The current board state.
 * @param m - The move to push.
 * @return The next game state.
 */
GameState pushMove(GameState *state, Move m);

/**
 * Pushes a move onto the current board, and returns a new board.
 * @param state - The current board state.
 * @param source - The source square of the moved piece.
 * @param destination - The destination square of the moved piece.
 * @param movedPiece - The type of piece moved.
 * @param capturedPiece - The type of piece captured, or
 * NUM_PIECES / BLOCKERS (12) if no piece was captured.
 * @param isEP - TRUE if the move is en passant. FALSE otherwise.
 * @param isCastling - TRUE if the move is castling. FALSE otherwise.
 * @param promotion - Promotion piece type.
 * NUM_PIECES / BLOCKERS (12) if no promotion.
 * @return The next game state.
 */
GameState pushMoveVerbose(GameState *state, Square source, Square destination,
    int movedPiece, int capturedPiece, int isEP, int isCastling, int promotion);

/**
 * Pushes a Long Algebraic Notation onto the board, and returns a new board.
 * @param state - The current board state.
 * @param szLAN - Pointer to the Long Algebraic Notation string.
 * @return The next game state.
 */
GameState pushLAN(GameState *state, const char *szLAN);

#endif // MOVE_H_INCLUDED
