/*
 * Gui.h
 *
 *  Created on: Oct 11, 2015
 *      Author: nirattar
 */

#ifndef SRC_GUI_H_
#define SRC_GUI_H_

#include "Gui_framework.h"

//this class is responsible for all the GUI windows in program.

#define WINDOW_TITLE "Chess"

//an enum that for all the windows in the program
typedef enum
{
	GUI_WINDOW_MAIN_MENU,
	GUI_WINDOW_PLAYER_SELECTION,
	GUI_WINDOW_SAVE_LOAD,
	GUI_WINDOW_SET_BOARD,
	GUI_WINDOW_AI_SETTINGS,
	GUI_WINDOW_GAME
} gui_window_t;

//gui globals
extern gui_window_t _NextWindow; //start with main menu.

//responsible for creating menus, and handling events.
void Gui_Main ();

void Gui_Main_Game (game_state_t * game, Control * window, SDL_Surface * screen);


//will get the appropriate window object, based on requested window.
Control * Gui_GetNextWindow(gui_window_t window);

//"constructing" functions for all windows
Control * Menu_MainMenu_Create();
Control * Menu_PlayerSelection_Create();

void BoardPieceDraw (Control * button, SDL_Surface * screen);

#endif /* SRC_GUI_H_ */
