#ifndef DRAUGHTS_
#define DRAUGHTS_

//debug
#define DEBUG

#ifdef DEBUG
	#define IS_DEBUG 1
#else
	#define IS_DEBUG 0
#endif

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"


//main class.
//will also include the settings.



//settings + defaults
extern color_t SETTINGS_USER_COLOR;
extern int SETTINGS_MAX_DEPTH;

#define DEFAULT_USER_COLOR COLOR_WHITE
#define DEFAULT_MAX_DEPTH 1

//

#define WELCOME_TO_DRAUGHTS "Welcome to Draughts!\n"
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 6\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
 
#define ENTER_YOUR_MOVE "Enter your move:\n" 
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));

//get color of user/CPU.
color_t Settings_UserColor_Get();
color_t Settings_CPUColor_Get();
//set color of user
void Settings_UserColor_Set(color_t color);

//get/set max depth for minimax.
int Settings_MaxDepth_Get();
void Settings_MaxDepth_Set(int max_depth);

//prints the board
void PrintBoard(game_state_t * game);

//one turn of the CPU.
void CPUTurn (game_state_t * game);

//one turn of the user.
void UserTurn (game_state_t * game);

//manages game
void DoGame(game_state_t * game);

//print a list of moves in good format
void MovesListPrint( LINK head);


#endif  
