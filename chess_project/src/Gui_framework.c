/*
 * gui_framework.c
 *
 *  Created on: Sep 5, 2015
 *      Author: nirattar
 */
#include "Gui_framework.h"
#include "Memory.h"

//general functions



SDL_Surface * init_screen ()
{
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

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


void display_bmp(char *file_name, SDL_Surface * screen)
{
    SDL_Surface *image;

    /* Load the BMP file into a surface */
    image = SDL_LoadBMP(file_name);
    if (image == NULL) {
        fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
        return;
    }


    /* Blit onto the screen surface */
    if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
    {
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    }
    SDL_UpdateRect(screen, 0, 0, image->w, image->h);

    /* Free the allocated BMP surface */
    SDL_FreeSurface(image);
}

//Button
Control * ButtonCreate ()
{
	Control * button = mymalloc (sizeof (Control));
	button->draw = DrawButton();
	button->x = 0;
	button->y = 0;
	button->children = NULL;
	button->parent = NULL;
	return button;
}

void ButtonFree()
{

}

void * ButtonDraw ()
{
	//get relative x , y somehow .

	//update SDL surface .
	display_bmp("button1.bmp", screen);

}
