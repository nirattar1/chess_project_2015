/*
 * gui_framework.c
 *
 *  Created on: Sep 5, 2015
 *      Author: nirattar
 */
#include "Gui_framework.h"
#include "Memory.h"
#include "SDL.h"

//general functions


//wrapper to SDL_Init
//Initializes SDL and make sure it quits
int init_sdl()
{
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	return 0;
}

SDL_Surface * init_screen (const char * title)
{

	SDL_Surface * screen = NULL;

	SDL_WM_SetCaption(title, NULL);

	/*
	 * Initialize the display in a 640x480 8-bit palettized mode,
	 * requesting a software surface
	 */
	//screen will be freed by SDL_Quit.
	screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_SWSURFACE | SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		fprintf(stderr, "Couldn't set video mode: %s\n",
						SDL_GetError());
		exit(1);
	}
	return screen;
}

//gets file name and surface and loads the BMP into it.
int bmp_load(char *file_name, SDL_Surface ** image)
{

    /* Load the BMP file into the surface */
    *image = SDL_LoadBMP(file_name);
    if (*image == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
        return 1;
    }
    return 0;

}

void bmp_display(SDL_Surface * src, SDL_Rect * dstrect, SDL_Surface * screen)
{
    /* Blit onto the screen surface */
    if(SDL_BlitSurface(src, NULL, screen, dstrect) < 0)
    {
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }

    //SDL_UpdateRect(dest, 0, 0, src->w, src->h);
    //flip instead of update
    SDL_Flip(screen);

    /* Free the allocated BMP surface. */
    SDL_FreeSurface(src);
}




//generic constructing function for Control.
Control * ControlCreate(char * filename, SDL_Rect * rect)
{
	Control * ctl = mymalloc (sizeof (Control));
	//handle memory failure.
	//TODO make global function/macro.
	if (!ctl)
	{
		return NULL;
	};

	ctl->surface = NULL;
	ctl->rect = rect;
	ctl->children = NULL;
	ctl->parent = NULL;

	//by default a control does nothing.
	ctl->HandleEvents = NULL;

	//load the image into ctl's surface
	bmp_load(filename, &(ctl->surface));

	return ctl;
}

void ControlFree (Control * ctl)
{
	if (ctl)
	{
		//free attached surface (if not deleted yet)
		if (ctl->surface)
		{
			SDL_FreeSurface(ctl->surface);
		}

		//free the struct itself
		myfree(ctl);
	}

}


void ControlAddChild(Control * control, Control * child)
{

	if (!control)
	{
		return;
	}


	//add child control to the list.
	ListNode ** listp = &control->children;
	ListPushBackElement (listp, (void *) child, sizeof (Control *));
	control->children = *listp;
}


void DFSTraverseDraw(Control * root, SDL_Surface * screen)
{
	//halt condition
	if (!root)
	{
		return;
	}

	//do the action with the root .
	//(draw it into screen)
	root->Draw(root, screen);

	//traverse all children (if exist),
	//and call DFS recursively for them
	if (root->children)
	{
		ListNode * pChild = root->children;
		for ( ; pChild !=NULL; pChild = pChild->next )
		{
			//get the data from child.
			Control * childControl = (Control *) pChild->data;
			//call recursively for child control (may be null).
			DFSTraverseDraw(childControl, screen);
		}
	}
}

//Window
Control * WindowCreate(char * filename, SDL_Rect * rect)
{
	Control * window = ControlCreate(filename, rect);

	//avoid null pointer
	if (!window)
	{
		return NULL;
	}

	window->Draw = WindowDraw;

	return window;
}

void WindowDraw(Control * window, SDL_Surface * screen)
{

	//update SDL surface .
	bmp_display(window->surface, NULL, screen);

	//the surface was freed, reset pointer.
	window->surface = NULL;
}




//Button
Control * ButtonCreate (char * filename, SDL_Rect * rect,
		void (*ButtonHandleEvents) (SDL_Event *))
{
	Control * button = ControlCreate(filename, rect);
	button->Draw = ButtonDraw;
	button->HandleEvents = ButtonHandleEvents;

	return button;
}

void ButtonDraw (Control * button, SDL_Surface * screen)
{

	//update SDL surface .
	//draw button inside it's rect.
	bmp_display(button->surface, button->rect, screen);

	//the surface was freed, reset pointer.
	button->surface = NULL;
}


