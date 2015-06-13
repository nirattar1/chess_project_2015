/*
 * Game.h
 *
 *	this class will be responsible for all game's logic.
 *  Created on: Jun 7, 2015
 *      Author: nirattar
 */

#ifndef EX3_GAME_H_
#define EX3_GAME_H_

#include "ListNode.h"


/*************
 * BASIC GAME DEFINITIONS
 * ***********/
#define BOARD_SIZE 10

//pieces identities
#define MAX_IDENTITIES 4
#define WHITE_M 'm'
#define WHITE_K 'k'
#define BLACK_M 'M'
#define BLACK_K 'K'
#define EMPTY ' '


//differentiate two players (sides of the game)
typedef enum
{
	PLAYER_WHITE,
	PLAYER_BLACK
} 	player_t;


/*************
 * POSITIONS ON BOARD
 * ***********/
//enum of the 8 directions you can walk on board.
//(for chess: add more for horse)
typedef enum
{
	UP_RIGHT  = 1 ,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT
	//	UP,
	//	DOWN,
	//	RIGHT,
	//	LEFT,

} direction_t;


//identities allowed directions.
direction_t * all_allowed_directions [MAX_IDENTITIES];

direction_t allowed_directions_whitem [3];
direction_t allowed_directions_whitek [5];
direction_t allowed_directions_blackm [3];
direction_t allowed_directions_blackk [5];


//just saved the directions.
void RulesInit ();


//struct for position (spot) on board.
//has x and y coordinates for column and row respectively.
typedef struct
{
	char x;		//column
	int y; 		//row
} position_t;


//position functions:

//will build and return a position, based on x and y values.
position_t Position (char x , int y);
//translate between position and row/column numbers.

//game language : row 2  (1-based).
//matrix language: row 1 (0-based).
int GetMatrixRow (position_t pos);

//game language : column a
//matrix language: column 0 (0-based).
int GetMatrixCol (position_t pos);

/**
 * return a position relative to base position.
 * examples: GetPositionRelative (<a,1> , UP_RIGHT, 1) == <b,2>
 * GetPositionRelative (<b,2> , RIGHT, 3) == <e,2>
 @param source : base position to go from.
 @param direction :	the direction to go.
 @parameter numSteps : number of steps to go.
 @return : the relative position.
 */
position_t GetPositionRelative
	(position_t source, direction_t direction, int numSteps);



/*************
 * PIECE STRUCT
************/

//struct for one piece on board
//holds its position and its identity
//for example : <1,2> WHITE_K.
typedef struct
{
	position_t 	position;
	char 		identity;
} piece_t;




/*************
 * GAME STATE
************/

// 	data structure that is used to represent the pieces on board.
//	implementation : matrix
//	optional implementation: 2 lists of pieces on board: one for each side.
//	pieces [PLAYER_WHITE] - white player's pieces.
//	pieces [PLAYER_BLACK] - black player's pieces.


typedef char board_column[BOARD_SIZE];
typedef board_column * PIECES_INTERNAL_DATA;


//game_state_t: structure that encapsulates a given state of game.
//will hold all the information such as: pieces on board.
typedef struct
{
	// whatever data structure is used to represent the pieces on board. (list or matrix)
	PIECES_INTERNAL_DATA pieces;

	//piece counts. also for both sides.
	int piecesCount [2];
	//for chess: additional game information (i.e. castling)...
} game_state_t;



//returns identity piece that is located in given position
//i.e. GetPiece (<1,2>) == 'm' (white man)
char GetPiece (position_t pos, game_state_t * game);

//puts piece in place
void SetPiece (position_t pos, char identity, game_state_t * game);


//returns a list of pieces on board that player has in current game.
ListNode * GetPiecesOfPlayer (player_t player, game_state_t * game);

//will initialize the game
void GameInit (game_state_t * game, char ** board );

//will initialize the game
void GameDefaultLayout (game_state_t * game);


/*************
 * MOVES ON BOARD
************/

//struct that holds information about one move of piece on board.
//the move can contain several destinations on the way (in case of successive captures)
typedef struct
{
	position_t 	src;			//source piece on board.
	position_t 	* dest;			//list of destination positions (more than 1 on successive captures).
	int 		num_captures;	//the number of captures in this move
} move_t;

//frees a move
void MoveFree( void * data );

//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity);


//get possible moves for 1 piece in game.
ListNode * GetMovesForPiece (game_state_t * game, piece_t piece,
		int is_successive_captures, position_t * dest_history);

ListNode * GetMovesForPieceDefault (game_state_t * game, piece_t piece) ;


//get all the possible moves for player in game.
//updates list moves.
//basic algorithm:
//1. iterate on all pieces in game.
//2. for each piece , run GetMovesForPiece and collect the moves.
//3. optional - sort moves by number of captures.
ListNode * GetMovesForPlayer (player_t player, game_state_t * game);


//will perform the move pointed by "move"
//is responsible for updating the game state afterwards:
//1. capturing opponents
//2. promoting to king
void DoMove (move_t * move, game_state_t * game);


#endif /* EX3_GAME_H_ */
