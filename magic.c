/**
 * This file finds magic numbers for use. All credit goes to Tord Romstad,
 * with very tiny modifications to the original code.
 * @author Tord Romstad
 * @date 2002-02-20
 * @see http://www.talkchess.com/forum3/viewtopic.php?topic_view=threads&p=175834&t=19699
 * @see https://www.chessprogramming.org/index.php?title=Looking_for_Magics#Feeding_in_Randoms
 */

#include <stdio.h>
#include <stdlib.h>

#include "magic.h"

unsigned long long int ROOK_MAGIC_NUMS[64] = {
    0xc80001828100040ULL,
    0x26004408400010ULL,
    0x1060040000202048ULL,
    0x110141100800888ULL,
    0x84420501a000802ULL,
    0x1803002905002224ULL,
    0x104380106000182ULL,
    0x208201001041ULL,
    0x4080118100002020ULL,
    0x1c40120100004020ULL,
    0x1009040000802020ULL,
    0x884002800841010ULL,
    0x2220068801201011ULL,
    0x1911442000004022ULL,
    0x4020802010011ULL,
    0x220004400102001ULL,
    0x4009608000842280ULL,
    0x4060104318400210ULL,
    0x811002400040020ULL,
    0x4801520000c41010ULL,
    0x4280020022204ULL,
    0x413000408404041ULL,
    0xc028800000101ULL,
    0x100400e904000201ULL,
    0x3800242000100a5ULL,
    0xc0008008210321ULL,
    0x2003002400001261ULL,
    0x410028894a041001ULL,
    0x800041040002901ULL,
    0x1002080204080eULL,
    0x22218040040401ULL,
    0x24084103280482ULL,
    0x20002020801040ULL,
    0x1020200020400812ULL,
    0x4082000000c0061ULL,
    0x4042024001200804ULL,
    0x5040040000024188ULL,
    0x100040008101a13ULL,
    0x408108018080802ULL,
    0x80408064000041ULL,
    0x850400821800120ULL,
    0x8100240008480bULL,
    0x6008080001100241ULL,
    0x1eb0100200110248ULL,
    0x30202216010004ULL,
    0x802021000c6142ULL,
    0x32200444020410aULL,
    0xa0202004580041ULL,
    0x80410000406180ULL,
    0x8220200002040a3ULL,
    0x93000200a002804ULL,
    0x40080080010010b2ULL,
    0x40080101012a1304ULL,
    0x4041000001012ULL,
    0x10001000e840482ULL,
    0xc04020200004885ULL,
    0x1080006110001041ULL,
    0x2081211102084250ULL,
    0x1020001500014619ULL,
    0x3810440a10000c23ULL,
    0x410484200860422ULL,
    0x180b0201090b2004ULL,
    0x1100040124410282ULL,
    0x1820a101000443ULL,
};

unsigned long long int BISHOP_MAGIC_NUMS[64] = {
    0x1841160051a00401ULL,
    0x202000012224a02ULL,
    0x284003005412542ULL,
    0x2800a220c40502ULL,
    0x68005050040308ULL,
    0x440802c810020230ULL,
    0x240034002014417ULL,
    0x45202411310208aULL,
    0x1a08410602016172ULL,
    0x830464044100e08ULL,
    0x411a31a010040808ULL,
    0x200124000484405ULL,
    0x60020c80000120aULL,
    0x6a001002583420aULL,
    0x10082020084c0051ULL,
    0x1325202008030692ULL,
    0x2002026a00045020ULL,
    0x41051c000052122cULL,
    0x10200b000100803ULL,
    0x720204000610032ULL,
    0x6818603041604002ULL,
    0x1041040010020001ULL,
    0x301081420025204ULL,
    0x42024420826001ULL,
    0x404304000131801ULL,
    0x1041090c00842100ULL,
    0x144110020080240ULL,
    0x1211004002202008ULL,
    0x105080000500401ULL,
    0x2080600c62080131ULL,
    0x844100114022082ULL,
    0x841080020804b00ULL,
    0x8240800080210ULL,
    0x4002144012084210ULL,
    0x2052000060904ULL,
    0x88400420500c14ULL,
    0x434000204404ULL,
    0x1840240800212081ULL,
    0x4208c019244184ULL,
    0x54da10030841ULL,
    0x1200090011080a02ULL,
    0x800140c000414c8ULL,
    0x4400382000401058ULL,
    0x100020140000105ULL,
    0x610040040044421ULL,
    0x20001202848110aULL,
    0x222011010300101ULL,
    0x106608008012304ULL,
    0x220429002014a02ULL,
    0x110800a005104208ULL,
    0x806201048200111ULL,
    0x2082008a03000802ULL,
    0x2041008034011420ULL,
    0x401006005004434ULL,
    0x90a429004080204ULL,
    0x2041040950041802ULL,
    0x1002200028441145ULL,
    0x109004012802018aULL,
    0x65100405500801ULL,
    0x284240458180a00ULL,
    0x2002441020024020ULL,
    0x4a10110101144112ULL,
    0x1010a2c00005030ULL,
    0x40c012121020424ULL,
};

