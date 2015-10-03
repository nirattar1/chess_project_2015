#include "../Chess.h"
#include "../Game.h"
#include "../Minimax.h"

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



