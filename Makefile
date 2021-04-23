CC=gcc
CFLAGS=-Wall -pedantic

default: build run

build:
	$(CC) $(CFLAGS) -I include src/**.c -o snake -lpthread

build_debug:
	$(CC) $(CFLAGS) -g -I include src/**.c -o snake -lpthread

run:
	./snake
