CC=clang
CFLAGS=-g
LFLAG=-c
OUTFLAG=-o

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
BIN=lisp

.PHONY: clean

all: dbg

dbg: CFLAGS += -DNDEBUG -Wall -Wextra
dbg: build

release: CFLAGS = -O2 -Werror -Wextra
release: build

build: $(OBJ)
	$(CC) $(CFLAGS) $^ $(OUTFLAG) $(BIN)


%.o: %.c
	$(CC) $(CFLAGS) $(LFLAG) $< $(OUTFLAG) $@

clean:
	rm -f *.o $(BIN)
