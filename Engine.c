#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#define WIDTH 1024
#define HEIGHT 768

Display *display;
int screen;
Window root;
Visual *visual;
int depth;
unsigned int col[2];

Pixmap db;

XGCValues gcvalues;
GC gc;

XSetWindowAttributes xswa;
Window win;

float TimeCounter, LastFrameTimeCounter, DT, prevTime=0.0, FPS, waitTime;
struct timeval tv, tv0;
int Frame=1, FramesPerFPS;

char running;

void init();

void event(XEvent ev);

void update();

void draw();

void render(){
	XCopyArea(display,db,win,gc,0,0,WIDTH,HEIGHT,0,0);
	XFlush(display);
}

void loop(){
	init();

	XEvent ev;

	gettimeofday(&tv0,NULL);

	int a,b;

	draw();

	running=1;
	FramesPerFPS=60;
	while(running){
		LastFrameTimeCounter=TimeCounter;
		gettimeofday(&tv,NULL);
		TimeCounter=(float)(tv.tv_sec-tv0.tv_sec)+0.000001*((float)(tv.tv_usec-tv0.tv_usec));
		DT=TimeCounter-LastFrameTimeCounter;

		Frame++;
		if((Frame%FramesPerFPS)==0){
			FPS=((float)(FramesPerFPS))/(TimeCounter-prevTime);
			prevTime=TimeCounter;
		}

		XSync(display, 0L);
		a=XPending(display);
		for(b=a;b>0;b--){
			XNextEvent(display,&ev);

			event(ev);
		}

		update();
		draw();
		render();

		waitTime=1000000/FramesPerFPS-DT;
		
		if(waitTime>0){
			usleep(waitTime);
		}
	}
}

void end();

void close_x(){
	XFreeGC(display,gc);
	XDestroyWindow(display,win);
	XCloseDisplay(display);
}

int main(int ac, char **av){
	char *dpyn=NULL;

	if(ac==3){
		if(strcmp(&av[1][1],"display")==0){
			dpyn=av[2];
		}else{
			fprintf(stderr,"Usage: xsimple[-display display_name]\n");
			exit(1);
		}
	}

	if((display=XOpenDisplay(dpyn))==NULL){
		fprintf(stderr,"Can't open Display\n");
		exit(1);
	}

	screen=DefaultScreen(display);
	root=RootWindow(display,screen);
	visual=DefaultVisual(display,screen);
	depth=DefaultDepth(display,screen);

	col[0]=WhitePixel(display,screen);
	col[1]=BlackPixel(display,screen);

	db=XCreatePixmap(display,root,WIDTH,HEIGHT,depth);

	gcvalues.foreground=col[1];
	gcvalues.background=col[0];

	gc=XCreateGC(display,root,(GCForeground|GCBackground),&gcvalues);

	xswa.background_pixel=col[0];
	xswa.event_mask=KeyPressMask|KeyReleaseMask|PointerMotionMask;
	xswa.border_pixel=col[1];

	win=XCreateWindow(display,root,0,0,WIDTH,HEIGHT,3,depth,InputOutput,visual,CWEventMask|CWBorderPixel|CWBackPixel,&xswa);

	XMapWindow(display, win);

	loop();

	end();

	close_x();

	return 0;
}