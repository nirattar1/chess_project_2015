/*
 * sdl_test2.c
 *
 *  Created on: Sep 3, 2015
 *      Author: nirattar
 */
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Gui_framework.h"
#define WIN_W 800
#define WIN_H 600




int main ()
{
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	SDL_Surface * screen;
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

    //display background image
    display_bmp("imgs/background.bmp", screen);

    int quit = 0 ;

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
    return 0;
}
