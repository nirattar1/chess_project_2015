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
#include "Files.h"

gui_window_t 	_NextWindow = GUI_WINDOW_MAIN_MENU; //start with main menu.
game_state_t * 	_CurrentGame = NULL; //a pointer to current game.
Control * 		_CurrentWindow = NULL;
SDL_Surface * 	_CurrentScreen = NULL; //TODO remove
move_t 			_BoardPieceNextMove;

//TODO make enum
//0==none is selected.
//1==selected and display move.
//2==selected and legal move (ready to perform).
int _BoardPieceSelection = 0;
position_t _BoardPieceMoveSrc;


//static function declarations
static void BoardPieceFindPictureByIdentity(position_t pos, char identity, char * filename);
static void BuildBoard(game_state_t * game, Control * panel_GameBoard);



//TODO remove
void Handler_New()
{
	DEBUG_PRINT (("new\n"));
	//restart game
	game_state_t * game = _CurrentGame;

	//avoid null ptr
	if (!game)
	{
		DEBUG_PRINT(("Error: no game ptr on new game.\n"));
	}

	//clear board, put default layout
	GameInit(game, (char **) game->pieces);
	GameDefaultLayout(game);

	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_PLAYER_SELECTION;
}

void Handler_GoToLoadGame()
{
	DEBUG_PRINT (("load\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_LOAD;

}

void Handler_Quit()
{
	DEBUG_PRINT (("quit\n"));
	_QuitCurrentWindow = 1;
	_QuitProgram = 1;

}

void Handler_NextPlayerSelect(Control * control, SDL_Event * event )
{
	//avoid null ptr
	if  (!control || !control->extra_data)
	{
		return;
	}

	//get button's color from button's extra data.
	color_t color = *((color_t *) control->extra_data);

	//set the global setting
	Settings_NextPlayer_Set(color);

	//redraw window
	DFSTraverseDraw (_CurrentWindow, _CurrentScreen);


	DEBUG_PRINT (("selected next player %d\n", color));

}


void Handler_UserColorSelect(Control * control, SDL_Event * event )
{
	//avoid null ptr
	if  (!control || !control->extra_data)
	{
		return;
	}

	//get button's color from button's extra data.
	color_t color = *((color_t *) control->extra_data);

	//set the global setting
	Settings_UserColor_Set(color);

	//redraw window
	DFSTraverseDraw (_CurrentWindow, _CurrentScreen);


	DEBUG_PRINT (("selected user color %d\n", color));

}

void Handler_GoToSetBoard()
{
	DEBUG_PRINT (("set board\n"));

	//
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_MAIN_MENU;
	//_NextWindow = GUI_WINDOW_GAME; //TODO pass to set board
}

void Handler_PlayerVsCpu()
{
	DEBUG_PRINT (("player vs. cpu\n"));

	//configure game mode
	Settings_GameMode_Set(GAME_MODE_PLAYER_VS_CPU);

	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_AI_SETTINGS;
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

void Handler_GoToSaveGame()
{
	//TODO implement
	DEBUG_PRINT (("save game\n"));
	_QuitCurrentWindow = 1;
	_NextWindow = GUI_WINDOW_SAVE;	//go to save/load

}

static void Gui_SlotTryLoad(char * file_name)
{

	//find out first if file exists.
	if (!DoesFileExist(file_name))
	{
		//TODO handle error graphically.
	}

	//trying to open with libxml2
	game_state_t * game = _CurrentGame;
	if (!LoadGame(game, file_name))
	{
		//TODO specific libxml2 errors? currently giving only 1 error code for all problems.
		//TODO handle error graphically.
	}
	else
	{
		//reading succeeded

		_QuitCurrentWindow = 1;
		//progress to next window
		_NextWindow = GUI_WINDOW_PLAYER_SELECTION;
	}
}

static void Gui_SlotTrySave(char * file_name)
{

	//check file can be written at location.
	if (!FileCanOpenForWriting(file_name))
	{
		//TODO handle error graphically.
	}

	//file is ok.
	//trying to save it with libxml2
	game_state_t * game = _CurrentGame;
	if (!SaveGame((const game_state_t *) game, file_name))
	{
		//TODO specific libxml2 errors? currently giving only 1 error code for all problems.
		//TODO handle error graphically.
	}
	else
	{
		//save succeeded
		//go back to game.
		_QuitCurrentWindow = 1;
		_NextWindow = GUI_WINDOW_GAME;
	}


}

void Handler_SaveLoadSlotDo(Control * control, SDL_Event * event )
{

	//avoid null ptr
	if  (!control || !control->extra_data)
	{
		return;
	}

	//get slot index from button's extra data.
	int slotIndex = *((int *) control->extra_data);

	//build file name from slot index.
	char file_name[MAX_FILE_NAME_LENGTH];
	sprintf(file_name, SLOTS_XML_FILE_FORMAT, slotIndex);

	//do save or load according to active window type
	if (_NextWindow==GUI_WINDOW_LOAD)
	{
		Gui_SlotTryLoad(file_name);
	}
	else if (_NextWindow==GUI_WINDOW_SAVE)
	{
		Gui_SlotTrySave(file_name);
	}

}

void Handler_BoardPieceClick(Control * control, SDL_Event * event)
{

	//avoid null ptr
	//extra_data may also be null
	if (!control || !control->extra_data)
	{
		DEBUG_PRINT(("Error: clicked a piece with no position\n"));
		return;
	}

	game_state_t * game = _CurrentGame;

	//find if there is a piece
	//retrieve piece from control.
	position_t pos = *((position_t *) control->extra_data);

	DEBUG_PRINT (("clicked position: <%c,%d>\n", pos.x, pos.y));

	//act based on previous selection
	if (_BoardPieceSelection==0)
	{
		//none is currently selected.

		piece_t piece;
		piece.position = pos;
		piece.identity = GetPiece (pos, game);

		//check if invalid piece
		//TODO more cases
		if (piece.identity==EMPTY)
		{
			return;
		}


		//piece is valid, mark as selected
		_BoardPieceSelection = 1;
		_BoardPieceMoveSrc = pos;

		//get pieces moves, highlight them
		ListNode * moves = GetMovesForPiece(game, piece); //free later

		//TODO highlight moves

		ListFreeElements(moves, MoveFree);
	}

	else if (_BoardPieceSelection==1)
	{
		//a piece was previously selected.

		//construct a move, from previous selection to current selection.
		move_t selected_move;

		//TODO promotion window.
		char promotion_identity = 0;

		//get list of moves starting from source (previously selected).
		piece_t piece;
		piece.position = _BoardPieceMoveSrc;
		piece.identity = GetPiece (_BoardPieceMoveSrc, game);
		ListNode * moves = GetMovesForPiece(game, piece); //free later

		//find move inside moves list.
		//find move in allowed moves list, (if found, will update move in argument).
		int valid = FindMoveInList(moves, _BoardPieceMoveSrc, pos,
				promotion_identity, &selected_move, NULL);

		//free moves list
		ListFreeElements(moves, MoveFree);

		//return based on result.
		if (valid)
		{
			DEBUG_PRINT(("legal move\n"));
			//TODO do / return the move.

			//reset selection
			_BoardPieceSelection=2;	//valid move was selected
			_BoardPieceNextMove = selected_move;
		}
		else
		{
			//move is not in list.
			DEBUG_PRINT(("illegal move\n"));
			_BoardPieceSelection=0;
		}




	}

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
	Control * button_LoadGame = ButtonCreate("imgs/load_g.bmp", button_LoadGame_rect, Handler_GoToLoadGame);

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
	//set board, next player.
	//player vs. cpu, player vs. player, cancel


	//next player label
	SDL_Rect * label_NextPlayer_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	label_NextPlayer_rect->x = 100; label_NextPlayer_rect->y = 150;
	label_NextPlayer_rect->w = 20; label_NextPlayer_rect->h = 30;
	Control * label_NextPlayer = LabelCreate("imgs/lbl_next_player.bmp", label_NextPlayer_rect);

	//buttons for white/black
	SDL_Rect * button_White_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_White_rect->x = 450; button_White_rect->y = 150;
	button_White_rect->w = 20; button_White_rect->h = 30;
	Control * button_White = ButtonCreate("imgs/white.bmp", button_White_rect, Handler_NextPlayerSelect);
	//special draw behavior
	button_White->Draw = Draw_NextPlayerButton;
	button_White->extra_data = mymalloc (sizeof(color_t));
	//add the color to the button's extra data
	if (button_White->extra_data)
	{
		*((color_t *) button_White->extra_data) = COLOR_WHITE;
	}
	else
	{
		//TODO handle
	}

	SDL_Rect * button_Black_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Black_rect->x = 550; button_Black_rect->y = 150;
	button_Black_rect->w = 20; button_Black_rect->h = 30;
	Control * button_Black = ButtonCreate("imgs/black.bmp", button_Black_rect, Handler_NextPlayerSelect);
	//special draw behavior
	button_Black->Draw = Draw_NextPlayerButton;
	button_Black->extra_data = mymalloc (sizeof(color_t));
	//add the color to the button's extra data
	if (button_Black->extra_data)
	{
		*((color_t *) button_Black->extra_data) = COLOR_BLACK;
	}
	else
	{
		//TODO handle
	}


	//set board button
	SDL_Rect * button_SetBoard_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_SetBoard_rect->x = 100; button_SetBoard_rect->y = 250;
	button_SetBoard_rect->w = 20; button_SetBoard_rect->h = 30;
	Control * button_SetBoard = ButtonCreate("imgs/btn_set_board.bmp", button_SetBoard_rect, Handler_GoToSetBoard);


	//game mode label
	SDL_Rect * label_GameMode_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	label_GameMode_rect->x = 100; label_GameMode_rect->y = 350;
	label_GameMode_rect->w = 20; label_GameMode_rect->h = 30;
	Control * label_GameMode = LabelCreate("imgs/lbl_game_mode.bmp", label_GameMode_rect);

	//player vs. cpu
	SDL_Rect * button_PlayerVsCpu_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_PlayerVsCpu_rect->x = 450; button_PlayerVsCpu_rect->y = 350;
	button_PlayerVsCpu_rect->w = 20; button_PlayerVsCpu_rect->h = 30;
	Control * button_PlayerVsCpu = ButtonCreate("imgs/Player_vs_Computer.bmp", button_PlayerVsCpu_rect, Handler_PlayerVsCpu);

	//player vs. player
	SDL_Rect * button_PlayerVsPlayer_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_PlayerVsPlayer_rect->x = 450; button_PlayerVsPlayer_rect->y = 420;
	button_PlayerVsPlayer_rect->w = 20; button_PlayerVsPlayer_rect->h = 30;
	Control * button_PlayerVsPlayer = ButtonCreate("imgs/player_vs_player.bmp", button_PlayerVsPlayer_rect, Handler_PlayerVsPlayer);

	//cancel
	SDL_Rect * button_Cancel_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Cancel_rect->x = 20; button_Cancel_rect->y = 470;
	button_Cancel_rect->w = 20; button_Cancel_rect->h = 30;
	Control * button_Cancel = ButtonCreate("imgs/Cancel.bmp", button_Cancel_rect, Handler_Cancel);


	//link the objects to window
	ControlAddChild(window, label_NextPlayer);
	ControlAddChild(window, button_White);
	ControlAddChild(window, button_Black);
	ControlAddChild(window, button_SetBoard);
	ControlAddChild(window, label_GameMode);
	ControlAddChild(window, button_PlayerVsCpu);
	ControlAddChild(window, button_PlayerVsPlayer);
	ControlAddChild(window, button_Cancel);

	return window;
}


Control * Menu_AISettings_Create()
{

	//window (has no rect).
	Control * window = WindowCreate("imgs/background.bmp", NULL);
	//handle error in creation of window.
	if (!window)
	{
		return NULL;
	}

	//buttons under window
	//user color , difficulty.


	//user color label
	SDL_Rect * label_UserColor_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	label_UserColor_rect->x = 100; label_UserColor_rect->y = 150;
	label_UserColor_rect->w = 20; label_UserColor_rect->h = 30;
	Control * label_UserColor = LabelCreate("imgs/lbl_user_color.bmp", label_UserColor_rect);

	//buttons for white/black
	SDL_Rect * button_White_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_White_rect->x = 450; button_White_rect->y = 150;
	button_White_rect->w = 20; button_White_rect->h = 30;
	Control * button_White = ButtonCreate("imgs/white.bmp", button_White_rect, Handler_UserColorSelect);
	//special draw behavior
	button_White->Draw = Draw_UserColorButton;
	button_White->extra_data = mymalloc (sizeof(color_t));
	//add the color to the button's extra data
	if (button_White->extra_data)
	{
		*((color_t *) button_White->extra_data) = COLOR_WHITE;
	}
	else
	{
		//TODO handle
	}

	SDL_Rect * button_Black_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Black_rect->x = 550; button_Black_rect->y = 150;
	button_Black_rect->w = 20; button_Black_rect->h = 30;
	Control * button_Black = ButtonCreate("imgs/black.bmp", button_Black_rect, Handler_UserColorSelect);
	//special draw behavior
	button_Black->Draw = Draw_UserColorButton;
	button_Black->extra_data = mymalloc (sizeof(color_t));
	//add the color to the button's extra data
	if (button_Black->extra_data)
	{
		*((color_t *) button_Black->extra_data) = COLOR_BLACK;
	}
	else
	{
		//TODO handle
	}


	//difficulty label
	SDL_Rect * label_Difficulty_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	label_Difficulty_rect->x = 100; label_Difficulty_rect->y = 350;
	label_Difficulty_rect->w = 20; label_Difficulty_rect->h = 30;
	Control * label_Difficulty = LabelCreate("imgs/lbl_difficulty.bmp", label_Difficulty_rect);


	//link the objects to window
	ControlAddChild(window, label_UserColor);
	ControlAddChild(window, button_White);
	ControlAddChild(window, button_Black);
	ControlAddChild(window, label_Difficulty);


	return window;
}

Control * Menu_SaveLoad_Create()
{

	//window (has no rect).
	Control * window = WindowCreate("imgs/background.bmp", NULL);
	//handle error in creation of window.
	if (!window)
	{
		return NULL;
	}

	//buttons under window
	//TODO load / save label

	//save slots
	int x_off = 300;
	int y_off = 20;
	for (int i=1; i<=NUM_SAVE_SLOTS; i++)
	{

		//build slot button and link it to window.
		SDL_Rect * button_slot_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
		button_slot_rect->x = x_off; button_slot_rect->y = y_off;
		button_slot_rect->w = 0; button_slot_rect->h = 0;

		//build file name from index and format.
		char slot_filename[MAX_FILE_NAME_LENGTH];
		sprintf(slot_filename, SLOTS_IMG_FILE_FORMAT, i);
		Control * button_Slot = ButtonCreate(slot_filename, button_slot_rect, Handler_SaveLoadSlotDo);

		//save on control's extra data - the number of slot.
		button_Slot->extra_data = mymalloc(sizeof(int));
		if (button_Slot->extra_data)
		{
			*((int *) button_Slot->extra_data) = i;
		}

		//link to window
		ControlAddChild(window, button_Slot);

		//progress downwards
		y_off+= 50;
	}

	//cancel button
	//TODO needed here?
	SDL_Rect * button_Cancel_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Cancel_rect->x = 20; button_Cancel_rect->y = 350;
	button_Cancel_rect->w = 20; button_Cancel_rect->h = 30;
	Control * button_Cancel = ButtonCreate("imgs/Cancel.bmp", button_Cancel_rect, Handler_Cancel);

	//link the objects to window
	ControlAddChild(window, button_Cancel);

	return window;
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
	panel_GameOptions_rect->x = 520; panel_GameOptions_rect->y = 0;
	panel_GameOptions_rect->w = 0; panel_GameOptions_rect->h = 0;
	Control * panel_GameOptions = PanelCreate("imgs/background.bmp", panel_GameOptions_rect);

	//buttons inside game options.
	//save game
	SDL_Rect * button_SaveGame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_SaveGame_rect->x = 600; button_SaveGame_rect->y = 0;
	button_SaveGame_rect->w = 0; button_SaveGame_rect->h = 0;
	Control * button_SaveGame = ButtonCreate("imgs/Save_Game.bmp", button_SaveGame_rect, Handler_GoToSaveGame);

	//main menu
	SDL_Rect * button_MainMenu_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_MainMenu_rect->x = 600; button_MainMenu_rect->y = 100;
	button_MainMenu_rect->w = 0; button_MainMenu_rect->h = 0;
	Control * button_MainMenu = ButtonCreate("imgs/Main_Menu.bmp", button_MainMenu_rect, Handler_Cancel);
	//TODO specific handler for main menu?


	//quit program
	SDL_Rect * button_Quit_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
	button_Quit_rect->x = 600; button_Quit_rect->y = 450;
	button_Quit_rect->w = 0; button_Quit_rect->h = 0;
	Control * button_Quit = ButtonCreate("imgs/Exit_Game.bmp", button_Quit_rect, Handler_Quit);


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
	ControlAddChild(panel_GameOptions, button_MainMenu);
	ControlAddChild(panel_GameOptions, button_Quit);

	//get current game.
	game_state_t * game = _CurrentGame;
	if (!game)
	{
		DEBUG_PRINT(("Error: game window with no active."));
		return NULL;
	}

	//build a board based on game.
	BuildBoard(game, panel_GameBoard);
	return window;
}


//build picture filename based on identity and position
//updates filename by reference
static void BoardPieceFindPictureByIdentity(position_t pos, char identity, char * filename)
{

	//find color of square
	int i = pos.x - 'a';
	int j = pos.y;
	color_t square_color = ((i%2==0 && j%2==0)||(i%2!=0 &&j%2!=0)) ? COLOR_BLACK : COLOR_WHITE;
	char * strSquareColor = (square_color==COLOR_BLACK) ? "blck":"wht" ;

	if (identity==EMPTY)
	{
		char * format = "imgs/empty_%s.bmp";
		sprintf(filename, format, strSquareColor);
	}
	else
	{
		//color_identity_bgcolor
		char * format = "imgs/%s_%s_%s.bmp";

		//determine piece's color
		char * strPieceColor = (GetColor(identity)==COLOR_WHITE) ? "wht" : "blck";

		//determine piece identity
		char * strPieceIdentity = NULL;
		piece_t piece;
		piece.identity = identity;
		piece.position = pos;
		if (IsMan(piece))
		{
			strPieceIdentity = "pwn";
		}
		else if (IsBishop(piece))
		{
			strPieceIdentity = "bshp";
		}
		else if (IsKnight(piece))
		{
			strPieceIdentity = "nght";
		}
		else if (IsRook(piece))
		{
			strPieceIdentity = "rk";
		}
		else if (IsQueen(piece))
		{
			strPieceIdentity = "qn";
		}
		else if (IsKing(piece))
		{
			strPieceIdentity = "kng";
		}

		//build filename
		sprintf (filename, format, strPieceColor, strPieceIdentity, strSquareColor);

	}


}

//a function for drawing board piece.
void Draw_PieceBoard (Control * button, SDL_Surface * screen)
{

	//avoid bull ptr on control and it's extra data.
	if (!button || !button->extra_data)
	{
		return;
	}
	//get current game state, find identity inside position.
	game_state_t * game = _CurrentGame;
	//get position of piece
	position_t pos = *((position_t *) button->extra_data);

	//decide what is in the piece
	char identity = GetPiece (pos, game);

	//TODO pre-load images.
	//find the correct image based on the piece and position
	char filename[50];
	BoardPieceFindPictureByIdentity(pos, identity, filename);
	//TODO handle errors

	//load image
	//load the image (if exists) into ctl's surface
	bmp_load(filename, &(button->surface));


	//load image to
	//update clipping information.
	//draw button inside it's parent's boundaries.
	if (button->parent && button->parent->rect)
	{
		SDL_SetClipRect(screen, button->parent->rect);
	}

	/* Blit onto the screen surface */
	//note: width and height are ignored in dstrect!
	if(SDL_BlitSurface(button->surface, NULL, screen, button->rect) < 0)
	{
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	}
	//reset clip
	SDL_SetClipRect (screen, NULL);

	//SDL_UpdateRect(dest, 0, 0, src->w, src->h);
	//flip instead of update
	SDL_Flip(screen);

	/* Free the allocated BMP surface. */
	SDL_FreeSurface(button->surface);

	//the surface was freed, reset pointer.
	button->surface = NULL;
}



//builds board (each square is a control),
//and links it to panel .
static void BuildBoard(game_state_t * game, Control * panel_GameBoard)
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

			//real draw is later.
			Control * button_BoardPiece = ButtonCreate( NULL, button_BoardPiece_rect, Handler_Quit);

			//save position info on button.
			button_BoardPiece->extra_data = mymalloc (sizeof(position_t));
			//TODO check malloc
			*((position_t *) button_BoardPiece->extra_data) = pos;

			//give draw and handle events handlers.
			button_BoardPiece->Draw = Draw_PieceBoard;
			button_BoardPiece->HandleEvents = Handler_BoardPieceClick;

			//progress to next column
			x_off += board_piece_width;

			//link button to panel
			ControlAddChild(panel_GameBoard, button_BoardPiece);
		}

		//finished with row, progress to next row.
		y_off += board_piece_height;

	}
}

