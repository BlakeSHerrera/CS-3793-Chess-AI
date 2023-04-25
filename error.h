/**
 * error.h contains functions for conveniently checking
 * and handling errors in the code.
 * @author Blake Herrera
 * @date 2023-04-24
 */

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

/**
 * errTrap provides a convenient way to error trap
 * other functions along with a failure message.
 * @param status - Status of the returning function.
 * @param msg - Message to display on a failure.
 */
int errTrap(int status, const char *msg);

#endif // ERROR_H_INCLUDED
