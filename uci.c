/**
 * uci.c contains implementation for the functions and constants
 * defined in the associated header file.
 * @author Blake Herrera
 * @since 2023-02-15
 * @see uci.h
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
#include "search.h"
#include "evaluate.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <config.h>
#include <unistd.h>

// Theoretically longest game is a bit less than 6000 moves
static char szBuffer[6000 * 6];
static GameState state;
static int wTime, bTime, wInc, bInc, movesToGo, bestMove, maxSearchDepth,
    isReady;

int searchStrategy, pruning, evaluation, forwardPruneN, numThreads;

void uciCommunicate() {
    typedef struct pair {
        char szCommand[20];
        void (*function)();
    } pair;
    pair pairs[11] = {{"uci", &uciBoot}, {"debug", &uciDebug},
        {"isready", &uciIsReady}, {"setoption", &uciSetOption},
        {"register", &uciRegister}, {"ucinewgame", &uciNewGame},
        {"position", &uciPosition}, {"stop", &uciStop},
        {"ponderhit", &uciPonderHit}, {"quit", &uciQuit},
        {"go", &uciGo}
    };
    int i;
    isReady = 0;

    printf("CS-3743-AI engine\n");
    fprintf(f, "CS-3743-AI engine\n");
    while(1) {
        if(fgets(szBuffer, sizeof(szBuffer), stdin) == NULL) {
            fprintf(stderr, "Read from stdin failed.\n");
            exit(-1);
        }
        szBuffer[strcspn(szBuffer, "\n")] = '\0';
        fprintf(f, " in> %s\n", szBuffer); fflush(f);
        if(strtok(szBuffer, " ") == NULL) {
            fprintf(stderr, "Tokenizing stdin failed.");
            exit(-1);
        }
        for(i=0; i<11; i++) {
            if(!strcmp(szBuffer, pairs[i].szCommand)) {
                (*pairs[i].function)();
                break;
            }
        }
        fflush(stdout);
    }
}

void uciBoot() {
    printf("id name %s v%s\nid author %s\n\n"
           "option name numThreads type spin default 1 min 1 max 512\n"
           "uciok\n", ENGINE_NAME, VERSION, AUTHORS);
    fprintf(f, "out> id name %s v%s\n"
               "out> id author %s\n\n"
               "out> option name numThreads type spin default 1 min 1 max 512\n"
               "out> uciok\n", ENGINE_NAME, VERSION, AUTHORS);
    fflush(f);
    // TODO add options
    searchStrategy = 0;
    pruning = 0;
    evaluation = 0;
    forwardPruneN = 999;
    numThreads = 1;
}

void uciDebug() {
    // TODO not implemented
    if(!strcmp(strtok(NULL, " "), "on")) {

    } else if(!strcmp(szBuffer, "off")){

    }
}

void uciIsReady() {
    isReady = 1;
    printf("readyok\n");
    fprintf(f, "out> readyok\n"); fflush(f);
}

void uciSetOption() {
    // TODO not implemented
    #define next() strtok(NULL, " ")
    next();  // "name"
    if(!strcmp(next(), "searchStrategy")) {
        next();  // "value"
        searchStrategy = atoi(next());
    } else if(!strcmp(szBuffer, "pruning")) {
        next();  // "value"
        pruning = atoi(next());
    } else if(!strcmp(szBuffer, "evaluation")) {
        next();  // "value"
        evaluation = atoi(next());
    } else if(!strcmp(szBuffer, "numThreads")) {
        next();  // "value"
        numThreads = atoi(next());
    } else if(!strcmp(szBuffer, "forwardPruneN")) {
        next();  // "value"
        forwardPruneN = atoi(next());
    } // else unknown option - ignore
    #undef next
}

void uciRegister() {
    printf("register later\n");
    fprintf(f, "out> register later\n"); fflush(f);
    // TODO look up registration?
}

void uciNewGame() {
    // TODO
}

void uciPosition() {
    #define next() strtok(NULL, " ")
    char *a, *b, *c, *d, *e, *f;
    a = next();
    if(!strcmp(a, "startpos")) {
        state = positionFromFen(START_FEN);
    } else if(!strcmp(a, "fen")) {
        a = next();
        b = next();
        c = next();
        d = next();
        e = next();
        f = next();
        state = positionFromFenParts(a, b, c, d, e, f);
        // For some reason the calls to strtok are done backwards
        // if put directly in the function call
    }
    a = next();
    for(a=next(); a!=NULL; a=next()) {
        state = pushLAN(&state, a);
    }
    #undef next
}

void uciStop() {
    // TODO threading
    toLAN(bestMove, szBuffer);
    printf("bestmove %s\n", szBuffer);
    fprintf(f, "out> bestmove %s\n", szBuffer); fflush(f);
}

void uciPonderHit() {
    // TODO not implemented
}

void uciQuit() {
    // TODO clean up memory?
    exit(0);
}

void uciGo() {
    int addSearchMoves = 0;
    if(!isReady) {
        return;
    }
    // TODO
    #define is(x) !strcmp(szBuffer, x)
    #define next() strtok(NULL, " ")
    #define nextInt() atoi(next())
    while(next() != NULL) {
        if(is("wtime")) {
            wTime = nextInt();
        } else if(is("btime")) {
            bTime = nextInt();
        } else if(is("winc")) {
            wInc = nextInt();
        } else if(is("binc")) {
            bInc = nextInt();
        } else if(is("movestogo")) {
            movesToGo = nextInt();
        } else if(is("depth")) {
            maxSearchDepth = nextInt();
        } else if(is("nodes")) {
            nextInt();  // TODO
        } else if(is("mate")) {
            nextInt();  // TODO
        } else if(is("movetime")) {
            nextInt();  // TODO
        } else if(is("infinite")) {
            // TODO
        } else if(is("searchmoves")) {
            addSearchMoves = 1;
            continue;
        } else if(addSearchMoves) {
            // TODO not implemented
            continue;
        }
        addSearchMoves = 0;
    }
    #undef is
    #undef next
    #undef nextInt

    // Do search
    //position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    if(searchStrategy == RANDOM_MOVES) {
        bestMove = getRandomMove(state);
        toLAN(bestMove, szBuffer);
        printf("bestmove %s\n", szBuffer);
        fprintf(f, "out> bestmove %s\n", szBuffer); fflush(f);
    } else if(searchStrategy == MINIMAX) {
        // TODO
    }
}
