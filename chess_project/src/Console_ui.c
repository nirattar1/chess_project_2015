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
	if (0==strcmp(line, "white pawn"))
	{
		return WHITE_M;
	}
	if (0==strcmp(line, "white bishop"))
	{
		return WHITE_B;
	}
	if (0==strcmp(line, "white knight"))
	{
		return WHITE_N;
	}
	if (0==strcmp(line, "white rook"))
	{
		return WHITE_R;
	}
	if (0==strcmp(line, "white queen"))
	{
		return WHITE_Q;
	}
	if (0==strcmp(line, "white king"))
	{
		return WHITE_K;
	}

	if (0==strcmp(line, "black pawn"))
	{
		return BLACK_M;
	}
	if (0==strcmp(line, "black bishop"))
	{
		return BLACK_B;
	}
	if (0==strcmp(line, "black knight"))
	{
		return BLACK_N;
	}
	if (0==strcmp(line, "black rook"))
	{
		return BLACK_R;
	}
	if (0==strcmp(line, "black queen"))
	{
		return BLACK_Q;
	}
	if (0==strcmp(line, "black king"))
	{
		return BLACK_K;
	}

	return 0;
}


static void Menu_Settings_PrintError(user_command_errorcode_t cmd_status)
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
		case SETTING_COMMAND_STATUS_WRONG_POSITION:
			printf (WRONG_POSITION);
			break;
		case SETTING_COMMAND_STATUS_BAD_PIECE_SET:
			printf (NO_PIECE);
			break;
		case SETTING_COMMAND_STATUS_WRONG_BOARD_INITIALIZATION:
			printf (WRONG_BOARD_INITIALIZATION);
			break;
		case SETTING_COMMAND_STATUS_PIECE_NOT_OF_PLAYER:
			printf (PIECE_NOT_OF_PLAYER);
			break;
		case SETTING_COMMAND_STATUS_ILLEGAL_COMMAND:
			printf (ILLEGAL_COMMAND);
			break;
		case SETTING_COMMAND_STATUS_ILLEGAL_MOVE_FOR_PLAYER:
			printf (ILLEGAL_MOVE);
			break;
		default:
			printf (ILLEGAL_COMMAND);
			break;
	}


}
//prompt user and set game mode
//possible values - 1, 2 .
static user_command_errorcode_t Menu_ReadSetting_GameMode
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

static user_command_errorcode_t Menu_ReadSetting_Difficulty(char * line, int start_at_char, game_state_t * game)
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

