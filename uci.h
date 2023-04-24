/**
 * uci.h contains all relevant functions for communicating with the
 * Universal Chess Interface (UCI) protocol, used by modern chess
 * applications.
 *
 * Because the UCI protocol requires the engine to simultaneously
 * be able to search for the move and process input (such as
 * immediately stopping and returning the best move), uci.c also
 * handles the management of threads, shared data, and time strategies.
 *
 * @author Blake Herrera
 * @date 2023-02-15
 * @see https://www.wbec-ridderkerk.nl/html/UCIProtocol.html
 */

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#define ENGINE_NAME "untitled"
#define AUTHORS "Blake Herrera"
#define VERSION "0.1"

/**
 * uciCommunicate is the main method for communication with the UCI.
 * I/O is done via stdio. Several commands may be issued by the UCI,
 * and the engine may similarly pass commands to the UCI.
 */
void uciCommunicate();

/**
 * uciBoot boots the the engine to use the UCI protocol. The engine responds
 * with its name and authors, as well as any options that may be configured.
 */
void uciBoot();

/**
 * uciDebug switches the debug mode on or off depending on the next token
 * in the input buffer (on | off). The engine will pass extra information
 * to the UCI.
 * TODO not implemented.
 */
void uciDebug();

/**
 * uciIsReady simply responds to the UCI "readyok" to indicate that the
 * process is still alive.
 */
void uciIsReady();

/**
 * uciSetOption parses options from the input buffer.
 * TODO not implemented (no options yet)
 */
void uciSetOption();

/**
 * TODO not implemented (idk what the register command does)
 */
void uciRegister();

/**
 * uciNewGame resets the game.
 * @see uciPosition
 */
void uciNewGame();

/**
 * uciPosition parses a fen from the remaining input buffer and sets the
 * game state to it. "startpos" may be used instead of the starting fen.
 */
void uciPosition();

/**
 * uciStop indicates that the engine should stop calculating.
 */
void uciStop();

/**
 * uciPonderHit indicates to the engine that the opponent played the
 * move it was pondering on.
 * @see uciGo
 */
void uciPonderHit();

/**
 * uciQuit exits the program ASAP.
 */
void uciQuit();

/**
 * uciGo tells the engine to begin calculating the current position. Several
 * other options may be passed in the input line.
 */
void uciGo();

/**
 * timeKeepStart is an entry point for the pthread which sleeps
 * until the engine has used enough time. It then submits the current
 * best move and kills the search thread and itself.
 * This function has no input and no output.
 */
void *timeKeepStart(void *params);

/**
 * threadStartSearch is an entry point for the pthread which
 * begins the iterative deepening search. This thread should be woken
 * up by the time keeping thread (unless the end of the game is found,
 * in which case it submits the best move and kills both threads).
 * This function has no input and no output.
 */
void *threadStartSearch(void *params);

/**
 * submitMove coordinates access for submitting a move in the event
 * that multiple threads try to submit a move.
 */
void submitMove();

#endif // UCI_H_INCLUDED
