/*
 * GameTest.c
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include "Game.h"
#include "Draughts.h"


void GameTest ()
{

//initialize game state.
//(empty layout)
game_state_t game;
char board [BOARD_SIZE][BOARD_SIZE];
GameInit(&game, (char **)board);

//GameDefaultLayout(&game);

//print empty board
PrintBoard(&game);

//put something on board.

//do 1 move.

//get pieces

//get list of moves for piece.

//get all moves for side.

}
