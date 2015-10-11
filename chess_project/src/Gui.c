/*
 * Gui.c
 *
 *  Created on: Oct 11, 2015
 *      Author: nirattar
 */

#include "Game_Mgr.h"
#include "Gui.h"
#include "Gui_framework.h"
#include "Memory.h"


gui_window_t _NextWindow = GUI_WINDOW_MAIN_MENU; //start with main menu.



//TODO remove
void Handler_New()
{
	DEBUG_PRINT (("new\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_PLAYER_SELECTION;
}

void Handler_Load()
{
	DEBUG_PRINT (("load\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_SAVE_LOAD;

}

void Handler_Quit()
{
	DEBUG_PRINT (("quit\n"));
	_QuitCurrentWindow = 1;
	_QuitProgram = 1;

}

void Handler_PlayerVsCpu()
{
	DEBUG_PRINT (("player vs. cpu\n"));
//	_QuitCurrentWindow = 1;
//	_NextWindow = 1;
}

void Handler_PlayerVsPlayer()
{
	DEBUG_PRINT (("player vs. player\n"));
//	_QuitCurrentWindow = 1;
//	_NextWindow = 1;
}

void Handler_Cancel()
{
	DEBUG_PRINT (("cancel\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_MAIN_MENU;	//go back to main menu.

}

//builds main menu
Control * Menu_MainMenu_Create()
{

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
	button_NewGame_rect->x = 310; button_NewGame_rect->y = 150;
	button_NewGame_rect->w = 20; button_NewGame_rect->h = 30;
	Control * button_NewGame = ButtonCreate("imgs/new_g.bmp", button_NewGame_rect, Handler_New);

	//load game button
	SDL_Rect * button_LoadGame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_LoadGame_rect->x = 310; button_LoadGame_rect->y = 250;
	button_LoadGame_rect->w = 20; button_LoadGame_rect->h = 30;
	Control * button_LoadGame = ButtonCreate("imgs/load_g.bmp", button_LoadGame_rect, Handler_Load);

	//quit button
	SDL_Rect * button_Quit_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Quit_rect->x = 310; button_Quit_rect->y = 350;
	button_Quit_rect->w = 20; button_Quit_rect->h = 30;
	Control * button_Quit = ButtonCreate("imgs/exit.bmp", button_Quit_rect, Handler_Quit);


	//link the objects to window
	ControlAddChild(w1, button_NewGame);
	ControlAddChild(w1, button_LoadGame);
	ControlAddChild(w1, button_Quit);

	return w1;
}

Control * Menu_PlayerSelection_Create()
{

	//window (has no rect).
	Control * window = WindowCreate("imgs/background.bmp", NULL);
	//handle error in creation of window.
	if (!window)
	{
		return NULL;
	}

	//buttons under window
	//player vs. cpu, player vs. player, cancel
	//TODO set board, next player.

	//player vs. cpu
	SDL_Rect * button_PlayerVsCpu_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_PlayerVsCpu_rect->x = 270; button_PlayerVsCpu_rect->y = 150;
	button_PlayerVsCpu_rect->w = 20; button_PlayerVsCpu_rect->h = 30;
	Control * button_PlayerVsCpu = ButtonCreate("imgs/Player_vs_Computer.bmp", button_PlayerVsCpu_rect, Handler_PlayerVsCpu);

	//player vs. player
	SDL_Rect * button_PlayerVsPlayer_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_PlayerVsPlayer_rect->x = 270; button_PlayerVsPlayer_rect->y = 250;
	button_PlayerVsPlayer_rect->w = 20; button_PlayerVsPlayer_rect->h = 30;
	Control * button_PlayerVsPlayer = ButtonCreate("imgs/player_vs_player.bmp", button_PlayerVsPlayer_rect, Handler_PlayerVsPlayer);

	//cancel
	SDL_Rect * button_Cancel_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Cancel_rect->x = 20; button_Cancel_rect->y = 350;
	button_Cancel_rect->w = 20; button_Cancel_rect->h = 30;
	Control * button_Cancel = ButtonCreate("imgs/Cancel.bmp", button_Cancel_rect, Handler_Cancel);


	//link the objects to window
	ControlAddChild(window, button_PlayerVsCpu);
	ControlAddChild(window, button_PlayerVsPlayer);
	ControlAddChild(window, button_Cancel);

	return window;
}

Control * Gui_GetNextWindow(gui_window_t window)
{
	switch (window)
	{
		case GUI_WINDOW_MAIN_MENU:
			return Menu_MainMenu_Create();
			break;
		case GUI_WINDOW_PLAYER_SELECTION:
			return Menu_PlayerSelection_Create();
			break;
	}

	//default
	return NULL;
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

	//a loop that will switch between windows.
	//will maintain a reference to the active window.
	//upon window close, will load the next window.
	while (!_QuitProgram)
	{
		//create main menu
		Control * window = Gui_GetNextWindow(_NextWindow);
		if (!window)
		{
			DEBUG_PRINT(("Error: program continues but no next window."));
		}

		//draw the objects to screen. (with DFS traverse)
		DFSTraverseDraw (window, screen);

		//handle events in loop, until exit.
		HandleEventsLoop(window);

		//loop was done (window exited).
		//clear it (will free all children objects)
		ControlFree((void *) window);
	}

	//done with program, will go back to main.
}


