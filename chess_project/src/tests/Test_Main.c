/*
 * Test_Main.c
 *
 *  Created on: Oct 3, 2015
 *      Author: nirattar
 */

#include "Test_Minimax.h"
#include "../Draughts.h"

void Test_Main ()
{

	printf(WELCOME_TO_DRAUGHTS);

	//init game rules
	RulesInit();

	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];

	//clear board, put default layout
	GameInit(&game, (char **) board);

	//ListTest ();

	//GameTest ();

	//original_main();

	Test_Minimax();

	//Test_GameWithMinimax();

	//Test_Files();

	memory_print();


}
