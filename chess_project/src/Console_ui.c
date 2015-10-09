/*
 * Console_ui.c
 *
 *  Created on: Jun 18, 2015
 *      Author: nirattar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Files.h"
#include "Game_Mgr.h"

void ClearCharBuffer (char * buffer, int buflen)
{
	for (int i=0; i<buflen; i++)
	{
		*((char *) (buffer + i)) = 0;
	}
}


#define MAX_COMMAND_LENGTH 51

int readline (char * line)
{
	if ( fgets (line , MAX_COMMAND_LENGTH , stdin) == NULL )
	{
			printf("No line\n");
			return 0;
	}
	else
	{
		//there is line

		//clean the last \n
		char * new_line = strchr(line, '\n');
		//replace newline, if found, with null char.
		if (new_line!=NULL)
		{
			*new_line = 0;
		}
		return 1;
	}
}

char GetIdentityByString (char * line)
{
	if (strstr(line, "white pawn"))
	{
		return WHITE_M;
	}
	if (strstr(line, "white bishop"))
	{
		return WHITE_B;
	}
	if (strstr(line, "white knight"))
	{
		return WHITE_N;
	}
	if (strstr(line, "white rook"))
	{
		return WHITE_R;
	}
	if (strstr(line, "white queen"))
	{
		return WHITE_Q;
	}
	if (strstr(line, "white king"))
	{
		return WHITE_K;
	}

	if (strstr(line, "black pawn"))
	{
		return BLACK_M;
	}
	if (strstr(line, "black bishop"))
	{
		return BLACK_B;
	}
	if (strstr(line, "black knight"))
	{
		return BLACK_N;
	}
	if (strstr(line, "black rook"))
	{
		return BLACK_R;
	}
	if (strstr(line, "black queen"))
	{
		return BLACK_Q;
	}
	if (strstr(line, "black king"))
	{
		return BLACK_K;
	}

	return 0;
}


static void Menu_Settings_PrintError(settings_command_error_t cmd_status)
{
	switch (cmd_status)
	{
		case SETTING_COMMAND_STATUS_WRONG_GAME_MODE:
			printf(WRONG_GAME_MODE);
			break;
		case SETTING_COMMAND_STATUS_WRONG_MINIMAX_DEPTH:
			printf(WRONG_MINIMAX_DEPTH);
			break;
		case SETTING_COMMAND_STATUS_WRONG_FILE_NAME:
			printf (WRONG_FILE_NAME);
			break;
		case SETTING_COMMAND_STATUS_ILLEGAL_COMMAND:
			printf (ILLEGAL_COMMAND);
			break;
		default:
			printf (ILLEGAL_COMMAND);
			break;
	}


}
//prompt user and set game mode
//possible values - 1, 2 .
static settings_command_error_t Menu_ReadSetting_GameMode
	(char * line, int start_at_char, game_state_t * game)
{

	//parse and process
	char d [2];
	strncpy(d, line+start_at_char, 1);
	d[1] = '\0';

	int mode = atoi(d);
	if (mode!=GAME_MODE_PLAYER_VS_PLAYER && mode!=GAME_MODE_PLAYER_VS_CPU)
	{
		return SETTING_COMMAND_STATUS_WRONG_GAME_MODE;
	}
	else
	{
		//success, set and print
		Settings_GameMode_Set(mode);
		if (mode == GAME_MODE_PLAYER_VS_PLAYER)
		{
			printf(TWO_PLAYERS_GAME_MODE);
		}
		else if (mode == GAME_MODE_PLAYER_VS_CPU)
		{
			printf(PLAYER_VS_AI_GAME_MODE);
		}
		return SETTING_COMMAND_STATUS_OK;
	}

}

static settings_command_error_t Menu_ReadSetting_Difficulty(char * line, int start_at_char, game_state_t * game)
{
	//check that game mode is indeed against CPU. otherwise we return as illegal command.
	if (Settings_GameMode_Get() != GAME_MODE_PLAYER_VS_CPU)
	{
		return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
	}

	if (strncmp(line+start_at_char, "best", 4)==0)
	{
		//user defined "best" difficulty.
		Settings_MaxDepth_Set(MAX_DEPTH_BEST_VALUE);
		return SETTING_COMMAND_STATUS_OK;
	}

	else if (strncmp(line+start_at_char, "depth", 5)==0)
	{
		//user defined a constant depth

		//parse and process
		char d [2];
		strncpy(d, line+start_at_char+6, 1);
		d[1] = '\0';
		int depth = atoi(d);
		if (depth<MAX_DEPTH_MIN_VALUE || depth > MAX_DEPTH_MAX_VALUE)
		{
			return SETTING_COMMAND_STATUS_WRONG_MINIMAX_DEPTH;
		}
		else
		{
			//valid depth, set it.
			Settings_MaxDepth_Set(depth);
			return SETTING_COMMAND_STATUS_OK;
		}
	}


	//if reached here it is an error.
	return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;

}

static settings_command_error_t Menu_ReadSetting_UserColor (char * line, int start_at_char, game_state_t * game)
{

	//check that game mode is indeed against CPU. otherwise we return as illegal command.
	if (Settings_GameMode_Get() != GAME_MODE_PLAYER_VS_CPU)
	{
		return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
	}

	//read black or white
	if (strncmp(line+start_at_char, "black", 5)==0)
	{
		Settings_UserColor_Set(COLOR_BLACK);
		DEBUG_PRINT(("user color is %d\n", Settings_UserColor_Get()));
		return SETTING_COMMAND_STATUS_OK;
	}
	else if (strncmp(line+start_at_char, "white", 5)==0)
	{
		Settings_UserColor_Set(COLOR_WHITE);
		DEBUG_PRINT(("user color is %d\n", Settings_UserColor_Get()));
		return SETTING_COMMAND_STATUS_OK;
	}

	return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
}


//loads the game
static settings_command_error_t Menu_ReadSetting_LoadGame (char * line, int start_at_char, game_state_t * game)
{

	//get file name from line and store it.
	//file name can be as long as maximum command length
	char file_name [MAX_COMMAND_LENGTH];
	strncpy(file_name, line+start_at_char, MAX_COMMAND_LENGTH);

	//find out first if file exists.
	if (!DoesFileExist(file_name))
	{
		return SETTING_COMMAND_STATUS_WRONG_FILE_NAME;
	}

	//trying to open with libxml2
	if (!LoadGame(game, file_name))
	{
		//TODO specific libxml2 errors? currently giving only 1 error code for all problems.
		return SETTING_COMMAND_STATUS_WRONG_FILE_NAME;
	}
	else
	{
		//reading succeeded
		PrintBoard(game);
		return SETTING_COMMAND_STATUS_OK;
	}

}

int Menu_Settings(game_state_t * game, char ** board)
{

	//print the board
	PrintBoard(game);

	//buffer to store commands
	char line[MAX_COMMAND_LENGTH];
	ClearCharBuffer (line, MAX_COMMAND_LENGTH);

	//
	int start = 0;
	//while user does'nt enter QUIT or START
	while (1)
	{

		//prompt user to enter input
		printf(ENTER_SETTINGS);

		readline(line);

		settings_command_error_t cmd_status = SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
		//game mode
		if (strncmp(line, "game_mode", 9)==0)
		{
			//TODO move print error to print function
			cmd_status = Menu_ReadSetting_GameMode(line, 10, game);
		}

		else if (strncmp(line, "difficulty", 10)==0)
		{
			//TODO move print error to print function
			cmd_status = Menu_ReadSetting_Difficulty(line, 11, game);
		}

		else if (strncmp(line, "user_color", 10)==0)
		{
			cmd_status = Menu_ReadSetting_UserColor(line, 11, game);
		}

		//load game
		else if (strncmp(line, "load", 4)==0)
		{
			cmd_status = Menu_ReadSetting_LoadGame(line, 5, game);
		}


		//rm
		//format:rm <x,y>
		else if(strncmp(line, "rm <", 4)==0)
		{
			//get position
			char x ;
			strncpy(&x, line+4, 1);
			//get 'y' value of position - either '10' (2 digits) or 1 digit
			int yInt;
			if (strncmp(line+6, "10", 2)==0)
			{
				yInt = 10;
			}
			else
			{
				char y[2] ;
				strncpy(y, line+6, 1);
				y[1] = '\0';
				yInt = atoi(y);
			}
			position_t pos;
			pos.x = x;
			pos.y = yInt;

			if (PositionInBounds(pos))
			{
				SetPiece(pos, EMPTY, game);
			}
			else
			{
				printf(WRONG_POSITION);
			}
		}


		//set
		//format: set <x,y> a b
		else if(strncmp(line, "set <", 5)==0)
		{
			//get position
			char x ;
			strncpy(&x, line+5, 1);
			//get 'y' value of position - either '10' (2 digits) or 1 digit
			int yInt;
			if (strncmp(line+7, "10", 2)==0)
			{
				yInt = 10;
			}
			else
			{
				char y[2] ;
				strncpy(y, line+7, 1);
				y[1] = '\0';
				yInt = atoi(y);
			}
			position_t pos;
			pos.x = x;
			pos.y = yInt;

			//find identity - by substring
			char identity = GetIdentityByString (line);

			if (PositionInBounds(pos))
			{
				SetPiece(pos, identity, game);
			}
			else
			{
				printf(WRONG_POSITION);
			}
		}

		//clear
		else if (strncmp(line, "clear", 5)==0)
		{
			GameInit(game, board);
		}

		//print
		else if (strncmp(line, "print", 5)==0)
		{
			PrintBoard(game);
		}

		//start
		else if (strncmp(line, "start", 5)==0)
		{
			start = 1;
			break;
		}

		else if (strncmp(line, "quit", 4)==0)
		{
			exit (1);
		}

		//handle unsuccessful commands.
		//(print the appropriate error)
		if (cmd_status!=SETTING_COMMAND_STATUS_OK)
		{
			Menu_Settings_PrintError(cmd_status);
		}

	}

	//after settings loop, either start game or quit.
	if (start)
	{
		DoGame (game);

	}
	else
	{
		exit (0);
	}


	return 1;


}



move_t Menu_PlayUser(game_state_t * game)
{

	//buffer to store commands
	char line[MAX_COMMAND_LENGTH];
	ClearCharBuffer (line, MAX_COMMAND_LENGTH);

	//print the command to enter settings to the user
	printf("XXXXXX- enter your move !!\n");


	//get the list of user's moves.
	ListNode * moves = GetMovesForPlayer(game, Settings_UserColor_Get());

	int valid = 0;
	//repeat until valid turn
	while (!valid)
	{
		//read into buffer
		readline(line);

		//if asked for "move" command, see that exists.
		//format - move <x,y> to <i,j>[<k,l>...]
		if(strncmp(line, "move <", 6)==0)
		{

			move_t proposed_move;
			//TODO always validate fields.
			proposed_move.num_captures = 0;
			proposed_move.promote_to_identity = 0;

			//source
			char x ;
			strncpy(&x, line+6, 1);
			char y[2] ;
			strncpy(y, line+8, 1);
			y[1] = '\0';
			int yInt = atoi(y);

			position_t src;
			src.x = x;
			src.y = yInt;
			proposed_move.src = src;

			//destinations.
			//15,17
			strncpy(&x, line+15, 1);
			strncpy(y, line+17, 1);
			y[1] = '\0';
			yInt = atoi(y);
			position_t dest;
			dest.x = x;
			dest.y = yInt;

			//mark if valid move .
			proposed_move.dest[0] = dest;

			//find the move in allowed moves list (will update it's captures)
			if (FindMoveInList(moves, &proposed_move))
			{
				valid = 1;
				return proposed_move;
			}
			else
			{
				//print
				printf(ILLEGAL_MOVE);
			}

		}

		//if asked for get moves, print the moves.
		else if (strncmp(line, "get_moves", 9)==0)
		{
			MovesListPrint(moves);
		}

		else if (strncmp(line, "quit", 4)==0)
		{
			exit (1);
		}


	}

	move_t zero;
	return zero;
}
