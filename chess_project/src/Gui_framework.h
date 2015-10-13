#ifndef SRC_GUI_FRAMEWORK_H_
#define SRC_GUI_FRAMEWORK_H_


#include "ListNode.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_video.h>


#define WIN_W 800
#define WIN_H 600


//gui globals
extern int _QuitCurrentWindow;
extern int _QuitProgram;



//general GUI functions

//inits SDL
int init_sdl();


//initializes SDL and returns screen object.
SDL_Surface * init_screen (const char *);

//will take image and load it into surface buffer;
int bmp_load(char *file_name, SDL_Surface ** image);





//GUI Control struct.
//contains SDL connection points (surface, rect),
//as well as information for our framework (part of controls tree).
typedef struct Control
{
	//fields
	ListNode * children;
	struct Control * parent;
	SDL_Surface * surface;
	SDL_Rect * rect;

	//functionality callbacks.
	void (*Draw)(struct Control * control, SDL_Surface * screen);
	void (*HandleEvents)(struct Control * control , SDL_Event * event);

	//TODO make dynamic and void *
	position_t pos;

} Control;


//generic constructing function for Control.
Control * ControlCreate(char * filename, SDL_Rect * rect);
//generic freeing function for Control.
//!important: freeing control also frees all it's children nodes.
void ControlFree (void * ctl_p) ;


/*
 * will allow adding a child control under specific control.
 *
 */
void ControlAddChild(Control * control, Control * child);

//visit UI tree in DFS, starting from root
void DFSTraverseDraw(Control * root, SDL_Surface * screen);

void DFSNotifyRelevantControl (SDL_Event * e, Control * window);


//Window
//Note: THERE'S ALWAYS ONLY ONE WINDOW, which is also the tree root.
Control * 	WindowCreate(char * filename, SDL_Rect * rect);
void		WindowDraw(Control * window, SDL_Surface * screen);

//Panel
Control * PanelCreate(char * filename, SDL_Rect * rect);
void PanelDraw (Control * panel, SDL_Surface * screen);



//Button
Control * ButtonCreate (char * filename, SDL_Rect * rect,
		void (*ButtonHandleEvents) (Control * control, SDL_Event *));

void 		ButtonDraw (Control * button, SDL_Surface * screen);

//Label
//Panel


//
/////////////////////
//EVENT HANDLING
/////////////////////

//find and notify relevant control of event.
Control * NotifyRelevantControl (SDL_Event * e);

//a loop for handling events.
void HandleEventsLoop(Control * window);

/////////////////////

#endif //SRC_GUI_FRAMEWORK_H_