#define USE_32_BIT_MULTIPLICATIONS

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

int count_1s(uint64 b) {
  int r;
  for(r = 0; b; r++, b &= b - 1);
  return r;
}

const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

int pop_1st_bit(uint64 *bb) {
  uint64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

uint64 index_to_uint64(int index, int bits, uint64 m) {
  int i, j;
  uint64 result = 0ULL;
  for(i = 0; i < bits; i++) {
    j = pop_1st_bit(&m);
    if(index & (1 << i)) result |= (1ULL << j);
  }
  return result;
}

uint64 rmask(int sq) {
  uint64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
  for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
  for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
  for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
  return result;
}

uint64 bmask(int sq) {
  uint64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
  for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
  for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
  for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
  return result;
}

uint64 ratt(int sq, uint64 block) {
  uint64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 7; r++) {
    result |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  }
  for(r = rk-1; r >= 0; r--) {
    result |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  }
  for(f = fl+1; f <= 7; f++) {
    result |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  }
  for(f = fl-1; f >= 0; f--) {
    result |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  }
  return result;
}

uint64 batt(int sq, uint64 block) {
  uint64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  return result;
}


int transform(uint64 b, uint64 magic, int bits) {
#if defined(USE_32_BIT_MULTIPLICATIONS)
  return
    (unsigned)((int)b*(int)magic ^ (int)(b>>32)*(int)(magic>>32)) >> (32-bits);
#else
  return (int)((b * magic) >> (64 - bits));
#endif
}

uint64 find_magic(int sq, int m, int bishop) {
  uint64 mask, b[4096], a[4096], used[4096], magic;
  int i, j, k, n, fail;

  mask = bishop? bmask(sq) : rmask(sq);
  n = count_1s(mask);

  for(i = 0; i < (1 << n); i++) {
    b[i] = index_to_uint64(i, n, mask);
    a[i] = bishop? batt(sq, b[i]) : ratt(sq, b[i]);
  }
  for(k = 0; k < 100000000; k++) {
    magic = random_uint64_fewbits();
    if(count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
    for(i = 0; i < 4096; i++) used[i] = 0ULL;
    for(i = 0, fail = 0; !fail && i < (1 << n); i++) {
      j = transform(b[i], magic, m);
      if(used[j] == 0ULL) used[j] = a[i];
      else if(used[j] != a[i]) fail = 1;
    }
    if(!fail) return magic;
  }
  printf("***Failed***\n");
  return 0ULL;
}

int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

int BBits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

void findMagics() {
  int square;

  printf("const uint64 RMagic[64] = {\n");
  for(square = 0; square < 64; square++)
    printf("  0x%llxULL,\n", find_magic(square, RBits[square], 0));
  printf("};\n\n");

  printf("const uint64 BMagic[64] = {\n");
  for(square = 0; square < 64; square++)
    printf("  0x%llxULL,\n", find_magic(square, BBits[square], 1));
  printf("};\n\n");
}
