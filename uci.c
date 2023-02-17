/**
 * uci.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @since 2023-02-15
 * @see uci.h
 */

#include "bitboard.h"
#include "debug.h"
#include "move.h"
#include "movegen.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include "uci.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char szBuffer[256];

void uciCommunicate() {
    typedef struct pair {
        char szCommand[20];
        void (*function)();
    } pair;
    pair pairs[11] = {{"uci", &uciBoot}, {"debug", &uciDebug},
        {"isready", &uciIsReady}, {"setoption", &uciSetOption},
        {"register", &uciRegister}, {"ucinewgame", &uciNewGame},
        {"position", &uciPosition}, {"stop", &uciStop},
        {"ponderhit", &uciPonderHit}, {"quit", &uciQuit}, {"go", &uciGo}
    };
    int i;

    while(1) {
        if(gets(szBuffer) == NULL) {
            fprintf(stdin, "Read from stdin failed.\n");
            exit(-1);
        }
        if(strtok(szBuffer, " ") == NULL) {
            fprintf(stdin, "Tokenizing stdin failed.");
            exit(-1);
        }
        for(i=0; i<11; i++) {
            if(!strcmp(szBuffer, pairs[i].szCommand)) {
                (*pairs[i].function)();
                break;
            }
        }
    }
}

void uciBoot() {
    printf("id name %s v%s\n", ENGINE_NAME, VERSION);
    printf("id author %s\n", AUTHORS);
    // TODO add options
}

void uciDebug() {
    // TODO not implemented
    if(!strcmp(strtok(NULL, " "), "on")) {

    } else if(!strcmp(szBuffer, "off")){

    }
}

void uciIsReady() {
    printf("readyok\n");
}

void uciSetOption() {
    // TODO not implemented
}

void uciRegister() {
    printf("register later\n");
    // TODO look up registration?
}

void uciNewGame() {
    // TODO
}

void uciPosition() {
    // TODO
}

void uciStop() {
    // TODO
}

void uciPonderHit() {
    // TODO not implemented
}

void uciQuit() {
    // TODO clean up memory
    exit(0);
}

void uciGo() {
    // TODO
}
