#include "ListNode.h"
#include <SDL.h>
#include <SDL_video.h>



//general GUI functions

//will take image and display on given screen.
void display_bmp(char *file_name, SDL_Surface * screen);

//initializes SDL and returns screen object.
SDL_Surface * init_screen ();

typedef enum
{
	CONTROL_BUTTON,
	CONTROL_LABEL,
	CONTROL_WINDOW,
	CONTROL_PANEL
} 	Control_Type_t;


//GUI Control struct
typedef struct Control
{
	//fields
	int x;
	int y;
	Control_Type_t type;
	ListNode * children;
	struct Control * parent;

	//function pointers - for functions.
	void (*draw)();

} Control;

//Button
Control * 	ButtonCreate ();
void 		ButtonDraw ();

//Label
//Window
//Note: THERE'S ALWAYS ONLY ONE WINDOW, which is also the tree root.
//Panel


//