void Draw_NextPlayerButton (Control * button, SDL_Surface * screen)
{
	//on null ptrs, no way to draw button.
	if (!button || !button->extra_data)
	{
		return;
	}

	//free previous surface if exists
	if (button->surface)
	{
		SDL_FreeSurface(button->surface);
		button->surface = NULL;
	}

	//get button's color from its extra data,
	//see if color matches
	color_t color = *((color_t *)button->extra_data);
	if (Settings_NextPlayer_Get()==color)
	{
		//button should be selected

		//determine filename based on color.
		char * filename = (color==COLOR_WHITE) ? "imgs/white_selected.bmp": "imgs/black_selected.bmp";
		//switch data on surface to selected version
		bmp_load(filename, &button->surface);
	}
	else
	{
		//button is not selected, load its normal version.
		//determine filename based on color.
		char * filename = (color==COLOR_WHITE) ? "imgs/white.bmp": "imgs/black.bmp";
		//switch data on surface to selected version
		bmp_load(filename, &button->surface);
	}

	//draw normally (will free the surface)
	ButtonDraw(button, screen);

}

//TODO unify with draw next player
void Draw_UserColorButton (Control * button, SDL_Surface * screen)
{
	//on null ptrs, no way to draw button.
	if (!button || !button->extra_data)
	{
		return;
	}

	//free previous surface if exists
	if (button->surface)
	{
		SDL_FreeSurface(button->surface);
		button->surface = NULL;
	}

	//get button's color from its extra data,
	//see if color matches
	color_t color = *((color_t *)button->extra_data);
	if (Settings_UserColor_Get()==color)
	{
		//button should be selected

		//determine filename based on color.
		char * filename = (color==COLOR_WHITE) ? "imgs/white_selected.bmp": "imgs/black_selected.bmp";
		//switch data on surface to selected version
		bmp_load(filename, &button->surface);
	}
	else
	{
		//button is not selected, load its normal version.
		//determine filename based on color.
		char * filename = (color==COLOR_WHITE) ? "imgs/white.bmp": "imgs/black.bmp";
		//switch data on surface to selected version
		bmp_load(filename, &button->surface);
	}

	//draw normally (will free the surface)
	ButtonDraw(button, screen);

}

