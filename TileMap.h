#ifndef _TileMap_h
#define _TileMap_h

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <X11/Xlib.h>

typedef struct TileMap{
	unsigned int mapW,mapH,tileSize;
	char *map;
}TileMap;

void TileMap_init(TileMap *tm, char *c, unsigned int w, unsigned int h, unsigned int ts);

void TileMap_draw(TileMap *tm, Display *d, GC gc, Pixmap db, unsigned int *col);

unsigned int TileMap_getTileIndex(TileMap *tm, unsigned int i);

char TileMap_getTile(TileMap *tm, unsigned int row, unsigned int col);

unsigned int TileMap_getTileSize(TileMap *tm);

void TileMap_free(TileMap *tm);

#endif