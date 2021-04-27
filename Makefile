CC=gcc
CFLAGS=-Wall -pedantic

default: build run

build:
	$(CC) $(CFLAGS) -I include src/**.c -o snake -lpthread -Llib -l:libvulcalien.a

build_debug:
	$(CC) $(CFLAGS) -g -I include src/**.c -o snake -lpthread -Llib -l:libvulcalien.a

run:
	./snake
