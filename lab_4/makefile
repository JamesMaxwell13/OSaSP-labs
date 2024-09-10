CC = gcc
CFLAGS = -std=gnu11 -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -D_DEFAULT_SOURCE

MAIN = main.c producer.c consumer.c init_and_exit.c ipc.c

main: $(MAIN)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: run
run:
	make main
	./main
	make clean

.PHONY: clean
clean:
	rm main