CC = gcc
CFLAGS = -g -pedantic -std=c11 -W -Wall -Wextra -pthread -Wreturn-type
.PHONY : clean

all : build
build: main.c 
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f lab5