static user_command_errorcode_t Menu_ReadSetting_UserColor (char * line, int start_at_char, game_state_t * game)
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
static user_command_errorcode_t Menu_ReadSetting_LoadGame (char * line, int start_at_char, game_state_t * game)
{

	//get file name from line and store it.
	//file name can be as long as maximum command length
	char file_name [MAX_FILE_NAME_LENGTH];
	strncpy(file_name, line+start_at_char, MAX_FILE_NAME_LENGTH);

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


static user_command_errorcode_t Menu_ReadSetting_NextPlayer (char * line, int start_at_char, game_state_t * game)
{

	//read black or white
	if (strncmp(line+start_at_char, "black", 5)==0)
	{
		Settings_NextPlayer_Set(COLOR_BLACK);
		DEBUG_PRINT(("next player is %d\n", Settings_NextPlayer_Get()));
		return SETTING_COMMAND_STATUS_OK;
	}
	else if (strncmp(line+start_at_char, "white", 5)==0)
	{
		Settings_NextPlayer_Set(COLOR_WHITE);
		DEBUG_PRINT(("next player is %d\n", Settings_NextPlayer_Get()));
		return SETTING_COMMAND_STATUS_OK;
	}

	return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
}

static user_command_errorcode_t Menu_ReadSetting_RemovePiece (char * line, int start_at_char, game_state_t * game)
{
	//get x and y value of position -
	//assuming always 1 digit (board size less than 10)
	char x;
	int y;
	sscanf (line+start_at_char, "<%c,%d>", &x, &y);

	//build new position from x and y
	position_t pos = Position(x, y);

	//check bounds
	if (PositionInBounds(pos))
	{
		//update game
		SetPiece(pos, EMPTY, game);
		return SETTING_COMMAND_STATUS_OK;
	}
	else
	{
		return SETTING_COMMAND_STATUS_WRONG_POSITION;
	}

}


static user_command_errorcode_t Menu_ReadSetting_SetPiece (char * line, int start_at_char, game_state_t * game)
{
	//get x and y value of position -
	//assuming always 1 digit (board size less than 10)
	char x;
	int y;
	sscanf (line+start_at_char, "<%c,%d>", &x, &y);

	//build new position from x and y
	position_t pos = Position(x, y);

	//check position in bounds
	if (!PositionInBounds(pos))
	{
		return SETTING_COMMAND_STATUS_WRONG_POSITION;
	}

	//move on to read identity: <h,1>_ (skip position and space)
	start_at_char+=6;
	char identity = GetIdentityByString (line+start_at_char);

	//check validity of piece type
	if (identity==0)
	{
		return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
	}

	//check if adding the piece is valid.
	if (!IsValidPieceAddition(game, pos, identity))
	{
		return SETTING_COMMAND_STATUS_BAD_PIECE_SET;
	}


	//all is valid, upadte board.
	SetPiece(pos, identity, game);
	return SETTING_COMMAND_STATUS_OK;

}

//will read moves from the user.
//if legal,
static user_command_errorcode_t Menu_ReadCommand_Move
(char * line, int start_at_char, game_state_t * game, color_t current_player, move_t * selected_move)
{

	//avoid null
	if (!selected_move)
	{
		DEBUG_PRINT (("Null pointer on move.\n"));
		return SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
	}

	//determine source and destination positions from user line.
	//assuming always 1 digit (board size less than 10)
	char 	src_x, dest_x;
	int 	src_y, dest_y;
	sscanf (line+start_at_char, "<%c,%d> to <%c,%d>",
			&src_x, &src_y, &dest_x, &dest_y);

	//build positions.
	position_t src 	= Position(src_x, src_y);
	position_t dest = Position(dest_x, dest_y);

	//check both positions are in bounds
	if (!PositionInBounds(src) || !PositionInBounds(dest))
	{
		return SETTING_COMMAND_STATUS_WRONG_POSITION;
	}

	//check that position contains user's piece
	if (GetColor(GetPiece(src, game)) != current_player)
	{
		return SETTING_COMMAND_STATUS_PIECE_NOT_OF_PLAYER;
	}


	//move on to read promotion identity (if exists)
	start_at_char += 15; // skip this pattern length: <a,1>_to_<b,2>_
	//TODO just the type not color
	char promotion_identity = GetIdentityByString (line+start_at_char);

	//check validity of piece type
	if (promotion_identity==0)
	{
		//
	}

	//check that move is indeed in user's allowed moves list.

	//get moves list.
	ListNode * moves = GetMovesForPlayer(game, current_player); //always free later

	//find move in allowed moves list, (if found, will update move in argument).
	int valid = FindMoveInList(moves, src, dest, promotion_identity, selected_move);

	//free moves list
	ListFreeElements(moves, MoveFree);

	//return based on result.
	if (valid)
	{
		return SETTING_COMMAND_STATUS_OK;
	}
	else
	{
		//move is not in list.
		return SETTING_COMMAND_STATUS_ILLEGAL_MOVE_FOR_PLAYER;
	}

}

//prints the moves list of a given piece.
//the piece location is given at char buffer.
static user_command_errorcode_t Menu_ReadCommand_GetMovesPiece
(char * line, int start_at_char, game_state_t * game, color_t current_player)
{
	//determine source position from user line.
	//assuming always 1 digit (board size less than 10)
	char 	src_x;
	int 	src_y;
	sscanf (line+start_at_char, "<%c,%d>",	&src_x, &src_y);

	//build position.
	position_t src 	= Position(src_x, src_y);

	//check position is in bounds
	if (!PositionInBounds(src))
	{
		return SETTING_COMMAND_STATUS_WRONG_POSITION;
	}

	//check that position contains user's piece
	char identity = GetPiece(src, game);
	if (GetColor(identity) != current_player)
	{
		return SETTING_COMMAND_STATUS_PIECE_NOT_OF_PLAYER;
	}

	//piece is ok.
	//build a list of moves and print it.
	piece_t piece;
	piece.identity = identity;
	piece.position = src;
	ListNode * moves = GetMovesForPiece(game, piece); //always free later

	if (moves)	//can be null list.
	{
		//print moves.
		MovesListPrint(moves);

		//free moves list
		ListFreeElements(moves, MoveFree);
	}


	return SETTING_COMMAND_STATUS_OK;
}

//save a game.
static user_command_errorcode_t Menu_ReadCommand_SaveGame
(char * line, int start_at_char, game_state_t * game)
{

	//get file name from line and store it.
	//file name can be as long as maximum command length
	char file_name [MAX_FILE_NAME_LENGTH];
	strncpy(file_name, line+start_at_char, MAX_FILE_NAME_LENGTH);
	//TODO possible buffer error? also on load.

	//check file can be written at location.
	if (!FileCanOpenForWriting(file_name))
	{
		return SETTING_COMMAND_STATUS_WRONG_FILE_NAME;
	}

	//file is ok.
	//trying to save it with libxml2
	if (!SaveGame((const game_state_t *) game, file_name))
	{
		//TODO specific libxml2 errors? currently giving only 1 error code for all problems.
		return SETTING_COMMAND_STATUS_WRONG_FILE_NAME;
	}
	else
	{
		//save succeeded
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


	int start = 0;
	//while user does'nt enter QUIT or START
	while (1)
	{

		//prompt user to enter input
		printf(ENTER_SETTINGS);

		readline(line);

		user_command_errorcode_t cmd_status = SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;
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

		//clear
		else if (strncmp(line, "clear", 5)==0)
		{
			GameInit(game, board);
			cmd_status = SETTING_COMMAND_STATUS_OK;
		}

		//next player
		else if (strncmp(line, "next_player", 11)==0)
		{
			cmd_status = Menu_ReadSetting_NextPlayer(line, 12, game);
		}

		//rm
		//format:rm <x,y>
		else if(strncmp(line, "rm", 2)==0)
		{
			cmd_status = Menu_ReadSetting_RemovePiece(line, 3, game);
		}

		//set
		//format: set <x,y> a b
		else if(strncmp(line, "set", 3)==0)
		{
			cmd_status = Menu_ReadSetting_SetPiece(line, 4, game);

		}

		//print
		else if (strncmp(line, "print", 5)==0)
		{
			PrintBoard(game);
			cmd_status = SETTING_COMMAND_STATUS_OK;
		}

		//start
		else if (strncmp(line, "start", 5)==0)
		{
			//check that board has king of each side.
			if (IsValidBoard(game)==BOARD_VALIDITY_KING_IS_MISSING)
			{
				cmd_status = SETTING_COMMAND_STATUS_WRONG_BOARD_INITIALIZATION;
			}
			else
			{
				//board is ok, can start game
				start = 1;
				cmd_status = SETTING_COMMAND_STATUS_OK;
				break;
			}
		}

		else if (strncmp(line, "quit", 4)==0)
		{
			cmd_status = SETTING_COMMAND_STATUS_OK;
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
		//should not reach here.
		exit (1);
	}


	//game has ended.
	return 0;


}


//selected_move- return argument, will be updated to user's selection, if legal.
int Menu_PlayUser(game_state_t * game, move_t * selected_move)
{

	//buffer to store commands
	char line[MAX_COMMAND_LENGTH];
	ClearCharBuffer (line, MAX_COMMAND_LENGTH);

	//determine player
	color_t current_player = Settings_NextPlayer_Get();

	//repeat until valid move had been chosen, or chose to quit.
	while (1)
	{
		//status of current command
		user_command_errorcode_t cmd_status = SETTING_COMMAND_STATUS_ILLEGAL_COMMAND;

		//print prompt message based on player.
		if (current_player==COLOR_WHITE)
		{
			printf("White player - enter your move:\n");
		}
		else if (current_player==COLOR_BLACK)
		{
			printf("Black player - enter your move:\n");
		}

		//read into buffer
		readline(line);

		//move command
		if(strncmp(line, "move", 4)==0)
		{
			cmd_status = Menu_ReadCommand_Move(line, 5, game, current_player, selected_move);
			//if legal move, we want to return success.
			if (cmd_status==SETTING_COMMAND_STATUS_OK)
			{
				return 1;
			}
			//failure handled later.
		}

		//get moves command.
		//format: get_moves <x,y>
		else if (strncmp(line, "get_moves", 9)==0)
		{
			cmd_status = Menu_ReadCommand_GetMovesPiece(line, 10, game, current_player);
		}

		//TODO get_best_moves d

		//TODO get_score d m

		//save
		//format: save filename
		else if (strncmp(line, "save", 4)==0)
		{
			cmd_status = Menu_ReadCommand_SaveGame(line, 5, game);
		}

		//quit
		else if (strncmp(line, "quit", 4)==0)
		{
			cmd_status = SETTING_COMMAND_STATUS_OK;
			exit (1);
		}

		//handle unsuccessful commands.
		//(print the appropriate error)
		if (cmd_status!=SETTING_COMMAND_STATUS_OK)
		{
			Menu_Settings_PrintError(cmd_status);
		}

	}

	//not supposed to reach here
	return 0;
}




void ConsoleUI_GameHandleEnd
	(game_state_t * game, play_status_t play_status, color_t next_player)
{
	if (play_status==STATUS_TIE)
	{
		printf(TIE);
	}
	//if next player is in checkmate - end game.
	else if (play_status==STATUS_PLAYER_IN_CHECKMATE)
	{
		if (next_player==COLOR_WHITE)
		{
			//white lost (black wins)
			printf("Mate! Black player wins the game\n");
		}
		else if (next_player==COLOR_BLACK)
		{
			//black lost (white wins)
			printf("Mate! White player wins the game\n");
		}
	}
	else
	{
		//not supposed to happen
	}
}

void ConsoleUI_GameHandleCheck (play_status_t play_status, color_t next_player)
{
	if (play_status==STATUS_PLAYER_IN_CHECK)
	{
		printf("Check!\n");
	}
	else if (play_status==STATUS_OPPONENT_IN_CHECK)
	{
		//not supposed to happen
	}
}

