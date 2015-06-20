/*
 * Game.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include "Game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Memory.h"

direction_t * all_allowed_directions [MAX_IDENTITIES];


direction_t allowed_directions_whitem [3] = {UP_LEFT, UP_RIGHT, 0};
direction_t allowed_directions_whitek [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_blackm [3] = {DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_blackk [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};


void RulesInit ()
{
	all_allowed_directions [0] = allowed_directions_whitem;
	all_allowed_directions [1] = allowed_directions_whitek;
	all_allowed_directions [2] = allowed_directions_blackm;
	all_allowed_directions [3] = allowed_directions_blackk;
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

position_t PositionFromMatrixCoordinates( int i, int j)
{
	position_t pos;
	pos.x = 'a' + i;
	pos.y = 1 + j;
	return pos;
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

direction_t GetDirection (position_t src, position_t dest)
{
	if (dest.x > src.x && dest.y > src.y)
	{
		return UP_RIGHT;
	}
	else if (dest.x < src.x && dest.y > src.y)
	{
		return UP_LEFT;
	}
	if (dest.x > src.x && dest.y < src.y)
	{
		return DOWN_RIGHT;
	}
	else if (dest.x < src.x && dest.y < src.y)
	{
		return DOWN_LEFT;
	}

	return DOWN_LEFT;
}

int GetDistance (position_t src, position_t dest)
{

	//only the x distance matters (diagonal moves only)
	if (dest.x > src.x )
	{
		return (dest.x - src.x);
	}
	else
	{
		return (src.x - dest.x);
	}


}

int PositionInBounds (position_t pos)
{
	if (pos.x < 'a' || pos.x > ('a' + BOARD_SIZE - 1))
	{
		return 0;
	}

	if (pos.y < 1 || pos.y > BOARD_SIZE)
	{
			return 0;
	}

	return 1;
}


void GameInit (game_state_t * game, char ** board )
{

	//define counts
	game->piecesCount[COLOR_BLACK]  = 0;
	game->piecesCount[COLOR_WHITE]  = 0;

	//save pointer to board - assume matrix.
	game->pieces = (board_column *) board;

	//clear board .
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			game->pieces[i][j] = EMPTY;
		}
	}

}

//copy from one game state (including all board representation) to another state.
void CopyGameState (game_state_t * to, game_state_t * from)
{

//		//define counts
//		game->piecesCount[COLOR_BLACK]  = 0;
//		game->piecesCount[COLOR_WHITE]  = 0;



	//copy board
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			to->pieces[i][j] = from->pieces[i][j] ;
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
	game->piecesCount[COLOR_BLACK]  = 20;
	game->piecesCount[COLOR_WHITE]  = 20;

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


//will get all pieces of player with given color
//build an array of pieces
//will fill caller the array and pieces count.
void GetAllPieces (game_state_t * game, color_t color, piece_t * array, int * cnt_pieces)
{

	*cnt_pieces = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{

			position_t pos = PositionFromMatrixCoordinates(i, j);
			//reached user's piece
			if (GetPiece(pos, game) != EMPTY && GetPieceColor(game, pos) == color)
			{
				array[*cnt_pieces].position = pos;
				array[*cnt_pieces].identity = GetPiece(pos, game);
				(*cnt_pieces) ++;
			}
		}
	}

}


int SameColor(game_state_t * game, position_t pos1, position_t pos2)
{
	if (GetPiece(pos1, game)==EMPTY || GetPiece(pos2, game)==EMPTY)
	{
		return 0;
	}
	return (GetPieceColor(game, pos1) == GetPieceColor(game, pos2));

}


color_t GetOppositeColor(color_t player)
{
	if (player==COLOR_BLACK)
		return COLOR_WHITE;
	else
		return COLOR_BLACK;
}


color_t GetPieceColor (game_state_t * game, position_t pos)
{
	char identity   = GetPiece(pos, game);
	if (identity == WHITE_K || identity == WHITE_M)
	{
		return COLOR_WHITE;
	}

	if (identity ==	BLACK_K || identity == BLACK_M)
	{
		return COLOR_BLACK;
	}

	//if empty return 0.
	return 0;
}

//will return 1 if piece is man, 0 otherwise.
int IsMan (piece_t piece)
{
	if ((piece.identity == WHITE_M) || (piece.identity == BLACK_M) )
		return 1;
	else
		return 0;
}

//will return 1 if piece is king, 0 otherwise.
int IsKing (piece_t piece)
{
	if ((piece.identity == WHITE_K) || (piece.identity == BLACK_K) )
		return 1;
	else
		return 0;
}

//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity)
{
	if (identity == WHITE_M) {	return all_allowed_directions[0];};
	if (identity == WHITE_K) {	return all_allowed_directions[1];};
	if (identity == BLACK_M) {	return all_allowed_directions[2];};
	if (identity == BLACK_K) {	return all_allowed_directions[3];};

	return NULL;


}


//scoring function to use with minimax.
//based on player color and game state.
int DraughtsScoringFunction (game_state_t * game, color_t player)
{

	int player_score=0, opposite_score=0;

	//get player's pieces
	int cntPiecesPlayer;//will count how many pieces player has.
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, player, piecesPlayer, &cntPiecesPlayer);

	//get opposite color's pieces
	int cntPiecesOpposite;//will count how many pieces player has.
	piece_t piecesOpposite [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, GetOppositeColor(player), piecesOpposite, &cntPiecesOpposite);


	//determine winning score if one side has no pieces left.
	if (cntPiecesPlayer==0)
	{
		return SCORE_WIN_OPPONENT;
	}
	else if (cntPiecesOpposite==0)
	{
		return SCORE_WIN_PLAYER;
	}

	//in any other case, count pieces to compute score
	//iterate and add on both sides.
	for (int i=0; i<cntPiecesPlayer; i++)
	{
		if (IsMan (piecesPlayer[i]))
		{
			player_score += 1;
		}
		else if (IsKing (piecesPlayer[i]) )
		{
			player_score += 3;
		}
	}

	for (int i=0; i<cntPiecesOpposite; i++)
	{
		if (IsMan (piecesOpposite[i]))
		{
			opposite_score += 1;
		}
		else if (IsKing (piecesOpposite[i]) )
		{
			opposite_score += 3;
		}
	}


	//return the difference between player's and opponent's score
	return ( player_score - opposite_score ) ;

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

	return (PositionInBounds (newDest)
			&& (GetPiece(middlePos, game) != EMPTY)
			&& !SameColor(game, source, middlePos)
			&& (GetPiece(newDest, game) == EMPTY));
}


/**
// Get (generate) all the allowed moves for 1 piece on board.
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

	//get possible directions for normal (non-capture) move.
	direction_t * directions = GetPieceDirections(piece.identity);

	//generate moves.
	//for each direction

	//determine how far the piece can go in the first destination
	int max_distance = (IsKing(piece)) ? 10 : 1;

	for (; *directions != 0; directions++)
	{
		direction_t direction = *directions;

		//loop through all the allowed distances (if piece has them).
		for (int distance = 1; distance <= max_distance ; distance++)
		{

			//get 1 possible destination position.
			position_t dest = GetPositionRelative(piece.position, direction, distance);
			if (!PositionInBounds(dest))
			{
				continue;
			}

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

		}
	}


	//try to generate capture moves (on all directions)
	//capture moves are on all directions (take direction array of some king)
	directions = allowed_directions_whitek;
	for (; *directions != 0; directions++)
	{
		direction_t direction = *directions;

		//loop through all the allowed distances (if piece has them).
		for (int distance = 1; distance <= max_distance ; distance++)
		{

			//get 1 possible destination position.
			position_t dest = GetPositionRelative(piece.position, direction, distance);
			if (!PositionInBounds(dest))
			{
				continue;
			}
			//position is occupied.
			//try generating capture moves from it.
			if (GetPiece(dest, game) != EMPTY)
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
						//if call returned a not null (list) - concat. with main list
						ListConcat(listp, captures);
						myfree(newmove);
					}
					//update list pointer .
					list = *listp;
				}
			}

		}
	}


	//iterated on both normal moves and eat moves.


	return list;
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

		if (baseMove->num_captures>=2)
		{
			//check if moves are equal.
			//TODO use memcmp
			if (newDest.x == baseMove->dest [(baseMove->num_captures - 2)].x
					&& newDest.y == baseMove->dest [(baseMove->num_captures - 2)].y)
				continue;
		}
		if (IsValidCapture (game, source, middlePos, newDest))
		{
			//report found.
			foundCaptures = 1;

			//duplicate baseMove into newMove.
			move_t * newMove = (move_t *) mymalloc(sizeof (move_t));
			memcpy((void *)newMove, (void *) baseMove, sizeof (move_t));

			//add new_dest to destinations of newMove.
			newMove->dest[newMove->num_captures] = newDest;

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
				ListConcat(listp, Captures);
			}
			//update list pointer
			list = *listp;


		}
	}
	//if haven't found any successive capture - return null.
	if (!foundCaptures)
	{
		return NULL;
		//caller will be responsible for freeing base move.
	}

	return list;
}


//get all the possible moves for player 'color' in game.
//will generate and return a list of the moves.
//basic algorithm:
//1. iterate on all pieces of the player in the game.
//2. for each piece, run GetMovesForPiece and collect the moves.
//3. optional - sort moves by number of captures.
ListNode * GetMovesForPlayer (game_state_t * game, color_t color)
{

	//get user's pieces
	int cntPiecesPlayer;//will count how many pieces player has.
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, color, piecesPlayer, &cntPiecesPlayer);


	//list of moves (start with empty ).
	ListNode * allMoves = NULL;

	//iterate on pieces
	for (int i=0; i<cntPiecesPlayer; i++)
	{
		//get moves per piece
		ListNode * currMoves = GetMovesForPiece(game, piecesPlayer[i]);
		//concat with general list.
		ListConcat(&allMoves, currMoves);
	}

	return allMoves;
}


void ClearPiecesInBetween (game_state_t * game, position_t src, position_t dest)
{
	//get the direction between the two
	direction_t direction = GetDirection (src, dest);
	int distance = GetDistance (src, dest);
	//start diagonally from src until dest, clear everyone in the way.
	//(it's okay because of draughts capture rules)
	for (int i=1; i < distance; i++)
	{
		//clear everyone on the way
		SetPiece (GetPositionRelative(src, direction, i), EMPTY, game);
	}
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
	position_t prev = move->src; 	//remember the last spot and kill in between
	for (i=0; i<move->num_captures; i++)
	{
		position_t curr = destinations[i];

		//perform capture (clear middle piece)
		ClearPiecesInBetween(game, prev, curr);
		prev = curr;
	}

	//mark final destination with identity

	position_t final;
	if (move->num_captures==0)
	{
		final = destinations[0];
	}
	else
	{
		final = destinations[move->num_captures-1];
	}

	SetPiece(final, identity, game);



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

//returns 1 if mymove is in list, 0 otherwise
int FindMoveInList (ListNode * moves, move_t * mymove)
{

	for ( ; moves !=NULL; moves = moves->next )
	{
		move_t * move = (move_t *) moves->data;
		if (move)
		{
			if (move->src.x==mymove->src.x && move->src.y==mymove->src.y)
			{
				if (move->dest[0].x==mymove->dest[0].x && move->dest[0].y==mymove->dest[0].y)
				{
					//TODO check all destinations are same
					//copy all destinations from original move
					for (int i=0; i<MAX_CAPTURES_MOVE; i++)
					{
						mymove->dest[i] = move->dest[i];
					}
					//update number of captures.
					mymove->num_captures = move->num_captures;
					return 1;
				}
			}
		}
	}

	return 0;
}
