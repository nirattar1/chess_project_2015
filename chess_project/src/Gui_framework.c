/*
 * gui_framework.c
 *
 *  Created on: Sep 5, 2015
 *      Author: nirattar
 */
#include "Gui_framework.h"
#include "Memory.h"
#include "SDL.h"

//GUI globals initializations
int _QuitCurrentWindow = 0;
int _QuitProgram = 0;



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


void ControlFree (void * ctl_p)
{
	if (!ctl_p)
	{
		return;
	}

	//cast to control type.
	Control * ctl = (Control *) ctl_p;

	//free attached surface (if not deleted yet)
	if (ctl->surface)
	{
		SDL_FreeSurface(ctl->surface);
	}

	//free attached rect (if exists)
	if (ctl->rect)
	{
		myfree(ctl->rect);
		ctl->rect=NULL;
	}

	//free list of children
	//(ControlFree will be called recursively on all children).
	if (ctl->children)
	{
		ListFreeElements(ctl->children, ControlFree);
	}

	//free the struct itself (with all static data).
	myfree(ctl);

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

	//link child to its parent
	child->parent = control;
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

void DFSNotifyRelevantControl (SDL_Event * e, Control * root)
{
	//halt condition
	if (!root)
	{
		return;
	}

	//check that event falls in boundaries of the control's rect
	//(if exists)
	if (root->rect)
	{
		SDL_Rect * rect = root->rect;

		if ((e->button.x > rect->x) && (e->button.x < rect->x + rect->w)
				&& (e->button.y > rect->y) && (e->button.y < rect->y+rect->h))
		{

			//if the control has a handler, invoke it.
			if (root->HandleEvents)
			{
				root->HandleEvents(e);
			}
		}
	}

	//traverse all children (if exist) in DFS,
	//and notify them recursively.
	if (root->children)
	{
		ListNode * pChild = root->children;
		for ( ; pChild !=NULL; pChild = pChild->next )
		{
			//get the data from child.
			Control * childControl = (Control *) pChild->data;
			//call recursively for child control (may be null).
			DFSNotifyRelevantControl (e, childControl);
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



void HandleEventsLoop(Control * window)
{

	while (!_QuitCurrentWindow)
	{
		/* Poll for keyboard & mouse events*/
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
				case (SDL_QUIT):
					_QuitCurrentWindow = 1;
					_QuitProgram = 1;
					break;
				case (SDL_KEYUP):
					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						//TODO handle -quit window? program?
					}
					break;
				case (SDL_MOUSEBUTTONUP):
					//send the event to the relevant control.
					DFSNotifyRelevantControl (&e, window);
					break;
				default:
					break;
			}
		}

		/* Wait a little before redrawing*/
		SDL_Delay(200);
	}

	//reset quit window indication.
	_QuitCurrentWindow = 0;
	//go back to caller.
}
