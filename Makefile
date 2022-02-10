CC=gcc
CFLAGS=-g -DNDEBUG
LFLAG=-c
OUTFLAG=-o

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
BIN=lisp

.PHONY: clean

all: $(OBJ)
	$(CC) $(CFLAGS) $^ $(OUTFLAG) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(LFLAG) $< $(OUTFLAG) $@

clean:
	rm -f $(OBJ)
