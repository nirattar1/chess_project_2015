/*
 * Game.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include "Game.h"

position_t Position (int x , int y)
{
	position_t pos;
	pos.x = x;
	pos.y = y;
	return pos;
}


void GameInit (game_state_t * game, char ** board )
{

	//define counts
	game->piecesCount[PLAYER_BLACK]  = 0;
	game->piecesCount[PLAYER_WHITE]  = 0;

	//save pointer to board - assume matrix.
	game->pieces = (board_column *) board;

	//allocate space for board.
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			game->pieces[i][j] = EMPTY;
		}
	}

}


void GameDefaultLayout (game_state_t * game)
{
	//place all the pieces.
	int i,j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if ((i + j) % 2 == 0){
				if (j <= 3){
					game->pieces[i][j] = WHITE_M;
				}
				else if (j >= 6){
					game->pieces[i][j] = BLACK_M;
				}
				else{
					game->pieces[i][j] = EMPTY;
				}
			}
			else{
				game->pieces[i][j] = EMPTY;
			}
		}
	}

	//define counts
	game->piecesCount[PLAYER_BLACK]  = 20;
	game->piecesCount[PLAYER_WHITE]  = 20;

}
