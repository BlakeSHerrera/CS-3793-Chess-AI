/**
 * move.h contains useful macros and typedefs for readability.
 * It also defines various functions that may be useful in determing
 * attributes of a move, such as if a move is a capture or en passant.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "square.h"
#include "piece.h"

#define isPromotion(move) (move.promotion != NO_PIECE)
#define onBoard(move) (move.source < 64 && move.dest < 64)

/* A move needs 16 bits to be stored:
 * bits 0-5: dest square (0 to 63)
 * bits 6-11: source square (0 to 63)
 * bits 12-13: promotion piece type (from knight - 2 to queen - 2)
 * bits 14-15: special move flag: promotion (1), ep (2), castle (3)
 */
typedef short Move;

#endif // MOVE_H_INCLUDED
