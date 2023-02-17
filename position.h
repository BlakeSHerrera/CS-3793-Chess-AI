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

#define getTurn(state) (state.fenInfo & 1)
#define getCastlingRights(state) (state.fenInfo >> 1 & 0b1111)
#define wCanCastleK(state) (state.fenInfo & 2)
#define wCanCastleQ(state) (state.fenInfo & 4)
#define bCanCastleK(state) (state.fenInfo & 8)
#define bCanCastleQ(state) (state.fenInfo & 16)
#define hasEPTarget(state) (state.fenInfo & 32)
#define getEPTarget(state) (state.fenInfo >> 8 & 63)
#define getHalfMoveCounter(state) (state.fenInfo >> 12 & 127)
#define getFullMoveCounter(state) (state.fenInfo >> 19)


/* A game state needs 12*64 bits for the bitboard, plus another
 * 64 bits to reference a previous state, plus more for info:
 * x for the rest of the game data.
 * Bit 0: Black to move (0) or White to move (1)
 * Bits 1-4: Castling rights
 * Bit 5: has ep target square
 * Bits 6-11: ep target square (0-63)
 * Bits 12-18: Halfmove counter (only up to 50 is needed)
 * Bits 19-31: Full move counter (can hold theoretical max no. of moves)
 */
typedef struct GameState {
    bitmask bb[12];
    struct GameState *prev;
    int fenInfo;
} GameState;

/**
 * Creates a game from a FEN string.
 * @param fen - The FEN string to convert.
 * @return - A GameState based on the FEN.
 */
GameState positionFromFen(char *szFen);

/**
 * Writes the FEN string into a given buffer.
 * @param state - The state to write.
 * @param szBuffer - Output to write the FEN.
 */
void posisitionToFen(GameState state, char *szBuffer);

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
 * Checks whether the position is illegal. A position is illegal if both
 * kings are in check or pawns are on the first or last rank.
 * @param state - The position to check.
 * @return TRUE if the position is illegal. FALSE otherwise.
 */
int isIllegalPosition(GameState state);

#endif // POSITION_H_INCLUDED
