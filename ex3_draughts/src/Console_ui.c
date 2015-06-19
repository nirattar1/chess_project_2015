/*
 * Console_ui.c
 *
 *  Created on: Jun 18, 2015
 *      Author: nirattar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Draughts.h"

void ClearCharBuffer (char * buffer, int buflen)
{
	for (int i=0; i<buflen; i++)
	{
		*((char *) (buffer + i)) = 0;
	}
}


#define MAX_COMMAND_LENGTH 1024

int readline (char * line)
{
	if ( fgets (line , MAX_COMMAND_LENGTH , stdin) == NULL ) {
			printf("No line\n");
			return 0;
	}
	else
	{
		return 1;
	}
}


int Menu_Settings(game_state_t * game, char ** board)
{


	//buffer to store commands
	char line[MAX_COMMAND_LENGTH];
	ClearCharBuffer (line, MAX_COMMAND_LENGTH);

	//print the command to enter settings to the user
	printf(ENTER_SETTINGS);

	//
	int start = 0;
	//while user does'nt enter QUIT or START
	while (1)
	{


		readline(line);


		if (strncmp(line, "minimax_depth", 13)==0)
		{
			//parse and give to command
			char d [2];
			strncpy(d, line+14, 1);
			d[1] = '\0';
			int depth = atoi(d);
			if (depth<1 || depth > 6)
			{
				printf(WRONG_MINIMAX_DEPTH);
			}
			else
			{
				Settings_MaxDepth_Set(depth);
			}
		}


		else if (strncmp(line, "user_color", 10)==0)
		{
			color_t color = DEFAULT_USER_COLOR;
			if (strncmp(line+11, "black", 5)==0)
			{
				color = COLOR_BLACK;
			}
			if (strncmp(line+11, "white", 5)==0)
			{
				color = COLOR_WHITE;
			}
			Settings_UserColor_Set(color);
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
			char identity = WHITE_M;
			if (strstr(line, "white m"))
			{
				identity = WHITE_M;
			}
			else if (strstr(line, "black m"))
			{
				identity = BLACK_M;
			}
			else if (strstr(line, "white k"))
			{
				identity = WHITE_K;
			}
			else if (strstr(line, "black k"))
			{
				identity = BLACK_K;
			}

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

	}

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
	printf(ENTER_YOUR_MOVE);


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
			proposed_move.num_captures = 0;

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



	}

	move_t zero;
	return zero;
}
