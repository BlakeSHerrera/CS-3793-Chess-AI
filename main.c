/**
 * main.c is the driver class which loads the engine.
 * @author Blake Herrera
 * @date 2023-02-15
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

int main(int argc, char **argv) {
    bitboardInit();
    movegenInit();
    uciCommunicate();

    return 0;
}
