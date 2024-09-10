CC = gcc
CFLAGS = -O2 -pedantic -W -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE
.PHONY: clean

all: sort_index generator
sort_index: sort_index.c Makefile
	$(CC) $(CFLAGS) sort_index.c -o sort_index
generator: genfile.c Makefile
	$(CC) $(CFLAGS) genfile.c -o generator
clean:
	rm -rf sort_index generator
