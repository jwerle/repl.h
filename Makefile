
SRC = $(wildcard src/*.c)
CFLAGS = -std=c99 -Wall -Iinclude

all: clean test

clean:
	rm -f ./test-repl

test:
	$(CC) $(SRC) test.c $(CFLAGS) -o test-repl
	./test-repl

examples/node.c:
	$(CC) $(SRC) $@ $(CFLAGS) -o examples/node-repl

.PHONY: all clean test examples/node.c
