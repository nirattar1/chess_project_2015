/*
 * GameTest.c
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include "Game.h"
#include "Draughts.h"

void BasicTest (game_state_t * game);
void MoveTest(game_state_t * game);
void MoveListTest(game_state_t * game);

void GameTest ()
{
	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];


	//GameDefaultLayout(&game);

	GameInit(&game, (char **)board);
	BasicTest(&game);

	GameInit(&game, (char **)board);
	MoveTest(&game);

	GameInit(&game, (char **)board);
	MoveListTest(&game);
}


void BasicTest (game_state_t * game)
{


	//print empty board
	PrintBoard(game);

	//position
	// (h, 2)
	position_t pos = Position ('h', 2);

	//put something on board.
	SetPiece(pos, WHITE_M, game);
	PrintBoard(game);



	//get piece allowed directions.
	direction_t * dirs = GetPieceDirections(WHITE_K);
	while (*dirs != 0)
	{
		printf ("allowed direction : %d \n", *dirs);
		dirs++;

	}



}

void MoveTest(game_state_t * game)
{



	//position
	// (h, 2)
	position_t pos = Position ('h', 2);
	//put something on board.
	SetPiece(pos, WHITE_M, game);
	PrintBoard(game);


	//generate move
	move_t * move1 = (move_t *) mymalloc(sizeof (move_t));
	move1->src = pos;
	move1->num_captures = 0;
	position_t dest[1];
	dest[0] = Position('g',3);
	move1->dest = dest;


	//do 1 move.
	DoMove(move1, game);
	PrintBoard(game);


	myfree(move1);

}


#define NUM_TEST_MOVES 3
void MoveListTest(game_state_t * game)
{
	//do list of moves

	//set 1 piece.
	position_t pos = Position('b', 4);
	SetPiece (pos, BLACK_M, game);
	PrintBoard(game);

	move_t * moves [NUM_TEST_MOVES];

	//create couple of simple moves (successive)
	moves[0] = (move_t *) mymalloc(sizeof (move_t));
	moves[0]->src = pos;
	moves[0]->num_captures = 0;
	position_t * dest_a = (position_t *) mymalloc(sizeof (position_t));
	dest_a[0] = Position('a',5);
	moves[0]->dest = dest_a;

	moves[1] = (move_t *) mymalloc(sizeof (move_t));
	moves[1]->src = moves[0]->dest[0];
	moves[1]->num_captures = 0;
	position_t * dest_b = (position_t *) mymalloc(sizeof (position_t));
	dest_b[0] = Position('b',6);
	moves[1]->dest = dest_b;

	moves[2] = (move_t *) mymalloc(sizeof (move_t));
	moves[2]->src = moves[1]->dest[0];
	moves[2]->num_captures = 0;
	position_t * dest_c = (position_t *) mymalloc(sizeof (position_t));
	dest_c[0] = Position('c',7);
	moves[2]->dest = dest_c;


	//prepare a list for the moves.

	ListNode * list = NULL;
	//loop on array and add to list
	for (int i=0; i<NUM_TEST_MOVES; i++)
	{
		//point to existing data
		int * pMove = moves[i];

		//create new node
		ListNode * newnode = ListCreateNode( (void *) pMove , sizeof (move_t));

		if (!list)
		{
			//list is empty- make node head of list.
			list = newnode;
		}
		else
		{
			//list is not empty- concat.
			ListNode ** listp = &list;
			ListConcat( listp, newnode);
			list = *listp;
		}
	}

	MovesListPerform( list, game);

	ListFreeElements(list, MoveFree);

}


//perform the moves one after the other
void MovesListPerform( LINK head , game_state_t * game)
{
	for ( ; head !=0; head = head->next )
	{
		//perform move
		DoMove((move_t *) head->data, game);
		//print after move
		PrintBoard(game);
	}
}



//get pieces

//get list of moves for piece.


//get all moves for side.
