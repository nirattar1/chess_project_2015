/*
 * Game.h
 *
 *  Created on: Jun 7, 2015
 *      Author: nirattar
 */

#ifndef EX3_GAME_H_
#define EX3_GAME_H_

typedef struct
{
	int x;		//column
	int y; 		//row
} position_t;

//struct for one piece on board
//holds its location and its identity
//for example : <1,2> WHITE_K.
typedef struct {
	position_t 	position;
	char 		identity;
} piece_t;

//will encapsulate a given state of game.
//will hold the pieces.
typedef struct
{
	piece_t * pieces[2];		//2 lists of pieces on board: one for each side.
	int cnt_pieces [2];			//piece counts for both sides.
	//for chess: additional game information (i.e. castling)...
} game_state_t;

typedef struct
{
	piece_t 	src;			//source piece on board.
	position_t 	* dest;			//list of destination positions (more than 1 on successive captures).
	int 		num_captures;	//the number of captures in this move
} move_t;

//will build and return a struct of position, by x and y values.
position_t Position (int x , int y)
{
	position_t pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

//returns piece in requested position
//i.e. GetPiece (<1,2>) == 'm' (white man)
char GetPiece (position_t pos);

//get possible moves for piece in position pos
move_t * GetMovesForPiece (position_t pos);


//will perform the move pointed by "move"
//is responsible for updating the game state afterwards:
//1. capturing opponents
//2. promoting to king
void DoMove (const move_t * move, game_state_t * game);


#endif /* EX3_GAME_H_ */
