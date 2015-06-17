#include "Game.h"
#include "Minimax.h"


void CPUTurn (game_state_t * game);

void Test_GameWithMinimax ()
{
	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];



	GameInit(&game, (char **)board);


	//set some layout.
	//GameDefaultLayout(&game);


	CPUTurn(&game);

}

//one turn of the CPU.
void CPUTurn (game_state_t * game)
{
	//get CPU's color
	color_t color = GetCPUColor();

	//get allowed moves for CPU.
	ListNode * movesCPU = GetMovesForPlayer(game, color);

	//choose the next move based on minimax:

	//get max depth based on settings
	int max_depth = Settings_MaxDepth_Get();

	//get initial children list from given state.

	ListNode * RootChildren = GetMovesForPlayer(game, color);

	//child with best score will decide what move to do .
	int childIndex;
	int childScore;
	MinimaxChoose (game, RootChildren, 0, max_depth,
			DraughtsScoringFunction, GetMovesForPlayer,
			&childIndex, &childScore);

	printf ("index %d was chosen. will lead to score of %d\n", childIndex, childScore);

}

}
