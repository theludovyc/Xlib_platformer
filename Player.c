#include "Player.h"

typedef struct Player{
	float x;
	float y;
	float dx;
	float dy;

	unsigned int width;
	unsigned int height;

	char left;
	char right;
	char jumping;
	char falling;

	float moveSpeed;
	float maxSpeed;
	float maxFallingSpeed;
	float stopSpeed;
	float jumpStart;
	float gravity;

	TileMap *tm;
	char topLeft;
	char topRight;
	char bottomLeft;
	char bottomRight;
}Player;

void Player_init(Player *p, TileMap *tm, int x, int y){
	(*p).x=x;
	(*p).y=y;
	(*p).tm=tm;

	(*p).width=20;
	(*p).height=20;

	(*p).moveSpeed=0.3;
	(*p).maxSpeed=2.1;
	(*p).maxFallingSpeed=6;
	(*p).stopSpeed=0.15;
	(*p).jumpStart=-7;
	(*p).gravity=0.32;
}

void Player_setLeftF(Player *p){
	(*p).left=0;
}

void Player_setLeftT(Player *p){
	(*p).left=1;
}

void Player_setRightF(Player *p){
	(*p).right=0;
}

void Player_setRightT(Player *p){
	(*p).right=1;
}

void Player_setJumping(Player *p){
	if(!(*p).falling){
		(*p).jumping=1;
	}
}

void Player_calculateCorners(Player *p, float x, float y){
	int left=TileMap_getTileIndex((*p).tm,(int)x);
	int right=TileMap_getTileIndex((*p).tm,(int)(x+(*p).width)-1);
	int top=TileMap_getTileIndex((*p).tm,(int)y);
	int bottom=TileMap_getTileIndex((*p).tm,(int)(y+(*p).height)-1);
	

	if(TileMap_getTile((*p).tm,left,top)=='0'){
		(*p).topLeft=1;
	}else{
		(*p).topLeft=0;
	}

	if(TileMap_getTile((*p).tm,right,top)=='0'){
		(*p).topRight=1;
	}else{
		(*p).topRight=0;
	}

	if(TileMap_getTile((*p).tm,left,bottom)=='0'){
		(*p).bottomLeft=1;
	}else{
		(*p).bottomLeft=0;
	}

	if(TileMap_getTile((*p).tm,right,bottom)=='0'){
		(*p).bottomRight=1;
	}else{
		(*p).bottomRight=0;
	}
}

void Player_update(Player *p){
	int currCol=TileMap_getTileIndex((*p).tm,(*p).x+(*p).width/2);
	int currRow=TileMap_getTileIndex((*p).tm,(*p).y+(*p).height/2);

	float tox;
	float toy;
	float tmpx;
	float tmpy;

	if((*p).left){
		(*p).dx-=(*p).moveSpeed;
		if((*p).dx<-(*p).maxSpeed){
			(*p).dx=-(*p).maxSpeed;
		}
	}else if((*p).right){
		(*p).dx+=(*p).moveSpeed;
		if((*p).dx>(*p).maxSpeed){
			(*p).dx=(*p).maxSpeed;
		}
	}else{
		if((*p).dx>0){
			(*p).dx-=(*p).stopSpeed;
			if((*p).dx<0){
				(*p).dx=0;
			}
		}else if((*p).dx<0){
			(*p).dx+=(*p).stopSpeed;
			if((*p).dx>0){
				(*p).dx=0;
			}
		}
	}

	if((*p).jumping){
		(*p).dy=(*p).jumpStart;
		(*p).falling=1;
		(*p).jumping=0;
	}
	
	if((*p).falling){
		(*p).dy+=(*p).gravity;
		if((*p).dy>(*p).maxFallingSpeed){
			(*p).dy=(*p).maxFallingSpeed;
		}
	}else{
		(*p).dy=0;
	}

	tox=(*p).x+(*p).dx;
	toy=(*p).y+(*p).dy;

	tmpx=(*p).x;
	tmpy=(*p).y;

	Player_calculateCorners(p,(*p).x,toy);
	if((*p).dy<0){
		if((*p).topLeft||(*p).topRight){
			(*p).dy=0;
			tmpy=currRow*TileMap_getTileSize((*p).tm);
		}else{
			tmpy+=(*p).dy;
		}
	}else{
		if((*p).bottomLeft||(*p).bottomRight){
			(*p).dy=0;
			(*p).falling=0;
			tmpy=(currRow+1)*TileMap_getTileSize((*p).tm)-(*p).height;
		}else{
			tmpy+=(*p).dy;
		}
	}

	Player_calculateCorners(p,tox,(*p).y);
	if((*p).dx<0){
		if((*p).topLeft||(*p).bottomLeft){
			(*p).dx=0;
			tmpx=currCol*TileMap_getTileSize((*p).tm);
		}else{
			tmpx+=(*p).dx;
		}
	}else{
		if((*p).topRight||(*p).bottomRight){
			(*p).dx=0;
			tmpx=(currCol+1)*TileMap_getTileSize((*p).tm)-(*p).width;
		}else{
			tmpx+=(*p).dx;
		}
	}

	if(!(*p).falling){
		Player_calculateCorners(p,(*p).x,(*p).y+1);
		if(!(*p).bottomLeft&&!(*p).bottomRight){
			(*p).falling=1;
		}
	}

	(*p).x=tmpx;
	(*p).y=tmpy;
}

void Player_draw(Player *p, Display *display, GC gc, Pixmap db, unsigned int pixel){
	XSetForeground(display,gc,pixel);
	XFillRectangle(display,db,gc,(int)(*p).x,(int)(*p).y,(*p).width,(*p).height);
}