//handler for game mgr. will update GUI's board.
void Gui_UpdateBoard(game_state_t * game)
{
	//TODO redraw only source and position ?
	//redraw board.
	DFSTraverseDraw(_CurrentWindow, _CurrentScreen);
}

//handler for game mgr.
void Gui_HandleCheck (play_status_t play_status, color_t next_player)
{
	//display a label on right panel saying check.


	if (play_status==STATUS_PLAYER_IN_CHECK)
	{
		char * filename= NULL;
		if (next_player==COLOR_WHITE)
		{
			filename="imgs/wht_plr_chs.bmp";
		}
		else if (next_player==COLOR_BLACK)
		{
			filename="imgs/blck_plr_chs.bmp";
		}

		if (filename)
		{
			//TODO properly, create label
			//add under right panel. no handler.
			SDL_Rect * label_Check_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
			label_Check_rect->x = 310; label_Check_rect->y = 150;
			label_Check_rect->w = 20; label_Check_rect->h = 30;


			//add and draw child.
			Control * label_Check = ButtonCreate(filename, label_Check_rect, NULL );
			ControlAddChild(_CurrentWindow, label_Check);
			label_Check->Draw(label_Check, _CurrentScreen);
		}
	}

	else if (play_status==STATUS_OPPONENT_IN_CHECK)
	{
		//not supposed to happen
	}

}


