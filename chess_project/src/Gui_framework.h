#include "ListNode.h"
#include <SDL.h>
#include <SDL_video.h>

#define WIN_W 800
#define WIN_H 600



//general GUI functions

//initializes SDL and returns screen object.
SDL_Surface * init_screen ();

//will take image and load it into surface buffer;
int bmp_load(char *file_name, SDL_Surface ** image);

//a wrapper around SDL Blit function.
//will display the source surface onto destination (screen).
//will also free source surface !
void bmp_display(SDL_Surface * src, SDL_Rect * dstrect, SDL_Surface * dest);




//typedef enum
//{
//	CONTROL_BUTTON,
//	CONTROL_LABEL,
//	CONTROL_WINDOW,
//	CONTROL_PANEL
//} 	Control_Type_t;


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
	void (*Draw)(struct Control * controlname, SDL_Surface * screen);
	void (*HandleEvents)(SDL_Event * event);

} Control;


//generic constructing function for Control.
Control * ControlCreate(char * filename, SDL_Rect * rect);
//generic freeing function for Control.
void ControlFree (Control * ctl) ;


//Window
//Note: THERE'S ALWAYS ONLY ONE WINDOW, which is also the tree root.
Control * 	WindowCreate(char * filename, SDL_Rect * rect);
void		WindowDraw(Control * window, SDL_Surface * screen);

//Button
Control * 	ButtonCreate (char * filename, SDL_Rect * rect,
		void (*ButtonHandleEvents)(SDL_Event *));
void 		ButtonDraw (Control * button, SDL_Surface * screen);

//Label
//Panel


//
/////////////////////
//EVENT HANDLING
/////////////////////

//find and notify relevant control of event.
Control * NotifyRelevantControl (SDL_Event * e, Control * b1, Control * b2);

//handle events
void HandleEvents();

/////////////////////
