#include "Game.h"
#include "Minimax.h"
#include "Draughts.h"

void CPUTurn (game_state_t * game);
void Test_CPUTurn (game_state_t * game);


void Test_GameWithMinimax ()
{
	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];



	GameInit(&game, (char **)board);
	Test_CPUTurn(&game);

	//set some layout.
	//GameDefaultLayout(&game);




}

void Test_CPUTurn (game_state_t * game)
{

	//put a piece in position
	position_t pos;
	char identity;

	pos = Position ('d', 4);
	identity = WHITE_M;
	SetPiece(pos, identity, game);
	piece_t piece1 ;
	piece1.identity = identity;
	piece1.position = pos;

	pos = Position ('c', 5);
	identity = BLACK_M;
	SetPiece(pos, identity, game);
	piece_t piece2 ;
	piece2.identity = identity;
	piece2.position = pos;

	pos = Position ('d', 2);
	identity = WHITE_K;
	SetPiece(pos, identity, game);
	piece_t piece3 ;
	piece3.identity = identity;
	piece3.position = pos;

	//b,2
	pos = Position ('b', 2);
	identity = WHITE_K;
	SetPiece(pos, identity, game);

	PrintBoard(game);


	CPUTurn(game);
}

//one turn of the CPU.
void CPUTurn (game_state_t * game)
{
	//get CPU's color
	color_t color = Settings_CPUColor_Get();

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
	MinimaxChoose (game, color, RootChildren, 0, max_depth,
			DraughtsScoringFunction, GetMovesForPlayer,
			&childIndex, &childScore);

	DEBUG_PRINT( ("index %d was chosen. will lead to score of %d\n", childIndex, childScore));

}


