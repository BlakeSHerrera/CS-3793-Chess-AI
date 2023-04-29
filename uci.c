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
#include <unistd.h>
#include <pthread.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <signal.h>

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
#include "config.h"
#include "error.h"

#ifdef _WIN32
//  For Windows (32- and 64-bit)
#   include <windows.h>
#   define SLEEP(msecs) Sleep(msecs)
#elif __unix
//  For linux, OSX, and other unixes
#   define _POSIX_C_SOURCE 199309L // or greater
#   include <time.h>
#   define SLEEP(msecs) do {            \
        struct timespec ts;             \
        ts.tv_sec = msecs/1000;         \
        ts.tv_nsec = msecs%1000*1000;   \
        nanosleep(&ts, NULL);           \
        } while (0)
#else
#   error "Unknown system"
#endif

// From config.h
int searchStrategy, pruning, evaluation, forwardPruneN, numThreads,
    maxSearchDepth;
double mobilityFactor, timeUseFraction, quiescenceCutoff;
double (*evaluationFunction)(GameState);

// UCI specific
static char szBuffer[6000 * 6];
static GameState state;
static int wTime, bTime, wInc, bInc, movesToGo, isReady;

// Thread and shared data management
static Move principalVariation;
static pthread_t timeKeeper, searchMaster;
static pthread_mutex_t manageThreads;
static pthread_cond_t readyToSubmit;

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

    printf("CS-3743-AI engine\n");
    errTrap(pthread_mutex_init(&manageThreads, NULL),
            "Error on pthread_mutex_init in uciBoot\n");
    errTrap(pthread_cond_init(&readyToSubmit, NULL),
            "Error on pthread_cond_init in uciBoot\n");
    timeKeeper = 0;
    isReady = 0;

    while(1) {
        errTrap(fgets(szBuffer, sizeof(szBuffer), stdin) == NULL,
                "Error on fgets in uciCommunicate\n");
        szBuffer[strcspn(szBuffer, "\n")] = '\0';
        errTrap(strtok(szBuffer, " ") == NULL,
                "Error on strtok in uciCommunicate\n");
        for(i=0; i<11; i++) {
            if(!strcmp(szBuffer, pairs[i].szCommand)) {
                (*pairs[i].function)();
                break;
            }
        }
        errTrap(fflush(stdout),
                "Error on fflush in uciCommunicate\n");
    }
}

void uciBoot() {
    printf("id name %s v%s\nid author %s\n\n"
           "option name searchStrategy type spin default 1 min 0 max 2\n"
           "option name pruning type spin default 1 min 0 max 7\n"
           "option name evaluation type spin default 2 min 0 max 2\n"
           "option name maxSearchDepth type spin default 99 min 1 max 99\n"
           "option name forwardPruneN type spin default 999 min 1 max 999\n"
           "option name numThreads type spin default 1 min 1 max 512\n"
           "option name mobilityFactor type double default 0.1 min 0 max 1\n"
           "option name timeUseFraction type double default 0.05 min 0.001 max 1.0\n"
           "option name quiescenceCutoff type double default 1.0 min 0.001 max 200.0\n"
           "uciok\n", ENGINE_NAME, VERSION, AUTHORS);
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
}

void uciSetOption() {
    char *token;
    #define next() token = strtok(NULL, " ")
    #define is(s) !strcmp(token, s)
    next();  // "name"
    next();  // option name
    if(is("maxSearchDepth")) {
        next();  // "value"
        maxSearchDepth = atoi(next());
    } else if(is("searchStrategy")) {
        next();  // "value"
        searchStrategy = atoi(next());
    } else if(is("pruning")) {
        next();  // "value"
        pruning = atoi(next());
    } else if(is("evaluation")) {
        next();  // "value"
        evaluation = atoi(next());
        switch(evaluation) {
        case PIECE_VALUE_EVAL:
            evaluationFunction = simplePieceValueCount;
            break;
        case VALUE_AND_INFLUENCE:
            evaluationFunction = valueAndInfluence;
            break;
        case VALUE_AND_MOBILITY:
            evaluationFunction = valueAndMobility;
            break;
        default:
            break;
        }
    } else if(is("numThreads")) {
        next();  // "value"
        numThreads = atoi(next());
    } else if(is("forwardPruneN")) {
        next();  // "value"
        forwardPruneN = atoi(next());
    } else if(is("mobilityFactor")) {
        next();  // "value"
        mobilityFactor = atof(next());
    } else if(is("timeUseFraction")) {
        next();  // "value"
        timeUseFraction = atof(next());
    } else if(is("quiescenceCutoff")) {
        next();  // "value"
        quiescenceCutoff = atof(next());
    } else {
        fprintf(stderr, "Unknown option: %s\n", token);
    }
    #undef next
    #undef is
}

void uciRegister() {
    printf("register later\n");
    // TODO look up what is registration?
}

void uciNewGame() {
    errTrap(pthread_mutex_init(&manageThreads, NULL),
            "Error on pthread_mutex_init in uciBoot\n");
    errTrap(pthread_cond_init(&readyToSubmit, NULL),
            "Error on pthread_cond_init in uciBoot\n");
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
    next();  // "moves"
    for(a=next(); a!=NULL; a=next()) {
        state = pushLAN(&state, a);
    }
    #undef next
}

