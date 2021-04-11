CC=gcc
CFLAGS=-Wall -pedantic

default: build run

build:
	$(CC) $(CFLAGS) -I include src/**.c -o snake

build_debug:
	$(CC) $(CFLAGS) -g -I include src/**.c -o snake

run:
	./snake
