/*
 * Test_Main.c
 *
 *  Created on: Oct 3, 2015
 *      Author: nirattar
 */

#include "Test_Minimax.h"
#include "Test_Main.h"

#include "../Game_Mgr.h"
#include "../Memory.h"
#include "Test_Files.h"

//declarations
int ListTest ();
void Test_GameWithMinimax();


void Test_Main ()
{



	//init game rules
	RulesInit();

//	//initialize game state.
//	//(empty layout)
//	game_state_t game;
//	char board [BOARD_SIZE][BOARD_SIZE];
//
//	//clear board, put default layout
//	GameInit(&game, (char **) board);

	//ListTest ();

	//GameTest();

	//original_main();

	//Test_Minimax();

	//Test_GameWithMinimax();

	Test_Files();

	//Test_Gui_framework();

	memory_print();


}

