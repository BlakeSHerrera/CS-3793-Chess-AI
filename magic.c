/**
 * This file finds magic numbers for use.
 * Some inspiration taken from the forum post by Tord Romstad.
 * @author Blake Herrera
 * @date 2023-03-07
 * @see http://www.talkchess.com/forum3/viewtopic.php?topic_view=threads&p=175834&t=19699
 * @see https://www.chessprogramming.org/index.php?title=Looking_for_Magics#Feeding_in_Randoms
 * @see magic.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "magic.h"
#include "debug.h"
#include "bitboard.h"
#include "movegen.h"

unsigned long long int ROOK_MAGIC_NUMS[64] = {
    0x80004000976080ULL,
    0x1040400010002000ULL,
    0x4880200210000980ULL,
    0x5280080010000482ULL,
    0x200040200081020ULL,
    0x2100080100020400ULL,
    0x4280008001000200ULL,
    0x1000a4425820300ULL,
    0x29002100800040ULL,
    0x4503400040201004ULL,
    0x209002001004018ULL,
    0x1131000a10002100ULL,
    0x9000800120500ULL,
    0x10e001804820010ULL,
    0x29000402000100ULL,
    0x2002000d01c40292ULL,
    0x80084000200c40ULL,
    0x10004040002002ULL,
    0x201030020004014ULL,
    0x80012000a420020ULL,
    0x129010008001204ULL,
    0x6109010008040002ULL,
    0x950010100020004ULL,
    0x803a0000c50284ULL,
    0x80004100210080ULL,
    0x200240100140ULL,
    0x20004040100800ULL,
    0x4018090300201000ULL,
    0x4802010a00102004ULL,
    0x2001000900040002ULL,
    0x4a02104001002a8ULL,
    0x2188108200204401ULL,
    0x40400020800080ULL,
    0x880402000401004ULL,
    0x10040800202000ULL,
    0x604410a02001020ULL,
    0x200200206a001410ULL,
    0x86000400810080ULL,
    0x428200040600080bULL,
    0x2001000041000082ULL,
    0x80002000484000ULL,
    0x210002002c24000ULL,
    0x401a200100410014ULL,
    0x5021000a30009ULL,
    0x218000509010010ULL,
    0x4000400410080120ULL,
    0x20801040010ULL,
    0x29040040820011ULL,
    0x4080400024800280ULL,
    0x500200040100440ULL,
    0x2880142001004100ULL,
    0x412020400a001200ULL,
    0x18c028004080080ULL,
    0x884001020080401ULL,
    0x210810420400ULL,
    0x801048745040200ULL,
    0x4401002040120082ULL,
    0x408200210012ULL,
    0x110008200441ULL,
    0x2010002004100901ULL,
    0x801000800040211ULL,
    0x480d000400820801ULL,
    0x820104201280084ULL,
    0x1001040311802142ULL,
};

unsigned long long int BISHOP_MAGIC_NUMS[64] = {
    0x420410208004484ULL,
    0x90100a00a22000ULL,
    0x90028a004b0200ULL,
    0x14304202003100ULL,
    0x84a0a1028080001ULL,
    0x3008244420210004ULL,
    0x104008450580000ULL,
    0x480450400a01d14ULL,
    0x40c0408020408ULL,
    0x100208020c820200ULL,
    0x84805242000ULL,
    0x4091401004940ULL,
    0x2820040420400009ULL,
    0x49c008260200206ULL,
    0x2012404040a1802ULL,
    0x204301180202a4ULL,
    0xc000203c850200ULL,
    0x1010200803880180ULL,
    0x103000810410200ULL,
    0x400a440401020100ULL,
    0x24c000210220801ULL,
    0x8100a08021800ULL,
    0x200a108010402ULL,
    0x4006040022220200ULL,
    0xb50041440080200ULL,
    0x428600804010200ULL,
    0x808080105020020ULL,
    0xa81040040440080ULL,
    0x104040008c10040ULL,
    0x40401a011028ULL,
    0x6020240883100ULL,
    0x2422020640410080ULL,
    0x401084000a30400ULL,
    0x80a1212280800ULL,
    0x44010400204044ULL,
    0x12401821060200ULL,
    0x406008400120020ULL,
    0x4802180602004052ULL,
    0x202220040120800ULL,
    0x8088062810100ULL,
    0x1008010860600800ULL,
    0x1621004001028ULL,
    0x40a010043080803ULL,
    0x1003c010400200ULL,
    0x400012012000100ULL,
    0x4010200041016090ULL,
    0x3040c00800c08ULL,
    0x52020401100021ULL,
    0x1400411008204200ULL,
    0x800240104104840ULL,
    0x11001440c040840ULL,
    0x15120022a080100ULL,
    0x2081101132020148ULL,
    0x14051408020400ULL,
    0x409107040810000ULL,
    0xa0188301182000ULL,
    0x820320304024020ULL,
    0x8844022000ULL,
    0x400001900411054ULL,
    0x2045422010840400ULL,
    0x1000000110021200ULL,
    0x5404044004080082ULL,
    0x40151c0c040400ULL,
    0x2082280124040044ULL,
};

typedef unsigned long long uint64;

uint64 random_uint64() {
  uint64 u1, u2, u3, u4;
  u1 = (uint64)(rand()) & 0xFFFF; u2 = (uint64)(rand()) & 0xFFFF;
  u3 = (uint64)(rand()) & 0xFFFF; u4 = (uint64)(rand()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64 random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

uint64 findMagic(int square, int isBishop) {
    int i, m=100000000;
    uint64 n;
    for(i=0; i<m; i++) {
        n = random_uint64_fewbits();
        if(isBishop) {
            BISHOP_MAGIC_NUMS[square] = n;
            memset(BISHOP_TABLE, -1, sizeof(bitmask) * NUM_SQUARES * (1 << 9));
            if(!_testMagicBishop(square, BISHOP_RELEVANT_OCCUPANCY[square], 0))
                break;
        } else {
            if(sumBits((ROOK_RELEVANT_OCCUPANCY[square] * n) & 0xFF00000000000000ULL) < 6)
                continue;
            ROOK_MAGIC_NUMS[square] = n;
            memset(ROOK_TABLE, -1, sizeof(bitmask) * NUM_SQUARES * (1 <<12));
            if(!_testMagicRook(square, ROOK_RELEVANT_OCCUPANCY[square], 0))
                break;
        }
    }
    if(i == m) {
        printf("Could not find magic after %d tries\n", m);
    }
    return n;
}

void findMagics() {
    int i;

    printf("unsigned long long int ROOK_MAGIC_NUMS[64] = {\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printf("    0x%llxULL,\n", findMagic(i, 0));
    }
    printf("};\n\n");

    printf("unsigned long long int BISHOP_MAGIC_NUMS[64] = {\n");
    for(i=0; i<NUM_SQUARES; i++) {
        printf("    0x%llxULL,\n", findMagic(i, 1));
    }
    printf("};\n\n");
}
