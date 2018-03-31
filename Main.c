#include "Engine.c"
#include "TileMap.h"
#include "Player.c"

TileMap tm;

Player p;

int keysymsPer;
KeySym *keySym;

void init(){
	TileMap_init(&tm,"test.map",32,24,32);

	Player_init(&p,&tm,32,HEIGHT-64);
}

void event(XEvent ev){
	switch(ev.type){
		case KeyPress:
			keySym=XGetKeyboardMapping(display,ev.xkey.keycode,1,&keysymsPer);
			switch(keySym[0]){
				case XK_Escape:
					running=0;
					break;

				case XK_Up:
					Player_setJumping(&p);
					break;

				case XK_Left:
					Player_setLeftT(&p);
					break;

				case XK_Right:
					Player_setRightT(&p);
					break;
			}
			XFree(keySym);
			break;

		case KeyRelease:
			keySym=XGetKeyboardMapping(display,ev.xkey.keycode,1,&keysymsPer);
			switch(keySym[0]){
				case XK_Left:
					Player_setLeftF(&p);
					break;

				case XK_Right:
					Player_setRightF(&p);
					break;
			}
			XFree(keySym);
			break;

		default:
			break;
	}
}

void update(){
	Player_update(&p);
}

void draw(){
	XSetForeground(display,gc,col[0]);
	XFillRectangle(display,db,gc,0,0,WIDTH,HEIGHT);

	TileMap_draw(&tm,display,gc,db,col);

	Player_draw(&p,display,gc,db,col[1]);
}

void end(){
	TileMap_free(&tm);
}