void uciStop() {
    errTrap(pthread_cond_broadcast(&readyToSubmit),
            "Error on pthread_cond_broascast in uciStop\n");
}

void uciPonderHit() {
    // TODO not implemented
}

void uciQuit() {
    // TODO clean up memory?
    exit(0);
}

void uciGo() {
    char *token;
    int addSearchMoves = 0;
    if(!isReady) {
        return;
    }
    #define is(x) !strcmp(token, x)
    #define next() strtok(NULL, " ")
    #define nextInt() atoi(next())
    for(token = next(); token != NULL; token = next()) {
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

    if(timeKeeper) {
        errTrap(pthread_join(timeKeeper, NULL),
                "Error on pthread_join timeKeeper in uciGo\n");
        timeKeeper = 0;
    }
    errTrap(pthread_create(&timeKeeper, NULL, timeKeepStart, NULL),
            "Error on pthread_create timeKeeper\n");
}

void *timeKeepStart(void *params) {
    char szMoveString[6];
    double msSleepTime;
    struct timespec finalTime;
    struct timespec now;
    int r;

    if(getTurn(state)) {
        msSleepTime = wTime * timeUseFraction + wInc * (1 - timeUseFraction);
    } else {
        msSleepTime = bTime * timeUseFraction + bInc * (1 - timeUseFraction);
    }
    if(msSleepTime < 0) {
        msSleepTime = 0;
    }

    // Get time and calculate sleep time
    errTrap(clock_gettime(CLOCK_REALTIME, &now),
            "Error on clock_gettime in timeKeepStart\n");
    finalTime.tv_sec = (time_t) (now.tv_sec + msSleepTime / 1000);
    finalTime.tv_nsec = (long) (now.tv_nsec + fmod(msSleepTime, 1000) * 1000) % 1000000000;

    // Sleep / wait for signal to submit move
    errTrap(pthread_mutex_lock(&manageThreads),
            "Error on pthread_mutex_lock in timeKeepStart\n");
    errTrap(pthread_create(&searchMaster, NULL, threadStartSearch, NULL),
            "Error on pthread_create threadStartSearch in timeKeepStart\n");
    r = pthread_cond_timedwait(&readyToSubmit, &manageThreads, &finalTime);
    if(r != 0 && r != ETIMEDOUT) {
        errTrap(r, "Error on pthread_cond_timedwait in timeKeepStart\n");
    }

    // kill search, submit move on wake
    errTrap(pthread_cancel(searchMaster),
            "Error on pthread_cancel in timeKeepStart\n");
    errTrap(pthread_join(searchMaster, NULL),
            "Error on pthread_join in timeKeepStart\n");
    toLAN(principalVariation, szMoveString);
    printf("bestmove %s\n", szMoveString);
    errTrap(fflush(stdout),
            "Error on fflush in timeKeepStart\n");
    errTrap(pthread_mutex_unlock(&manageThreads),
            "Error on pthread_mutex_unlock in timeKeepStart\n");
    return NULL;
}

void *threadStartSearch(void *params) {
    int i;
    moveScoreLeaves msp;
    char temp[6];
    clock_t start;
    double seconds;

    switch(searchStrategy) {
    case RANDOM_MOVES:
        errTrap(pthread_mutex_lock(&manageThreads),
                "Error on pthread_mutex_lock in threadStartSearch (RANDOM_MOVES)\n");
        principalVariation = getRandomMove(state);
        errTrap(pthread_mutex_unlock(&manageThreads),
                "Error on pthread_mutex_unlock in threadStartSearch (RANDOM_MOVES)\n");
        break;
    case MINIMAX:
    case MINIMAX_QUIESCENCE:
        for(i=0; i<=maxSearchDepth; i++) {
            start = clock();
            msp = miniMax(state, i, -INFINITY, INFINITY, evaluationFunction(state));
            seconds = (double)(clock() - start + 1) / CLOCKS_PER_SEC;
            errTrap(pthread_mutex_lock(&manageThreads),
                    "Error on pthread_mutex_lock in threadStartSearch (MINIMAX)\n");
            if(!i) {
                msp.move = getRandomMove(state);
            }
            principalVariation = msp.move;
            toLAN(principalVariation, temp);
            printf("info depth %d nodes %lu time %0.3f nps %d score cp %d pv %s\n",
                   i, msp.leaves, seconds, (int)(msp.leaves / seconds), (int)(msp.score * 100), temp);
            errTrap(fflush(stdout),
                    "Error in fflush stdout in threadStartSearch\n");
            errTrap(pthread_mutex_unlock(&manageThreads),
                    "Error on pthread_mutex_unlock in threadStartSearch (MINIMAX)\n");
            if(msp.score == DBL_MAX || msp.score == -DBL_MAX) {
                break;
            }
        }
        break;
    default:
        errTrap(searchStrategy, "Unknown search strategy");
        break;
    }

    errTrap(pthread_cond_broadcast(&readyToSubmit),
            "Error on pthread_cond_broadcast in threadStartSearch\n");
    return NULL;
}
