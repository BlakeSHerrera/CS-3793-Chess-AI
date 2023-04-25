/**
 * error.c contains implementations for functions
 * defined in the associated header file.
 * @author Blake Herrera
 * @date 2023-04-24
 */

#include "error.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int errTrap(int status, const char *msg) {
    if(status) {
        fprintf(stderr, msg);
        fprintf(stderr, "Error number: %d\n", status);
        exit(status);
    }
    return status;
}