void Gui_HandleEnd
	(game_state_t * game, play_status_t play_status, color_t next_player)
{

	//check filename to display message
	char * filename = NULL;

	if (play_status==STATUS_TIE)
	{
		filename = "imgs/tie.bmp";
	}
	//if next player is in checkmate - end game.
	else if (play_status==STATUS_PLAYER_IN_CHECKMATE)
	{
		if (next_player==COLOR_WHITE)
		{
			//white lost (black wins)
			filename = "imgs/blck_plr_wns.bmp";
		}
		else if (next_player==COLOR_BLACK)
		{
			//black lost (white wins)
			filename = "imgs/wht_plr_wns.bmp";
		}
	}
	else
	{
		//not supposed to happen
	}

	//there is a message to display
	if (filename)
	{
		//TODO properly, create label
		//add under right panel. no handler.
		SDL_Rect * label_Endgame_rect = (SDL_Rect *) mymalloc(sizeof(SDL_Rect));
		label_Endgame_rect->x = 310; label_Endgame_rect->y = 150;
		label_Endgame_rect->w = 20; label_Endgame_rect->h = 30;

		//add and draw child.
		Control * label_Endgame = ButtonCreate(filename, label_Endgame_rect, NULL );
		ControlAddChild(_CurrentWindow, label_Endgame);
		label_Endgame->Draw(label_Endgame, _CurrentScreen);
	}
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
		case GUI_WINDOW_AI_SETTINGS:
			return Menu_AISettings_Create();
			break;
		case GUI_WINDOW_SAVE:
		case GUI_WINDOW_LOAD:
			return Menu_SaveLoad_Create();	//handles both save/load
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

	_CurrentScreen = screen; //TODO remove

	//a loop that will switch between windows.
	//will maintain a reference to the active window.
	//upon window close, will load the next window.
	while (!_QuitProgram)
	{
		//create the upcoming window
		Control * window = Gui_GetNextWindow(_NextWindow);
		if (!window)
		{
			DEBUG_PRINT(("Error: program continues but no next window."));
		}
		//save window on global (for game manager).
		_CurrentWindow = window;

		//if game window, do separate flow.
		if (_NextWindow==GUI_WINDOW_GAME)
		{
			Gui_Main_Game(game, window, screen);
		}
		else
		{
			//all other windows flow.

			//draw the objects to screen. (with DFS traverse)
			DFSTraverseDraw (window, screen);

			//handle events in loop, until exit.
			HandleEventsLoop(window);

			//loop was done (window exited).
			//clear it (will free all children objects)
			ControlFree((void *) window);
		}
	}

	//done with program, will go back to main.
}


