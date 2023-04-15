# Define the compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Define the source files and object files
SRCS = main.c bitboard.c move.c movegen.c piece.c position.c search.c square.c Release/uci.c Release/magic.c Release/debug.c
OBJS = $(addprefix obj/, $(SRCS:.c=.o))

# Define the build targets and dependencies
# chess: $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) -o chess
# bin/Debug/CS-3793-Chess-AI: main.o bitboard.o move.o movegen.o piece.o position.o square.o uci.o magic.o debug.o
# 	$(CC) $(CFLAGS) $(OBJS) -o bin/Debug/CS-3793-Chess-AI
bin/Release/CS-3793-Chess-AI.exe: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o bin/Release/CS-3793-Chess-AI.exe

obj/main.o: main.c bitboard.h debug.h move.h movegen.h piece.h position.h search.h square.h uci.h magic.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/bitboard.o: bitboard.c bitboard.h piece.h square.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/move.o: move.c move.h piece.h position.h square.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/movegen.o: movegen.c movegen.h square.h bitboard.h debug.h magic.h position.h move.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/piece.o: piece.c piece.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/position.o: position.c position.h bitboard.h piece.h square.h movegen.h magic.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/search.o: search.c bitboard.h move.h movegen.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/square.o: square.c square.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Debug/uci.o: uci.c uci.h bitboard.h debug.h move.h movegen.h piece.h position.h square.h magic.h #stdlib.h stdio.h string.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Release/uci.o: uci.c uci.h bitboard.h debug.h move.h movegen.h piece.h position.h square.h magic.h #stdlib.h stdio.h string.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Debug/magic.o: magic.c magic.h debug.h bitboard.h movegen.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Release/magic.o: magic.c magic.h debug.h bitboard.h movegen.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Debug/debug.o: debug.c debug.h bitboard.h square.h position.h move.h
	$(CC) $(CFLAGS) -c $< -o $@

obj/Release/debug.o: debug.c debug.h bitboard.h square.h position.h move.h
	$(CC) $(CFLAGS) -c $< -o $@

# Define a clean target to remove object files and the executable
clean:
	rm -f $(OBJS) chess

