/*
 * Game.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include "Game.h"
#include "Game_Mgr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Memory.h"

//static functions declarations
//will add the move by piece , to it's destination - to the list by listp.
//the move may be a normal /capture move.
//if the move follows promotion rules,
//then 4 possibilities of the promotion, will be added to the list.
static void MoveAddWithPossiblePromotion (game_state_t * game, ListNode ** listp, piece_t piece, position_t dest, int is_capture);

//returns whether the hop from piece to dest, is worth promotion.
static int IsPromotionMove (piece_t piece, position_t dest);





direction_t * all_allowed_directions [MAX_IDENTITIES];



//allowed directions.
//for all pieces except pawn,
//the allowed directions are same for simple move and for capture move.
//for pawn there is a separate array for capture directions.
direction_t allowed_directions_whitem [2] = {UP, 0};
direction_t allowed_directions_whitem_for_capture [3] = {UP_RIGHT, UP_LEFT, 0};
direction_t allowed_directions_whiteb [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_whiten [9] =
	{KNIGHT_1, KNIGHT_2, KNIGHT_3, KNIGHT_4, KNIGHT_5, KNIGHT_6, KNIGHT_7, KNIGHT_8, 0};
direction_t allowed_directions_whiter [5] = {UP, DOWN, LEFT, RIGHT, 0};
direction_t allowed_directions_whiteq [9] =
	{UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT, 0};
direction_t allowed_directions_whitek [9] =
	{UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT, 0};

direction_t allowed_directions_blackm [2] = {DOWN, 0};
direction_t allowed_directions_blackm_for_capture [3] = {DOWN_RIGHT, DOWN_LEFT, 0};
direction_t allowed_directions_blackb [5] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, 0};
direction_t allowed_directions_blackn [9] =
	{KNIGHT_1, KNIGHT_2, KNIGHT_3, KNIGHT_4, KNIGHT_5, KNIGHT_6, KNIGHT_7, KNIGHT_8, 0};
direction_t allowed_directions_blackr [5] = {UP, DOWN, LEFT, RIGHT, 0};
direction_t allowed_directions_blackq [9] =
	{UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT, 0};
direction_t allowed_directions_blackk [9] =
	{UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT, 0};

//an array of all directions.
direction_t allowed_directions_array_of_all [17] =
	{UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT,
	KNIGHT_1, KNIGHT_2, KNIGHT_3, KNIGHT_4, KNIGHT_5, KNIGHT_6, KNIGHT_7, KNIGHT_8, 0};

//promotion options.
char pawn_promotion_options_white[PAWN_PROMOTION_OPTIONS_SIZE] = {WHITE_Q, WHITE_R, WHITE_B, WHITE_N};
char pawn_promotion_options_black[PAWN_PROMOTION_OPTIONS_SIZE] = {BLACK_Q, BLACK_R, BLACK_B, BLACK_N};


void RulesInit ()
{
	all_allowed_directions [0] = allowed_directions_whitem;
	all_allowed_directions [1] = allowed_directions_whiteb;
	all_allowed_directions [2] = allowed_directions_whiten;
	all_allowed_directions [3] = allowed_directions_whiter;
	all_allowed_directions [4] = allowed_directions_whiteq;
	all_allowed_directions [5] = allowed_directions_whitek;
	all_allowed_directions [6] = allowed_directions_blackm;
	all_allowed_directions [7] = allowed_directions_blackb;
	all_allowed_directions [8] = allowed_directions_blackn;
	all_allowed_directions [9] = allowed_directions_blackr;
	all_allowed_directions [10] = allowed_directions_blackq;
	all_allowed_directions [11] = allowed_directions_blackk;

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

	if (direction == UP)
	{
		dest.x = source.x;
		dest.y = source.y + numSteps;
	}
	if (direction == LEFT)
	{
		dest.x = source.x - numSteps;
		dest.y = source.y;
	}
	if (direction == RIGHT)
	{
		dest.x = source.x + numSteps;
		dest.y = source.y;
	}
	if (direction == DOWN)
	{
		dest.x = source.x;
		dest.y = source.y - numSteps;
	}
	if (direction == KNIGHT_1)
	{
		dest.x = source.x + 1;
		dest.y = source.y + 2;
	}
	if (direction == KNIGHT_2)
	{
		dest.x = source.x + 2;
		dest.y = source.y + 1;
	}
	if (direction == KNIGHT_3)
	{
		dest.x = source.x + 2;
		dest.y = source.y - 1;
	}
	if (direction == KNIGHT_4)
	{
		dest.x = source.x + 1;
		dest.y = source.y - 2;
	}
	if (direction == KNIGHT_5)
	{
		dest.x = source.x - 1;
		dest.y = source.y - 2;
	}
	if (direction == KNIGHT_6)
	{
		dest.x = source.x - 2;
		dest.y = source.y - 1;
	}
	if (direction == KNIGHT_7)
	{
		dest.x = source.x - 2;
		dest.y = source.y + 1;
	}
	if (direction == KNIGHT_8)
	{
		dest.x = source.x - 1;
		dest.y = source.y + 2;
	}
	return dest;
}


direction_t GetDirection (position_t src, position_t dest)
{
	//check knight directions.
	if (dest.x==src.x+1 && dest.y==src.y+2)
	{
		return KNIGHT_1;
	}
	if (dest.x==src.x+2 && dest.y==src.y+1)
	{
		return KNIGHT_2;
	}
	if (dest.x==src.x+2 && dest.y==src.y-1)
	{
		return KNIGHT_3;
	}
	if (dest.x==src.x+1 && dest.y==src.y-2)
	{
		return KNIGHT_4;
	}
	if (dest.x==src.x-1 && dest.y==src.y-2)
	{
		return KNIGHT_5;
	}
	if (dest.x==src.x-2 && dest.y==src.y-1)
	{
		return KNIGHT_6;
	}
	if (dest.x==src.x-2 && dest.y==src.y+1)
	{
		return KNIGHT_7;
	}
	if (dest.x==src.x-1 && dest.y==src.y+2)
	{
		return KNIGHT_8;
	}

	//check all other directions
	if (dest.x > src.x && dest.y > src.y)
	{
		return UP_RIGHT;
	}
	if (dest.x < src.x && dest.y > src.y)
	{
		return UP_LEFT;
	}
	if (dest.x > src.x && dest.y < src.y)
	{
		return DOWN_RIGHT;
	}
	if (dest.x < src.x && dest.y < src.y)
	{
		return DOWN_LEFT;
	}
	if (dest.x == src.x && dest.y > src.y)
	{
		return UP;
	}
	if (dest.x == src.x && dest.y < src.y)
	{
		return DOWN;
	}
	if (dest.x > src.x && dest.y == src.y)
	{
		return RIGHT;
	}
	if (dest.x < src.x && dest.y == src.y)
	{
		return LEFT;
	}

	//TODO check option to return null (does not interfere allowed arrays?)
	return DOWN_LEFT;
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

//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity)
{
	if (identity == WHITE_M) {	return all_allowed_directions[0];};
	if (identity == WHITE_B) {	return all_allowed_directions[1];};
	if (identity == WHITE_N) {	return all_allowed_directions[2];};
	if (identity == WHITE_R) {	return all_allowed_directions[3];};
	if (identity == WHITE_Q) {	return all_allowed_directions[4];};
	if (identity == WHITE_K) {	return all_allowed_directions[5];};
	if (identity == BLACK_M) {	return all_allowed_directions[6];};
	if (identity == BLACK_B) {	return all_allowed_directions[7];};
	if (identity == BLACK_N) {	return all_allowed_directions[8];};
	if (identity == BLACK_R) {	return all_allowed_directions[9];};
	if (identity == BLACK_Q) {	return all_allowed_directions[10];};
	if (identity == BLACK_K) {	return all_allowed_directions[11];};

	return NULL;
}

int IsDirectionAllowedForPieceCapture(char identity, direction_t direction)
{
	//find the allowed directions to capture, for this identity.
	direction_t * allowed_directions = NULL;

	//handle pawn cases.
	if (identity==WHITE_M || identity==BLACK_M)
	{
		allowed_directions = GetPawnCaptureDirections(identity);
	}
	else
	{
		//non-pawn, the directions are same as in simple move.
		allowed_directions = GetPieceDirections(identity);
	}

	//try to find given direction in this array
	for (; *allowed_directions != 0; allowed_directions++)
	{
		if (*allowed_directions == direction)
		{
			return 1;
		}
	}

	//if reached here then the direction is not allowed for this identity
	return 0;
}

direction_t * GetPawnCaptureDirections (char identity)
{
	if (identity == WHITE_M) {return allowed_directions_whitem_for_capture;};
	if (identity == BLACK_M) {return allowed_directions_blackm_for_capture;};
	return NULL;
}


int GetPieceMaxNumOfHops(char identity)
{
	switch (identity)
	{
		case WHITE_M:
			return 1;
		case WHITE_B:
			return (BOARD_SIZE-1);
		case WHITE_N:
			return 1;
		case WHITE_R:
			return (BOARD_SIZE-1);
		case WHITE_Q:
			return (BOARD_SIZE-1);
		case WHITE_K:
			return 1;
		case BLACK_M:
			return 1;
		case BLACK_B:
			return (BOARD_SIZE-1);
		case BLACK_N:
			return 1;
		case BLACK_R:
			return (BOARD_SIZE-1);
		case BLACK_Q:
			return (BOARD_SIZE-1);
		case BLACK_K:
			return 1;
		default:
			return -1;
	}
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

	//black
	SetPiece(Position('a',8), BLACK_R, game);
	SetPiece(Position('b',8), BLACK_N, game);
	SetPiece(Position('c',8), BLACK_B, game);
	SetPiece(Position('d',8), BLACK_Q, game);
	SetPiece(Position('e',8), BLACK_K, game);
	SetPiece(Position('f',8), BLACK_B, game);
	SetPiece(Position('g',8), BLACK_N, game);
	SetPiece(Position('h',8), BLACK_R, game);
	for (int i=0; i<BOARD_SIZE; i++)
	{
		SetPiece(Position('a'+i,7), BLACK_M, game);
	}
	//white
	SetPiece(Position('a',1), WHITE_R, game);
	SetPiece(Position('b',1), WHITE_N, game);
	SetPiece(Position('c',1), WHITE_B, game);
	SetPiece(Position('d',1), WHITE_Q, game);
	SetPiece(Position('e',1), WHITE_K, game);
	SetPiece(Position('f',1), WHITE_B, game);
	SetPiece(Position('g',1), WHITE_N, game);
	SetPiece(Position('h',1), WHITE_R, game);
	for (int i=0; i<BOARD_SIZE; i++)
	{
		SetPiece(Position('a'+i,2), WHITE_M, game);
	}

}




int SameColor(game_state_t * game, position_t pos1, position_t pos2)
{
	if (GetPiece(pos1, game)==EMPTY || GetPiece(pos2, game)==EMPTY)
	{
		return 0;
	}
	return (GetColor(GetPiece(pos1,game)) == GetColor(GetPiece(pos2,game)));

}


color_t GetOppositeColor(color_t player)
{
	if (player==COLOR_BLACK)
		return COLOR_WHITE;
	else
		return COLOR_BLACK;
}


color_t GetColor (char identity)
{

	if (identity == WHITE_M || identity == WHITE_B || identity == WHITE_N
			|| identity == WHITE_R || identity == WHITE_Q || identity == WHITE_K)
	{
		return COLOR_WHITE;
	}

	if (identity == BLACK_M || identity == BLACK_B || identity == BLACK_N
			|| identity == BLACK_R || identity == BLACK_Q || identity == BLACK_K)
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
//will return 1 if piece is bishop, 0 otherwise.
int IsBishop (piece_t piece)
{
	if ((piece.identity == WHITE_B) || (piece.identity == BLACK_B) )
		return 1;
	else
		return 0;
}
//will return 1 if piece is knight, 0 otherwise.
int IsKnight (piece_t piece)
{
	if ((piece.identity == WHITE_N) || (piece.identity == BLACK_N) )
		return 1;
	else
		return 0;
}
//will return 1 if piece is rook, 0 otherwise.
int IsRook (piece_t piece)
{
	if ((piece.identity == WHITE_R) || (piece.identity == BLACK_R) )
		return 1;
	else
		return 0;
}
//will return 1 if piece is queen, 0 otherwise.
int IsQueen (piece_t piece)
{
	if ((piece.identity == WHITE_Q) || (piece.identity == BLACK_Q) )
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
			char identity = GetPiece(pos, game);
			if (identity != EMPTY && GetColor(identity) == color)
			{
				array[*cnt_pieces].position = pos;
				array[*cnt_pieces].identity = GetPiece(pos, game);
				(*cnt_pieces) ++;
			}
		}
	}

}


board_validation_status_t IsValidBoard (game_state_t * game)
{

	//check if one of kings are missing.
	position_t king_position;
	if (!GetKingPosition(game, COLOR_WHITE, &king_position)
			|| !GetKingPosition(game, COLOR_BLACK, &king_position) )
	{
		return BOARD_VALIDITY_KING_IS_MISSING;
	}

	//TODO check if piece counts by types do not exceed limits.


	return BOARD_VALIDITY_OK;
}

//checks if it is valid to set piece of certain identity and position, to game.
//returns 1 if valid, 0 if not valid.
int IsValidPieceAddition (game_state_t * game, position_t position, char identity)
{
	//create a stack copy of the game state, and perform the add on it.
	game_state_t newState;
	char newBoard [BOARD_SIZE][BOARD_SIZE];
	newState.pieces = (board_column *) newBoard;
	CopyGameState(&newState, game);

	//add the piece
	SetPiece(position, identity, &newState);

	//return if new board piece count is OK.
	//(not checking other checks)
	if (IsValidBoard(&newState)==BOARD_VALIDITY_TOO_MANY_PIECES_OF_TYPE)
	{
		return 0;
	}

	return 1;
}

int IsValidCapture (game_state_t * game, position_t source, position_t newDest)
{

	return (PositionInBounds (newDest)
			&& (GetPiece(newDest, game) != EMPTY)
			&& (GetPiece(source, game) != EMPTY)
			&& !SameColor(game, source, newDest));
}


int IsValidCaptureMoreChecks(game_state_t * game, position_t src, position_t dest, int distance)
{
	//do all the trivial checks
	if (!IsValidCapture(game, src, dest))
	{
		return 0;
	}

	//get the 2 pieces.
	char src_identity = GetPiece(src, game);

	//check that source is allowed to walk the distance.
	if (distance > GetPieceMaxNumOfHops(src_identity))
	{
		return 0;
	}

	//get direction between them
	direction_t direction = GetDirection(src, dest);
	//check that source can capture in this direction.
	if (!IsDirectionAllowedForPieceCapture(src_identity, direction))
	{
		return 0;
	}


	return 1;
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

	if (piece.identity == EMPTY)
	{
		DEBUG_PRINT(("Error: asked for GetMoves for empty piece."));
		return NULL;
	}

	//prepare list .
	ListNode * list = NULL;

	//get possible directions for simple (non-capture) move.
	direction_t * directions = GetPieceDirections(piece.identity);

	//generate moves.
	//for each direction

	//determine how far the piece can go in direction
	int max_distance = GetPieceMaxNumOfHops(piece.identity);

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
				//no point in going more in this direction
				break;
			}

			//position is empty. generate simple move.
			if (GetPiece(dest, game) == EMPTY)
			{
				//generate move (possible promotion).
				ListNode ** listp = &list;
				MoveAddWithPossiblePromotion (game, listp, piece, dest, 0); //no capture
				list = *listp;
			}

			else if (!IsMan(piece))
				//piece is not pawn, and is blocked by another piece.
				//will try to capture in same direction.
				//(pawn captures separately)
			{

				//try to generate capture move (once)

				//check validity of capture.
				if (IsValidCapture(game, piece.position, dest))
				{
					//generate new capture move, add it to list.
					//(promotion will not happen since it's not pawn).
					ListNode ** listp = &list;
					MoveAddWithPossiblePromotion (game, listp, piece, dest, 1); //capture
					list = *listp;
				}
				//no point in going more in this direction
				//(either capture opponent's piece or blocked by same color piece.)
				break;
			}

		}
	}

	if (IsMan(piece))
	{
		//get possible directions for pawn capture.
		direction_t * directions = GetPawnCaptureDirections(piece.identity);

		for (; *directions != 0; directions++)
		{
			direction_t direction = *directions;
			//get 1 possible destination position.
			position_t dest = GetPositionRelative(piece.position, direction, 1); //in this case max_distance == 1
			if (!PositionInBounds(dest))
			{
				//out of bounds, try next direction
				continue;
			}

			//position is not empty. try to generate valid capture move.
			if (GetPiece(dest, game) != EMPTY && IsValidCapture(game, piece.position, dest))
			{
				//generate new capture move (possible promotion).
				ListNode ** listp = &list;
				MoveAddWithPossiblePromotion (game, listp, piece, dest, 1); //capture
				list = *listp;
			}

			//if not a valid capture, move on
		}
	}

	//iterated on both normal moves and eat moves.
	//return the generated list.

	return list;
}

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



//does the move, does not check validation.
//also does promotion if exists.
void DoMove (move_t * move, game_state_t * game)
{

	//get original identity and destinations
	char identity = GetPiece (move->src, game);

	//clear source
	SetPiece(move->src, EMPTY, game);

	//if promotion, get updated identity
	if (move->promote_to_identity!=0)
	{
		identity = move->promote_to_identity;
	}

	//fill destination with identity
	SetPiece(move->dest, identity, game);

}

move_t * MoveCreate (position_t src, position_t dest)
{
	//call with defaults- no capture, no promotion.
	return MoveCreateWithOptions (src, dest, 0, 0);
}

//generates a new move.
//from position src to position dest.
//(allocates and returns)
move_t * MoveCreateWithOptions (position_t src, position_t dest,
		int is_capture, char promote_to_identity)
{
	//allocate memory for move.
	move_t * newmove = (move_t *) mymalloc(sizeof (move_t));
	//add the source.
	newmove->src = src;
	//add the destination.
	newmove->dest = dest;
	//update if it's a capture (0 or 1)
	newmove->num_captures = is_capture;
	//add the identity to promote to (if it's a promotion).
	newmove->promote_to_identity = promote_to_identity;
	return newmove;
}


static int IsPromotionMove (piece_t piece, position_t dest)
{
	//not a pawn - cannot promote.
	if (!IsMan(piece))
	{
		return 0;
	}

	//it is pawn, check it has reached the appropriate row.
	if (GetColor(piece.identity) == COLOR_WHITE && dest.y==LAST_ROW_WHITE)
	{
		return 1;
	}

	else if (GetColor(piece.identity) == COLOR_BLACK && dest.y==LAST_ROW_BLACK)
	{
		return 1;
	}

	else //pawn but didn't reach last row.
	{
		return 0;
	}

}


//will add the move by piece , to it's destination - to the list by listp.
//the move may be a normal /capture move.
//if the move follows promotion rules,
//then 4 possibilities of the promotion, will be added to the list.
static void MoveAddWithPossiblePromotion (game_state_t * game, ListNode ** listp, piece_t piece, position_t dest, int is_capture)
{

	//check promotion conditions.
	if (IsPromotionMove(piece, dest))
	{

		//promotion move. add a list of the promotion options.
		char promote_to_identity = 0;
		for (int i=0; i<PAWN_PROMOTION_OPTIONS_SIZE; i++)
		{
			//iterate on all options (for appropriate color)
			if (GetColor(piece.identity) == COLOR_WHITE)
			{
				promote_to_identity = pawn_promotion_options_white[i];
			}
			else
			{
				promote_to_identity = pawn_promotion_options_black[i];
			}

			//create a move with this option
			move_t * newmove = MoveCreateWithOptions(piece.position, dest, is_capture, promote_to_identity); //promotion move

			//add the move only if it's legal
			//(not revealing king)
			//TODO maybe more conditions ?
			if (!IsMoveRevealingKing(game, GetColor(piece.identity), newmove))
			{
				//move is legal, add it to the list (becomes owner of move)
				ListPushBackElement (listp, (void *) newmove, sizeof (move_t));
			}
			else
			{
				//move is illegal.
				//free the move.
				MoveFree((void *) newmove);
			}
		}
	}
	else
	{

		//no promotion, create simple/capture move.
		move_t * newmove = MoveCreateWithOptions(piece.position, dest, is_capture, 0); //0 -no promotion

		//add the move only if it's legal
		//(not revealing king)
		//TODO maybe more conditions ?
		if (!IsMoveRevealingKing(game, GetColor(piece.identity), newmove))
		{
			//move is legal, add it to list (list becomes owner of move).
			ListPushBackElement (listp, (void *) newmove, sizeof (move_t));
		}
		else
		{
			//move is illegal, free it.
			MoveFree((void *) newmove);
		}
	}
}

void MoveFree( void * data )
{
	move_t * p = (move_t * ) data;

	//memory of destinations - static, no special handling.

	myfree (p);
}

void MoveCopy( move_t * m2, move_t * m1 )
{
	memcpy((void *) m2, (void *) m1, sizeof(move_t));
}


int FindMoveInList 	(ListNode * moves, position_t src, position_t dest,
		char promotion_identity, move_t * move_return, int * move_index)
{

	//avoid null ptr, give default value (not found)
	if (move_index)
	{
		*move_index = -1;
	}

	//iterate through moves list.
	int i = 0;
	for ( ; moves !=NULL; moves = moves->next )
	{
		move_t * move = (move_t *) moves->data;

		//avoid null move.
		if (move)
		{
			//compare the move based on given arguments.
			if (move->src.x==src.x && move->src.y==src.y
					&& move->dest.x==dest.x && move->dest.y==dest.y)
			{
				//TODO promotion + default
				//found the move. return it through argument.
				//TODO handle null move_return
				MoveCopy(move_return, move);
				//also update the index inside list.
				if (move_index)
				{
					*move_index = i;
				}
				return 1;
			}
		}
		//increment index.
		i++;
	}

	//not found the move.
	return 0;
}


//updates the position of color's king on board (by reference).
//if king is not found returns 0.
//otherwise returns 1.
int GetKingPosition (game_state_t * game, color_t color, position_t * pos)
{
	//get player's pieces
	int cntPiecesPlayer;//will count how many pieces player has.
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //will be filled by pieces
	GetAllPieces (game, color, piecesPlayer, &cntPiecesPlayer);

	//iterate on pieces to find king
	for (int i=0; i<cntPiecesPlayer; i++)
	{
		if (IsKing(piecesPlayer[i]) && color==GetColor(piecesPlayer[i].identity))
		{
			//it is color's king . update it's position in "pos" argument.
			pos->x = piecesPlayer[i].position.x;
			pos->y = piecesPlayer[i].position.y;
			return 1; //success
		}
	}

	//king not found, return bad status
	return 0;
}

//return whether there is a 'check' state for player "color"
//(if player's king is threatened).
int IsCheckState (game_state_t * game, color_t color)
{

	//find player's king
	position_t king_position;
	if (!GetKingPosition (game, color, &king_position))
	{
		//king is not found (error)
		DEBUG_PRINT(("ERROR: king not found.\n"));
		return 0;
	}

	//try to find a threatening piece on king

	//iterate on all directions
	direction_t * directions = allowed_directions_array_of_all;
	for (; *directions != 0; directions++)
	{
		//try to find a threatening piece on direction
		direction_t direction = *directions;

		//determine max distance. 1 if direction is a knight's, or otherwise - the farthest.
		int max_distance = (direction>=KNIGHT_1 && direction<=KNIGHT_8) ? 1 : (BOARD_SIZE-1);

		//loop through all the distances.
		for (int distance = 1; distance <= max_distance ; distance++)
		{
			//get 1 possible position.
			position_t piece_position = GetPositionRelative(king_position, direction, distance);
			if (!PositionInBounds(piece_position))
			{
				//out of bounds, move on to next direction
				break;
			}
			//in bounds, find the piece in that position
			char identity = GetPiece(piece_position, game);

			if (identity == EMPTY)
			{
				//empty square, move on to next hop
				continue;
			}
			else
			{
				//non-empty square, check if piece can capture the king.
				if (IsValidCaptureMoreChecks(game, piece_position, king_position,distance))
				{
					return 1;
				}

				//if reached here, then the piece does not threat the king.
				//meaning- we finished with this direction, move on to next direction.
				break;
			}

		}

		//there must be nothing here. (due to breaks)
	}


	//no piece can capture player's king, return false.
	return 0;
}

int IsMoveRevealingKing (game_state_t * state, color_t player, move_t * move)
{
	//create a stack copy of the game state, and perform the move on it.
	game_state_t newState;
	char newBoard [BOARD_SIZE][BOARD_SIZE];
	newState.pieces = (board_column *) newBoard;

	CopyGameState(&newState, state);
	//update state based on move (play move)
	DoMove(move, &newState);

	//check if the new state is a check state for player
	return (IsCheckState(&newState, player));

}


//TODO need refine - with next player.
play_status_t GetPlayStatus (game_state_t * game, color_t current_player)
{

	color_t opponent_player = GetOppositeColor(current_player);

	//get the moves for both players.
	//(only interested if there are moves or not, freeing the lists themselves)
	ListNode * list_moves_player = GetMovesForPlayer(game, current_player); //free later
	ListNode * list_moves_opponent = GetMovesForPlayer(game, opponent_player); //free later
	int moves_player = (list_moves_player!=NULL);
	int moves_opponent = (list_moves_opponent!=NULL);
	ListFreeElements(list_moves_player, MoveFree);
	ListFreeElements(list_moves_opponent, MoveFree);

	//check if there is "check" state on either side.
	int is_player_in_check = IsCheckState(game, current_player);
	int is_opponent_in_check = IsCheckState(game, opponent_player);

	//end game situations (checkmate/tie)
	if (!moves_player) //player can't move
	{
		if (is_player_in_check) //if player in check, then checkmate (player lost)
		{
			return STATUS_PLAYER_IN_CHECKMATE;
		}
		else	//player can't move but not treatened, it is just tie for player.
		{
			return STATUS_TIE;
			//TODO return speicfic tie for player ?
		}
	}

	if (!moves_opponent) //opponent can't move
	{
		if (is_opponent_in_check) //if opponent in check, then checkmate (player wins)
		{
			return STATUS_OPPONENT_IN_CHECKMATE;
		}
		else	//opponent can't move but not treatened, it is just tie for opponent.
		{
			return STATUS_TIE;
			//TODO return speicfic tie for player ?
		}
	}


	//'check' situations
	//(if 'checkmate' then already handled.)
	if (is_player_in_check && moves_player)
	{
		//player in check but still has moves.
		return STATUS_PLAYER_IN_CHECK;
	}
	if (is_opponent_in_check && moves_opponent)
	{
		//opponent in check but still has moves.
		return STATUS_OPPONENT_IN_CHECK;
	}

	//didn't answer any of end game conditions.
	//can continue play.
	return STATUS_CONTINUE_PLAY;
}



