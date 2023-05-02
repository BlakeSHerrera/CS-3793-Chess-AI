/**
 * position.h has useful structs and functions to store the state of a
 * position efficiently, and convert to and from FENs.
 * @author Blake Herrera
 * @date 2023-02-15
 * @see https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 */

#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "bitboard.h"
#include "piece.h"
#include "square.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define getTurn(state) ((state).fenInfo & 1)
#define getCastlingRights(state) ((state).fenInfo >> 1 & 0b1111)
#define wCanCastleK(state) ((state).fenInfo & 2)
#define wCanCastleQ(state) ((state).fenInfo & 4)
#define bCanCastleK(state) ((state).fenInfo & 8)
#define bCanCastleQ(state) ((state).fenInfo & 16)
#define hasEPTarget(state) ((state).fenInfo & 32)
#define getEPTarget(state) ((state).fenInfo >> 6 & 63)
#define getHalfMoveCounter(state) ((state).fenInfo >> 12 & 127)
#define getFullMoveCounter(state) ((state).fenInfo >> 19)

#define setTurn(state, turn) \
    (state).fenInfo = ((state).fenInfo & ~1) | (turn)
#define setCastlingRights(state, rights) \
    (state).fenInfo = ((state).fenInfo & ~0b11110) | (rights) << 1
#define setWCanCastleK(state, canCastle) \
    (state).fenInfo = ((state).fenInfo & ~0b10) | (canCastle) << 1
#define setWCanCastleQ(state, canCastle) \
    (state).fenInfo = ((state).fenInfo & ~0b100) | (canCastle) << 2
#define setBCanCastleK(state, canCastle) \
    (state).fenInfo = ((state).fenInfo & ~0b1000) | (canCastle) << 3
#define setBCanCastleQ(state, canCastle) \
    (state).fenInfo = ((state).fenInfo & ~0b10000) | (canCastle) << 4
#define setHasEPTarget(state, hasTarget) \
    (state).fenInfo = ((state).fenInfo & ~0b100000) | (hasTarget) << 5
#define setEPTarget(state, square) \
    setHasEPTarget(state, 1); \
    (state).fenInfo = ((state).fenInfo & ~(63 << 6)) | (square) << 6
#define setHalfMoveCounter(state, counter) \
    (state).fenInfo = ((state).fenInfo & ~(127 << 12)) | (counter) << 12
#define setFullMoveCounter(state, counter) \
    (state).fenInfo = ((state).fenInfo & ~(8191 << 19)) | (counter) << 19


/* A game state needs 12*64 bits for the bitboard, plus another
 * 64 bits to reference a previous state, plus more for info:
 * x for the rest of the game data.
 * Bit 0: Black to move (0) or White to move (1)
 * Bits 1-4: Castling rights
 * Bit 5: has ep target square
 * Bits 6-11: ep target square (0-63)
 * Bits 12-18: Half move counter (only up to 50 is needed)
 * Bits 19-31: Full move counter (can hold theoretical max no. of moves)
 * The material value is updated incrementally to save on computations.
 */
typedef struct GameState {
    bitmask bb[NUM_PIECES + 1]; // Last index for blockers
    struct GameState *prev;
    int fenInfo;
    double material;
} GameState;

/**
 * Creates a game from a FEN string.
 * @param fen - The FEN string to convert.
 * @return A GameState based on the FEN.
 */
GameState positionFromFen(const char *szFen);

/** Creates a game from FEN parts.
 * @param szBoard - The string representing the board.
 * @param szTurn - The string representing the side to move.
 * @param szCastlingRights - The string representing castling rights.
 * @param szEPTarget - The string representing the en passant target.
 * @param szHalfMoveCounter - The string representing the half move counter.
 * @param szFullMoveCounter - The string representing the full move counter.
 * @return A GameState based on the FEN parts.
 */
GameState positionFromFenParts(const char *szBoard, const char *szTurn,
    const char *szCastlingRights, const char *szEPTarget,
    const char *szHalfMoveCounter, const char *szFullMoveCounter);

/**
 * Writes the FEN string into a given buffer.
 * @param state - The state to write.
 * @param szBuffer - Output to write the FEN.
 */
void positionToFen(GameState state, char *szBuffer);

/**
 * Checks whether the position meets the 50 move rule.
 * @param state - The position to check.
 * @return - TRUE if the position meets the 50 move rule. FALSE otherwise.
 */
int is50MoveRule(GameState state);

/**
 * Checks whether the position is a draw by 3-fold repetition.
 * @param state - The position to check.
 * @return - TRUE if the position is 3-fold repetition. FALSE otherwise.
 */
int is3FoldRepetition(GameState state);

/**
 * Checks whether the position is illegal. A position is illegal if the
 * side not to move is in check or pawns are on the first or last rank.
 * @param state - The position to check.
 * @return TRUE if the position is illegal. FALSE otherwise.
 */
int isIllegalPosition(GameState state);

/**
 * Checks whether or not black attacks a square.
 * @param state - The position to check.
 * @param sq - The square to check.
 * @return TRUE if black attacks the square. FALSE otherwise.
 */
int bAttacks(GameState state, Square sq);

/**
 * Checks whether or not black is in check.
 * @param state - The position to check.
 * @param sq - The square to check.
 * @return TRUE if white attacks the square. FALSE otherwise.
 */
int wAttacks(GameState state, Square sq);

/**
 * Checks whether or not white is in check.
 * @param state - The position to check.
 * @return TRUE if white is in check. FALSE otherwise.
 */
int wInCheck(GameState state);

/**
 * Checks whether or not black is in check.
 * @param state - The position to check.
 * @return TRUE if black is in check. FALSE otherwise.
 */
int bInCheck(GameState state);

/**
 * Return a bitmask of all attacked squares for one side.
 * This does not include pins.
 * @param state - The current game state to evaluate.
 * @param white - TRUE to get white's attacks.
 * FALSE to get black's attacks.
 */
bitmask getAllAttacks(GameState state, int white);

/**
 * Gets the piece sitting on the given square.
 * @param state - The state to check.
 * @param square - The square to check.
 * @return The piece sitting on the square,
 * or 12 (NUM_PIECES) if no such square exists.
 * @see getPieceFromBitmask
 */
int getPieceFromSquare(GameState state, Square square);

/**
 * Gets the piece sitting on the given square.
 * @param state - The state to check.
 * @param square - The square to check, as a bitmask.
 * @return The piece sitting on the square,
 * or 12 (NUM_PIECES) if no such square exists.
 */
int getPieceFromBitmask(GameState state, bitmask square);

#endif // POSITION_H_INCLUDED
