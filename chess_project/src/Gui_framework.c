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

void bmp_display(SDL_Surface * src, SDL_Surface * screen)
{
    /* Blit onto the screen surface */
    if(SDL_BlitSurface(src, NULL, screen, NULL) < 0)
    {
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }

    //SDL_UpdateRect(dest, 0, 0, src->w, src->h);
    //flip instead of update
    SDL_Flip(screen);

    /* Free the allocated BMP surface ? */
    SDL_FreeSurface(src);
}

//a loop for handling events.
void HandleEvents()
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
					//!
					//send the event to the relevant control.
					//if ((e.button.x > rect.x) && (e.button.x < rect.x + rect.w) && (e.button.y > rect.y) && (e.button.y < rect.y+rect.h))
					quit = 1;
					break;
				default:
					break;
			}
		}
		/* Wait a little before redrawing*/
		SDL_Delay(200);
	}
}

//Window
Control * WindowCreate(char * filename)
{
	Control * window = mymalloc (sizeof (Control));
	window->Draw = WindowDraw;
	window->HandleEvents = WindowHandleEvents;
	window->x = 0;
	window->y = 0;
	window->w = WIN_W;
	window->h =	WIN_H;
	window->surface = NULL;
	window->children = NULL;
	window->parent = NULL;

	//load the background into window's surface
	bmp_load(filename, &window->surface);

	return window;
}

void WindowDraw(Control * window, SDL_Surface * screen)
{
	//get relative x , y somehow .

	//update SDL surface .
	bmp_display(window->surface, screen);

}

void WindowFree()
{
	;
}

void WindowHandleEvents(SDL_Event * event)
{
	;
}

//Button
Control * ButtonCreate (char * filename)
{
	Control * button = mymalloc (sizeof (Control));
	button->Draw = ButtonDraw;
	button->HandleEvents = ButtonHandleEvents;
	button->x = 0;
	button->y = 0;
	button->w = 30;
	button->h = 30;
	button->surface = NULL;
	button->children = NULL;
	button->parent = NULL;

	//load the image into button's surface
	bmp_load(filename, &button->surface);


	return button;
}

void ButtonDraw (Control * button, SDL_Surface * screen)
{
	//get relative x , y somehow .

	//update SDL surface .
	bmp_display(button->surface, screen);

}

void ButtonFree()
{

}

void ButtonHandleEvents(SDL_Event * event)
{
	;
}
