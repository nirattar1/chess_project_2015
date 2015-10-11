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
game_state_t * _CurrentGame = NULL;


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

	//configure game mode
	Settings_GameMode_Set(GAME_MODE_PLAYER_VS_CPU);

	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_GAME; //TODO pass in settings
}

void Handler_PlayerVsPlayer()
{
	DEBUG_PRINT (("player vs. player\n"));

	//configure game mode
	Settings_GameMode_Set(GAME_MODE_PLAYER_VS_PLAYER);

	//
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_GAME; //TODO pass in settings
}

void Handler_Cancel()
{
	DEBUG_PRINT (("cancel\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_MAIN_MENU;	//go back to main menu.

}

void Handler_SaveGame()
{
	//TODO implement
	DEBUG_PRINT (("save game\n"));
	//_QuitCurrentWindow = 1;
	//_NextWindow = GUI_WINDOW_MAIN_MENU;	//go back to main menu.

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


static void UpdateBoard(game_state_t * game, Control * panel_GameBoard)
{

	//build board.
	int x_off = 0;
	int y_off = 0;
	const int board_piece_width = 65;
	const int board_piece_height = 65;
	//create board squares, add them to board panel
	for (int j=BOARD_SIZE; j>0; j--)
	{

		//start from beginning of row
		x_off = 0;

		//iterate on columns
		for (int i=0; i<BOARD_SIZE; i++)
		{
			//create button with image
			SDL_Rect * button_BoardPiece_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
			button_BoardPiece_rect->x = x_off;
			button_BoardPiece_rect->y = y_off;

			//build position from loop
			position_t pos = Position ('a'+i, j);
			DEBUG_PRINT(("building button for position <%c,%d>\n", pos.x, pos.y));
			color_t square_color = ((i%2==0 && j%2==0)||(i%2!=0 &&j%2!=0)) ? COLOR_BLACK : COLOR_WHITE;


			char * filename = (square_color==COLOR_BLACK) ? "imgs/empty_blck.bmp":"imgs/empty_wht.bmp" ;
			Control * button_BoardPiece = ButtonCreate( filename, button_BoardPiece_rect, Handler_Quit);

			//progress to next column
			x_off += board_piece_width;

			//link button to panel
			ControlAddChild(panel_GameBoard, button_BoardPiece);
		}

		//finished with row, progress to next row.
		y_off += board_piece_height;

	}
}


Control * Menu_GameWindow_Create()
{
	//2 panels:
	//* game options panel
	//* game board

	//window (has no rect).
	Control * window = WindowCreate("imgs/background.bmp", NULL);
	//handle error in creation of window.
	if (!window)
	{
		return NULL;
	}

	//game options panel
	SDL_Rect * panel_GameOptions_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	panel_GameOptions_rect->x = 600; panel_GameOptions_rect->y = 0;
	panel_GameOptions_rect->w = 0; panel_GameOptions_rect->h = 0;
	Control * panel_GameOptions = PanelCreate("imgs/background.bmp", panel_GameOptions_rect);

	//buttons inside game options.
	SDL_Rect * button_SaveGame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_SaveGame_rect->x = 600; button_SaveGame_rect->y = 0;
	button_SaveGame_rect->w = 0; button_SaveGame_rect->h = 0;
	Control * button_SaveGame = ButtonCreate("imgs/Save_Game.bmp", button_SaveGame_rect, Handler_SaveGame);
	//TODO more buttons

	//game board panel
	SDL_Rect * panel_GameBoard_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	panel_GameBoard_rect->x = 0; panel_GameBoard_rect->y = 0;
	panel_GameBoard_rect->w = 0; panel_GameBoard_rect->h = 0;
	Control * panel_GameBoard = PanelCreate("imgs/background.bmp", panel_GameBoard_rect);

	//link panels to window
	ControlAddChild(window, panel_GameBoard);
	ControlAddChild(window, panel_GameOptions);
	//link buttons to game options panel
	ControlAddChild(panel_GameOptions, button_SaveGame);


	//get current game.
	game_state_t * game = _CurrentGame;
	if (!game)
	{
		DEBUG_PRINT(("Error: game window with no active."));
		return NULL;
	}

	//print board based on game.
	UpdateBoard(game, panel_GameBoard);
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
		case GUI_WINDOW_GAME:
			return Menu_GameWindow_Create();
			break;
	}

	//default
	return NULL;
}


void Gui_Main(game_state_t * game)
{

	if (!game)
	{
		exit(1);
	}
	//update global game
	_CurrentGame = game;

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


