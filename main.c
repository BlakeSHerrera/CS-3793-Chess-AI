/**
 * main.c is the driver class which loads the engine.
 * It calls the initializers for other files before
 * communicating with the UCI program.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitboard.h"
#include "debug.h"
#include "move.h"
#include "movegen.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include "uci.h"
#include "magic.h"
#include "config.h"
#include "search.h"
#include "evaluate.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(clock());
    bitboardInit();
    movegenInit();
    //findMagics();
    uciCommunicate();
    return 0;
}
