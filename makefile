CFLAGS=-c -Wall -lX11
LDFLAGS=-Wall -lX11
DESTFOL=~/Documents/TrucProg/Xlib/Platform1/
CC=gcc

all: Main

Main: Main.o TileMap.o
	$(CC) $^ -o Main $(LDFLAGS) && mv Main $(DESTFOL) && chmod +x $(DESTFOL)Main

Main.o: Main.c
	$(CC) $(CFLAGS) Main.c

TileMap.o: TileMap.c TileMap.h
	$(CC) $(CFLAGS) TileMap.c