//just a special main function for main
void Gui_Main_Game (game_state_t * game, Control * window, SDL_Surface * screen)
{
	//game window is ready.
	//(board was already built)

	//draw window for the first time
	DFSTraverseDraw(window, screen);

	//start game.
	//game is handled by DoGame function.
	//it will be call GUI for getting user input and displaying game state.
	DoGame(game);

	//still have the controls open
	//TODO disable all handlers.
	Gui_SelectUserMove(game, NULL);

	//game was done .
	//clear it (will free all children objects)
	ControlFree((void *) window);

	//go back to caller.
}


int Gui_SelectUserMove(game_state_t * game, move_t * selected_move)
{

	//handle events in loop, until exit.
	while (_BoardPieceSelection!=2 && !_QuitCurrentWindow)
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
					DFSNotifyRelevantControl (&e, _CurrentWindow);
					break;
				default:
					break;
			}
		}

		/* Wait a little before redrawing*/
		SDL_Delay(200);
	}

	//if clicked on a move + caller asked for a move
	if (_BoardPieceSelection==2 && selected_move)
	{
		//reset move select indication
		_BoardPieceSelection=0;

		//update return argument to selecte move.
		*selected_move = _BoardPieceNextMove;
		return 1;
	}

	//anyway reset indication
	if (_BoardPieceSelection==2)
	{
		//reset move select indication
		_BoardPieceSelection=0;
	}
	//TODO handle error?
	return 0;
}

