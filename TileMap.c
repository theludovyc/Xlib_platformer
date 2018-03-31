#include "TileMap.h"

void TileMap_init(TileMap *tm, char *c, unsigned int w, unsigned int h, unsigned int tileSize){
	(*tm).mapW=w;
	(*tm).mapH=h;
	(*tm).tileSize=tileSize;

	FILE *f;

	f=fopen(c,"r");
	if(f==NULL){
		perror("fopen à échoué :");
		exit(-1);
	}

	(*tm).map=(char*)malloc(sizeof(char)*w*h);

	int i;
	char c2;
	for(i=0;i<w*h;i++){
		c2=fgetc(f);
		if(c2!=10){
			(*tm).map[i]=c2;
		}else{
			i--;
		}
	}

	fclose(f);
}

void TileMap_draw(TileMap *tm, Display *display, GC gc, Pixmap db, unsigned int *col){
	int i;
	int j;
	for(j=0;j<(*tm).mapH;j++){
		for(i=0;i<(*tm).mapW;i++){
			if((*tm).map[j*(*tm).mapW+i]=='0'){
				XSetForeground(display,gc,col[1]);
				XFillRectangle(display,db,gc,i*(*tm).tileSize,j*(*tm).tileSize,(*tm).tileSize,(*tm).tileSize);
			}else if((*tm).map[j*(*tm).mapW+i]=='1'){
				XSetForeground(display,gc,col[0]);
				XFillRectangle(display,db,gc,i*(*tm).tileSize,j*(*tm).tileSize,(*tm).tileSize,(*tm).tileSize);
			}
			
		}
	}
}

unsigned int TileMap_getTileIndex(TileMap *tm, unsigned int i){
	return i/(*tm).tileSize;
}

char TileMap_getTile(TileMap *tm, unsigned int row, unsigned int col){
	return (*tm).map[col*(*tm).mapW+row];
}

unsigned int TileMap_getTileSize(TileMap *tm){
	return (*tm).tileSize;
}

void TileMap_free(TileMap *tm){
	free((*tm).map);
}