
CC = gcc
CFLAGS = -std=gnu11 -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -D_DEFAULT_SOURCE

_CHILD = main.c child.c
CHILD = $(patsubst %,src/child/%,$(_CHILD))

_PARENT = main.c parent.c
PARENT = $(patsubst %,src/parent/%,$(_PARENT))

child: $(CHILD)
	$(CC) $(CFLAGS) -o $@ $^

parent: $(PARENT)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: run
run:
	make child
	make parent
	./parent
	make clean

.PHONY: clean
clean:
	rm child parent