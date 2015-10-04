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
#define BOARD_SIZE 8
#define MAX_PIECES_PLAYER 20


#define SCORE_WIN_PLAYER 	100
#define SCORE_WIN_OPPONENT 	-100


//pieces identities
#define MAX_IDENTITIES 12
#define WHITE_M 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_M 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'
#define EMPTY ' '


//differentiate two colors (sides of the game)
typedef enum
{
	COLOR_WHITE = 1,
	COLOR_BLACK
} 	color_t;


//return the opposite color to given color.
color_t GetOppositeColor(color_t player);



/*************
 * POSITIONS ON BOARD
 * ***********/
//enum of the directions pieces can move on board.
typedef enum
{
	UP_RIGHT  = 1 ,	UP_LEFT,	DOWN_RIGHT,	DOWN_LEFT,
	UP,	DOWN,	RIGHT,	LEFT,
	KNIGHT_1,	KNIGHT_2,	KNIGHT_3,	KNIGHT_4,
	KNIGHT_5,	KNIGHT_6,	KNIGHT_7,	KNIGHT_8
} direction_t;


//identities allowed directions.
//see RulesInit-  fills these values.
direction_t * all_allowed_directions [MAX_IDENTITIES];

direction_t allowed_directions_whitem [2];
direction_t allowed_directions_whitem_for_capture [3];
direction_t allowed_directions_whiteb [5];
direction_t allowed_directions_whiten [9];
direction_t allowed_directions_whiter [5];
direction_t allowed_directions_whiteq [9];
direction_t allowed_directions_whitek [9];

direction_t allowed_directions_blackm [2];
direction_t allowed_directions_blackm_for_capture [3];
direction_t allowed_directions_blackb [5];
direction_t allowed_directions_blackn [9];
direction_t allowed_directions_blackr [5];
direction_t allowed_directions_blackq [9];
direction_t allowed_directions_blackk [9];


//fills values of all_allowed_directions based on rules.
void RulesInit ();


//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity);

//will receive identity of pawn, and return its allowed directions FOR A CAPTURE MOVE.
//(diagonal, based on black or white pawn).
direction_t * GetPawnCaptureDirections (char identity);

//will return the maximum number of hops possible for 1 move of the piece.
//i.e. for queen, rook, bishop - can move up to BOARD_SIZE-1.
//for king, knight, pawn - only 1.
int GetPieceMaxNumOfHops(char identity);

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

//determine direction and distance between two positions
direction_t GetDirection (position_t src, position_t dest);
int GetDistance (position_t src, position_t dest);


//return whether position is inside board boundaries.
//1 = true
//0 = false
int PositionInBounds (position_t pos);


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


//copy from one game state (including all board representation) to another state.
void CopyGameState (game_state_t * to, game_state_t * from);


//returns identity piece that is located in given position
//i.e. GetPiece (<1,2>) == 'm' (white man)
char GetPiece (position_t pos, game_state_t * game);

//puts piece in place
void SetPiece (position_t pos, char identity, game_state_t * game);

//returns the color (black/white)
//of piece in given position, or 0 if empty.
color_t GetPieceColor (game_state_t * game, position_t pos);

//returns whether pieces in 2 positions are in same color.
//1 = both pieces in positions exist and same color
//0 = different colors or at least one is empty.
int SameColor(game_state_t * game, position_t pos1, position_t pos2);

//will return 1 if piece is man, 0 otherwise.
int IsMan (piece_t piece);

//will return 1 if piece is king, 0 otherwise.
int IsKing (piece_t piece);


//will initialize the game
void GameInit (game_state_t * game, char ** board );

//will initialize the game
void GameDefaultLayout (game_state_t * game);


//scoring function to use with minimax.
//based on player and game state.
int DraughtsScoringFunction (game_state_t * game, color_t player);

//did someone win game
int GameWinning(game_state_t * game, color_t color);

/*************
 * MOVES ON BOARD
************/
//tells if the move represented by 2 positions, is a valid capture.
//checks 3 conditions:
//1. new dest. is in board boundaries.
//2. both positions are not empty.
//3. destination has a piece of opposite color.
//returns 1 if valid, return 0 if not valid.
int IsValidCapture (game_state_t * game, position_t source, position_t newDest);

//maximum captures in 1 move (if eat all opponents).
#define MAX_CAPTURES_MOVE 20

//struct that holds information about one move of piece on board.
//the move can contain several destinations on the way (in case of successive captures)
typedef struct
{
	//source piece on board.
	position_t 	src;
	//series of destination positions (1 on normal move, more on successive captures).
	position_t 	dest [MAX_CAPTURES_MOVE];
	//TODO dest is only 1 now.
	//the number of captures in this move
	int 		num_captures;
} move_t;


//generates a new move.
//from position src to position dest.
//(allocates and returns)
move_t * MoveCreate (position_t src, position_t dest);

//frees a move
void MoveFree( void * data );


//get all the pieces of certain color
void GetAllPieces (game_state_t * game, color_t color, piece_t * array, int * cnt_pieces);


//TODO remove, not needed for chess
//ListNode * GetSuccessiveCapturesFromMove (game_state_t * game, move_t * baseMove);

//get possible moves for 1 piece in game.
ListNode * GetMovesForPiece (game_state_t * game, piece_t piece);

//get all the possible moves for player in game.
//updates list moves.
//basic algorithm:
//1. iterate on all pieces in game.
//2. for each piece , run GetMovesForPiece and collect the moves.
//3. optional - sort moves by number of captures.
ListNode * GetMovesForPlayer (game_state_t * game, color_t color);


//will perform the move pointed by "move"
//is responsible for updating the game state afterwards:
//1. capturing opponents
//2. promoting to king
void DoMove (move_t * move, game_state_t * game);

//find the move in allowed moves list (will update it's captures)
int FindMoveInList (ListNode * moves, move_t * mymove);

//will clear all the pieces in between two positions
void ClearPiecesInBetween (game_state_t * game, position_t src, position_t dest);

#endif /* EX3_GAME_H_ */
