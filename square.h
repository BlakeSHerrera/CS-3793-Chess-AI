/**
 * square.h contains useful macros for readability. A square on the board
 * is indexed to an array index.
 * @author Blake Herrera
 * @date 2023-02-15
 */

#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#define NUM_SQUARES 64

#define A1 0
#define B1 1
#define C1 2
#define D1 3
#define E1 4
#define F1 5
#define G1 6
#define H1 7
#define A2 8
#define B2 9
#define C2 10
#define D2 11
#define E2 12
#define F2 13
#define G2 14
#define H2 15
#define A3 16
#define B3 17
#define C3 18
#define D3 19
#define E3 20
#define F3 21
#define G3 22
#define H3 23
#define A4 24
#define B4 25
#define C4 26
#define D4 27
#define E4 28
#define F4 29
#define G4 30
#define H4 31
#define A5 32
#define B5 33
#define C5 34
#define D5 35
#define E5 36
#define F5 37
#define G5 38
#define H5 39
#define A6 40
#define B6 41
#define C6 42
#define D6 43
#define E6 44
#define F6 45
#define G6 46
#define H6 47
#define A7 48
#define B7 49
#define C7 50
#define D7 51
#define E7 52
#define F7 53
#define G7 54
#define H7 55
#define A8 56
#define B8 57
#define C8 58
#define D8 59
#define E8 60
#define F8 61
#define G8 62
#define H8 63

#define charsToSquare(file, rank) \
    (((file)-'a') * 8 + (rank) - '1')
#define getRank(square) \
    ((square) >> 3)
#define getFile(square) \
    ((square) & 0b111)

typedef unsigned char Square;

extern char *SQUARE_TO_STR[64];

#endif // SQUARE_H_INCLUDED
