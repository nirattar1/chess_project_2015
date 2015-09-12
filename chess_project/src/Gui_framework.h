#include "ListNode.h"
#include <SDL.h>
#include <SDL_video.h>

#define WIN_W 800
#define WIN_H 600



//general GUI functions

//will take image and load it into surface buffer;
int bmp_load(char *file_name, SDL_Surface ** image);

//will display the source surface onto destination (screen).
void bmp_display(SDL_Surface * src, SDL_Surface * dest);

//initializes SDL and returns screen object.
SDL_Surface * init_screen ();


//handle events
void HandleEvents();

//
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
	int x, y;	//RELATIVE position.
	int w, h;	//width height.
	//Control_Type_t type;
	ListNode * children;
	struct Control * parent;
	//connection to SDL
	SDL_Surface * surface;



	//functionality callbacks.
	void (*Draw)(struct Control * controlname, SDL_Surface * screen);
	void (*HandleEvents)(SDL_Event * event);

} Control;


//Window
//Note: THERE'S ALWAYS ONLY ONE WINDOW, which is also the tree root.
Control * 	WindowCreate();
void		WindowDraw(Control * window, SDL_Surface * screen);
void		WindowHandleEvents(SDL_Event * event);
void 		WindowFree();

//Button
Control * 	ButtonCreate ();
void 		ButtonDraw (Control * button, SDL_Surface * screen);
void		ButtonHandleEvents(SDL_Event * event);
void 		ButtonFree();
//Label
//Panel


//
