/*
 * Gui.c
 *
 *  Created on: Oct 11, 2015
 *      Author: nirattar
 */

#include "Gui.h"
#include "Gui_framework.h"
#include "Memory.h"


//TODO remove
void testfunction3()
{
	printf ("new\n");
}

void testfunction4()
{
	printf ("load\n");
}

void testfunction5()
{
	printf ("quit\n");
}

//builds main menu
Control * Menu_MainMenu_Create()
{
	//create objects and attach them to each other.

	//window (has no rect).
	Control * w1 = WindowCreate("imgs/background.bmp", NULL);
	//handle error in creation of window.
	if (!w1)
	{
		return NULL;
	}

	//buttons under window
	//new game, load game, quit.

	//new game button
	SDL_Rect * button_NewGame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_NewGame_rect->x = 20; button_NewGame_rect->y = 100;
	button_NewGame_rect->w = 20; button_NewGame_rect->h = 30;
	Control * button_NewGame = ButtonCreate("imgs/new_g.bmp", button_NewGame_rect, testfunction3);
//
	SDL_Rect * button_LoadGame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_LoadGame_rect->x = 20; button_LoadGame_rect->y = 200;
	button_LoadGame_rect->w = 20; button_LoadGame_rect->h = 30;
	Control * button_LoadGame = ButtonCreate("imgs/load_g.bmp", button_LoadGame_rect, testfunction4);

	SDL_Rect * button_Quit_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Quit_rect->x = 20; button_Quit_rect->y = 300;
	button_Quit_rect->w = 20; button_Quit_rect->h = 30;
	Control * button_Quit = ButtonCreate("imgs/exit.bmp", button_Quit_rect, testfunction5);


	//link the objects to window
	ControlAddChild(w1, button_NewGame);
	ControlAddChild(w1, button_LoadGame);
	ControlAddChild(w1, button_Quit);

	return w1;
}

void Gui_Main()
{

	//init SDL
	if (init_sdl() != 0)
	{
		exit (1);
	}


	//init screen
	//(screen is a global surface).
	//(will be freed by SDL_Quit).
	SDL_Surface * screen = init_screen(WINDOW_TITLE);

	//create main menu
	Control * w1 = Menu_MainMenu_Create();

	//draw the objects.
	//(DFS traverse)
	DFSTraverseDraw (w1, screen);


	//handle events until exit.
	//TestHandleEvents(b1, b2);
	SDL_Delay(5000);
	//clear window (will free all children objects)
	ControlFree(w1);
}

