/**
 * magic.h contains the constants found from executing the findMagics
 * function in magic.c.
 * @author Blake Herrera
 * @date 2023-02-17
 * @see https://www.chessprogramming.org/index.php?title=Looking_for_Magics#Feeding_in_Randoms
 * @see https://rhysre.net/fast-chess-move-generation-with-magic-bitboards.html
 */

#ifndef MAGIC_H_INCLUDED
#define MAGIC_H_INCLUDED

/**
 * This function finds magic numbers.
 */
void findMagics();

extern unsigned long long int ROOK_MAGIC_NUMS[64];
extern unsigned long long int BISHOP_MAGIC_NUMS[64];

#endif // MAGIC_H_INCLUDED
