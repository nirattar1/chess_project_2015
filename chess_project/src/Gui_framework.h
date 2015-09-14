#include "ListNode.h"
#include <SDL.h>
#include <SDL_video.h>

#define WIN_W 800
#define WIN_H 600



//general GUI functions

//will take image and load it into surface buffer;
int bmp_load(char *file_name, SDL_Surface ** image);

//will display the source surface onto destination (screen).
void bmp_display(SDL_Surface * src, SDL_Rect * dstrect, SDL_Surface * dest);

//initializes SDL and returns screen object.
SDL_Surface * init_screen ();





//typedef enum
//{
//	CONTROL_BUTTON,
//	CONTROL_LABEL,
//	CONTROL_WINDOW,
//	CONTROL_PANEL
//} 	Control_Type_t;


//GUI Control struct
typedef struct Control
{
	//fields
	ListNode * children;
	struct Control * parent;
	//connection to SDL
	SDL_Surface * surface;
	SDL_Rect * rect;

	//functionality callbacks.
	void (*Draw)(struct Control * controlname, SDL_Surface * screen);
	void (*HandleEvents)(SDL_Event * event);

} Control;


//generic constructing function for Control.
Control * ControlCreate(char * filename, SDL_Rect * rect);

//Window
//Note: THERE'S ALWAYS ONLY ONE WINDOW, which is also the tree root.
Control * 	WindowCreate(char * filename, SDL_Rect * rect);
void		WindowDraw(Control * window, SDL_Surface * screen);
void 		WindowFree();

//Button
Control * 	ButtonCreate (char * filename, SDL_Rect * rect,
		void (*ButtonHandleEvents)(SDL_Event *));
void 		ButtonDraw (Control * button, SDL_Surface * screen);
void 		ButtonFree();

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
