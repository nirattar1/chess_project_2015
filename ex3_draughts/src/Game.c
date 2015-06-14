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


position_t GetPositionRelative
	(position_t source, direction_t direction, int numSteps)
{

	position_t dest;
	if (direction == UP_RIGHT)
	{
		dest.x = source.x + numSteps;
		dest.y = source.y + numSteps;
	}
	if (direction == UP_LEFT)
	{
		dest.x = source.x - numSteps;
		dest.y = source.y + numSteps;
	}
	if (direction == DOWN_RIGHT)
	{
		dest.x = source.x + numSteps;
		dest.y = source.y - numSteps;
	}
	if (direction == DOWN_LEFT)
	{
		dest.x = source.x - numSteps;
		dest.y = source.y - numSteps;
	}
	return dest;
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



/**
//* Get (generate) all the allowed moves for 1 piece on board.
//@param game the game state.
//@param piece piece to play.
//@...
//@return a pointer to a list of moves.
//caller responsible to free the list.
 */
ListNode * GetMovesForPiece (game_state_t * game, piece_t piece)
{

	//prepare list .
	ListNode * list = NULL;

	//get possible directions.
	direction_t * directions = GetPieceDirections(piece.identity);

	//generate moves.
	//for each direction
	int max_distance = 1;
	for (; *directions != 0; directions++)
	{

		direction_t direction = *directions;

		//loop through all the allowed distances (if piece has them).
		for (int distance = 1; distance <= max_distance ; distance++)
		{

			//get 1 possible destination position.
			position_t dest = GetPositionRelative(piece.position, direction, distance);

			//position is empty. generate simple move.
			if (GetPiece(dest, game) == EMPTY)
			{
				//generate new move.
				move_t * newmove = MoveCreate(piece.position, dest);

				//add move to the list.
				ListNode ** listp = &list;
				ListPushBackElement (listp, (void *) newmove, sizeof (move_t));
				list = *listp;

			}
			//position is occupied.
			//try generating capture moves from it.
			else
			{

				//go 1 spot further in direction (loop for king)
				position_t afterDest = GetPositionRelative(dest, direction, 1);

				//TODO king check on each possible landing position

				//check validity of capture.
				if (IsValidCapture(game, piece.position, dest, afterDest))
				{

					//generate new capture move.
					move_t * newmove = MoveCreate(piece.position, afterDest);
					newmove->num_captures = 1;

					//get list of successive capture moves starting from this move.
					ListNode * captures = GetSuccessiveCapturesFromMove(game, newmove);

					ListNode ** listp = &list;
					//if call returned null - just add move to the main list.
					if (!captures)
					{
						//add move to the list.
						ListPushBackElement (listp, (void *) newmove, sizeof (move_t));
					}
					else
					{
						//if call returned a not null (list) - concat with main list
						ListConcat(listp, list);
					}
					//update list pointer .
					list = *listp;
				}
			}
		}
	}

	return list;
}



//tells if the move represented by 3 positions, is a valid capture.
//* note! assumes that the pieces aligned on same diagonal.
//checks 3 conditions:
//1. new dest. is in board boundaries.
//2. piece in middle is not empty and has opposite color.
//3. new dest. is empty
//returns 1 if valid, return 0 if not valid.
int IsValidCapture (game_state_t * game, position_t source, position_t middlePos, position_t newDest)
{

	return (PieceInBounds (newDest)
			&& (GetPiece(middlePos, game) != EMPTY)
			&& !SameColor(game, source, middlePos)
			&& (GetPiece(newDest, game) == EMPTY));
}


//gets a capture move (move with num_captures > 0).
//returns a list of ALL the capture moves, that are based on this move.
//recursive function. it will try to generate routes until no more captures can be done.
//(all these moves will have num_captures >= 1).
ListNode * GetSuccessiveCapturesFromMove (game_state_t * game, move_t * baseMove)
{
	//get the last position from move.
	position_t source = baseMove->dest [(baseMove->num_captures - 1)];

	//build moves list.
	ListNode * list = NULL;
	int foundCaptures = 0;

	//get possible directions
	//ALL directions are valid in successive captures (equal to king).
	//note: possible distances are only 1
	direction_t * directions = GetPieceDirections(WHITE_K);

	//generate moves.

	//for each direction
	for (; *directions != 0; directions++)
	{

		direction_t direction = *directions;

		//get destination
		position_t middlePos = GetPositionRelative(source, direction, 1);
		position_t newDest = GetPositionRelative(source, direction, 2);
		//if possible to eat:
		if (IsValidCapture (game, source, middlePos, newDest))
		{
			//report found.
			foundCaptures = 1;

			//duplicate baseMove into newMove.
			move_t * newMove = (move_t *) mymalloc(sizeof (move_t));
			memcpy(newMove, baseMove, sizeof (move_t));

			//add new_dest to destinations of newMove.
			newMove->dest[newMove->num_captures];

			//increment capture counts
			newMove->num_captures++;

			//call recursively with newMove.
			ListNode * Captures = GetSuccessiveCapturesFromMove(game, newMove);


			//if call returned null - create a list with one move (this destination) and return it.
			ListNode ** listp = &list;
			if (!Captures)
			{
				//add move to the list.
				ListPushBackElement (listp, (void *) newMove, sizeof (move_t));
			}
			//if call returned a not null (list) - concat with main list
			else
			{
				ListConcat(listp, list);
			}
			//update list pointer
			list = *listp;
		}
	}
	//if haven't found any successive capture - return null.
	if (!foundCaptures)
	{
		return NULL;
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

//generates a new move.
//from position src to position dest.
//(allocates and returns)
move_t * MoveCreate (position_t src, position_t dest)
{
	move_t * newmove = (move_t *) mymalloc(sizeof (move_t));
	newmove->src = src;
	newmove->num_captures = 0;
	newmove->dest[0] = dest;
	return newmove;
}

//free memory of 1 move.
void MoveFree( void * data )
{
	move_t * p = (move_t * ) data;

	//memory of destinations - static, no special handling.

	myfree (p);
}

