/*
 * Game.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include "Game.h"
#include <stdlib.h>


direction_t * all_allowed_directions [MAX_IDENTITIES];


direction_t allowed_directions_whitem [3] = {UP_LEFT, UP_RIGHT, 0};
direction_t allowed_directions_whitek [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_blackm [3] = {DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_blackk [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};


void RulesInit ()
{
	all_allowed_directions [WHITE_M] = allowed_directions_whitem;
	all_allowed_directions [WHITE_K] = allowed_directions_whitek;
	all_allowed_directions [BLACK_M] = allowed_directions_blackm;
	all_allowed_directions [BLACK_K] = allowed_directions_blackk;
}


position_t Position (char x , int y)
{
	position_t pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

//translate between position and row/column numbers.
int GetMatrixRow (position_t pos)
{
	return (pos.y - 1);
}

int GetMatrixCol (position_t pos)
{
	//distance from the letter 'a'
	//'a' - 'a' = 0
	//'c' - 'a' = 2
	return (pos.x - 'a');
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

//
void SetPiece (position_t pos, char identity, game_state_t * game )
{
	game->pieces[GetMatrixCol(pos)][GetMatrixRow(pos)] = identity;
}

char GetPiece (position_t pos, game_state_t * game)
{
	return game->pieces[GetMatrixCol(pos)][GetMatrixRow(pos)];
}


//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity)
{
	return all_allowed_directions [identity];
}


ListNode * GetMovesForPieceDefault (game_state_t * game, piece_t piece)
{
	GetMovesForPiece(game, piece, 0, NULL);
}

//get allowed moves for piece on board.
//returns pointer to a list of moves.
//caller responsible to free the list.
ListNode * GetMovesForPiece (game_state_t * game, piece_t piece,
		int is_successive_captures, position_t * dest_history)
{

	//prepate list .
	ListNode * list = NULL;


	//get possible directions.
	direction_t * directions = GetPieceDirections(piece.identity);

	//generate moves.
	//for each direction
	int max_distance = 1;
	for (; *directions != 0; directions++)
	{

		//check all the allowed distances (if piece has them).
		for (int distance = 1; distance <= max_distance ; distance++)
		{
			direction_t direction = *directions;

			position_t dest;// = GetPositionRelative(piece.position, direction, distance);

			//empty spot. can do move.
			if (GetPiece(dest, game) == EMPTY)
			{
				//generate new move.
				move_t * newmove = (move_t *) mymalloc(sizeof (move_t));
				newmove->src = piece.position;
				newmove->num_captures = 0;
				position_t * destinations = (position_t *) mymalloc(sizeof (position_t));
				destinations[0] = dest;
				newmove->dest = destinations;

				//add move to the list.
				//create new node
				ListNode * newnode = ListCreateNode( (void *) newmove , sizeof (move_t));

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
			else //possible capture
			{

			}
		}
	}

	return list;
}

//does the move, does not check validation.
void DoMove (move_t * move, game_state_t * game)
{

	//get identity and destinations
	char identity = GetPiece (move->src, game);
	position_t * destinations = move->dest;

	//clear source
	SetPiece(move->src, EMPTY, game);


	//clear along the way
	int i;
	for (i=0; i<move->num_captures; i++)
	{
		//perform capture.
		SetPiece(destinations[i], EMPTY, game);

	}

	//mark final destination with identity
	SetPiece(destinations[i], identity, game);

	//update game piece counts.


}


//free memory of 1 move.
void MoveFree( void * data )
{
	move_t * p = (move_t * ) data;

	//free memory of destinations.
	if (p->dest!=0)
	{
		myfree(p->dest);
	}
	myfree (p);
}
