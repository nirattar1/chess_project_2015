/*
 * gui_framework.c
 *
 *  Created on: Sep 5, 2015
 *      Author: nirattar
 */
#include "Gui_framework.h"
#include "Memory.h"

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

SDL_Surface * init_screen ()
{

	SDL_Surface * screen = NULL;
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

//find and notify relevant control of event.
Control * NotifyRelevantControl (SDL_Event * e,
		//DEBUG parameters
		Control * b1, Control * b2)
{
	Control * found = NULL;
	SDL_Rect * b1Rect = b1->rect;
	SDL_Rect * b2Rect = b2->rect;

	//check boundaries of the control
	if ((e->button.x > b1Rect->x) && (e->button.x < b1Rect->x + b1Rect->w) && (e->button.y > b1Rect->y) && (e->button.y < b1Rect->y+b1Rect->h))
	{
		//mark control as found.
		found = b1;
	}
	if ((e->button.x > b2Rect->x) && (e->button.x < b2Rect->x + b2Rect->w) && (e->button.y > b2Rect->y) && (e->button.y < b2Rect->y+b2Rect->h))
	{
		found = b2;
	}


	//real life - loop through all controls.


	//call the function of control ( if exists ).
	if (found)
	{
		found->HandleEvents(e);
	}
	return found;
}

//a loop for handling events.
void HandleEvents(
		//DEBUG parameters
		Control * b1,Control * b2
)
{
	int quit = 0;

	while (!quit)
	{
		/* Poll for keyboard & mouse events*/
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = 1;
					}
					break;
				case (SDL_MOUSEBUTTONUP):
					//send the event to the relevant control.
					;
					Control * matched = NotifyRelevantControl (&e, b1, b2);
					if (matched)
					{
						printf ("matched control!\n");
					}
					break;
				default:
					break;
			}
		}
		/* Wait a little before redrawing*/
		SDL_Delay(200);
	}
}


//generic constructing function for Control.
Control * ControlCreate(char * filename, SDL_Rect * rect)
{
	Control * ctl = mymalloc (sizeof (Control));
	ctl->surface = NULL;
	ctl->rect = rect;
	ctl->children = NULL;
	ctl->parent = NULL;

	//by default a control does nothing.
	ctl->HandleEvents = NULL;

	//load the image into ctl's surface
	bmp_load(filename, &ctl->surface);

	return ctl;
}

void ControlFree (Control * ctl)
{
	if (ctl)
	{
		//free attached surface
		if (ctl->surface)
		{
			SDL_FreeSurface(ctl->surface);
		}

		//free the struct itself
		myfree(ctl);
	}

}

//Window
Control * WindowCreate(char * filename, SDL_Rect * rect)
{
	Control * window = ControlCreate(filename, rect);
	window->Draw = WindowDraw;

	return window;
}

void WindowDraw(Control * window, SDL_Surface * screen)
{

	//update SDL surface .
	bmp_display(window->surface, NULL, screen);

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

}


