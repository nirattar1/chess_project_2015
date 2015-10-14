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

//defines the 'last row' for the 2 colors (used for promotion)
#define LAST_ROW_WHITE BOARD_SIZE
#define LAST_ROW_BLACK 1

#define MAX_PIECES_PLAYER 16


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

//will return the color (black/white) of the given identity.
color_t GetColor (char identity);

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
direction_t allowed_directions_array_of_all [17];

#define PAWN_PROMOTION_OPTIONS_SIZE 4
char pawn_promotion_options [PAWN_PROMOTION_OPTIONS_SIZE];

//fills values of all_allowed_directions based on rules.
void RulesInit ();


//will receive identity and return its allowed directions.
direction_t * GetPieceDirections (char identity);

//return whether given identity can do a CAPTURE move in given direction.
int IsDirectionAllowedForPieceCapture(char identity, direction_t direction);

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

//returns whether pieces in 2 positions are in same color.
//1 = both pieces in positions exist and same color
//0 = different colors or at least one is empty.
int SameColor(game_state_t * game, position_t pos1, position_t pos2);

//will return 1 if piece is man (pawn), 0 otherwise.
int IsMan (piece_t piece);
//will return 1 if piece is bishop, 0 otherwise.
int IsBishop (piece_t piece);
//will return 1 if piece is knight, 0 otherwise.
int IsKnight (piece_t piece);
//will return 1 if piece is rook, 0 otherwise.
int IsRook (piece_t piece);
//will return 1 if piece is queen, 0 otherwise.
int IsQueen (piece_t piece);
//will return 1 if piece is king, 0 otherwise.
int IsKing (piece_t piece);


//will initialize the game
//(clears board to empty layout)
void GameInit (game_state_t * game, char ** board );

//will initialize the game
void GameDefaultLayout (game_state_t * game);


//did someone win game
int GameWinning(game_state_t * game, color_t color);

//error code for valid board status
typedef enum
{
	BOARD_VALIDITY_OK,
	BOARD_VALIDITY_TOO_MANY_PIECES_OF_TYPE,
	BOARD_VALIDITY_KING_IS_MISSING
} board_validation_status_t;
//will return whether this board is valid, or else an appropriate error code.
board_validation_status_t IsValidBoard (game_state_t * game);


//checks if it is valid to set piece of certain identity and position, to game.
//returns 1 if valid, 0 if not valid.
int IsValidPieceAddition (game_state_t * game, position_t position, char identity);

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

//checks if move represented by 2 positions is valid.
//performs all the checks of IsValidCapture, and also:
//1. checks that piece in source is allowed to move in the direction.
//2. checks that distance is ok.
//! assumes that there are no pieces in between.
int IsValidCaptureMoreChecks(game_state_t * game, position_t source, position_t newDest, int distance);

//returns whether the hop from piece to dest, is worth promotion.
int IsPromotionMove (piece_t piece, position_t dest);


//maximum captures in 1 move (if eat all opponents).
#define MAX_CAPTURES_MOVE 20

//struct that holds information about one move of piece on board.
//the move can contain several destinations on the way (in case of successive captures)
typedef struct
{
	//source piece on board.
	position_t 	src;
	//destination positions .
	position_t 	dest;
	//the number of captures in this move (can be 0 or 1)
	int 		num_captures;
	//if this move deserves promotion, this saves the ideneity to promote to.
	//0 default (no promotion)
	char		promote_to_identity;
} move_t;


//generates a new move.
//from position src to position dest.
//(allocates and returns)
move_t * MoveCreateWithOptions (position_t src, position_t dest, int is_capture, char promote_to_identity);
move_t * MoveCreate (position_t src, position_t dest);

//frees a move
void MoveFree( void * data );

//copy contents on move "m1", to move "m2".
//currently - just shallow copy (move struct doesn't have any pointers).
void MoveCopy( move_t * m2, move_t * m1 );

//get all the pieces of certain color
void GetAllPieces (game_state_t * game, color_t color, piece_t * array, int * cnt_pieces);


//get possible moves for 1 piece in game.
//get all the possible moves for player 'color' in game.
//will generate and return a list of the moves.
//basic algorithm:
//1. iterate on all pieces of the player in the game.
//2. for each piece, run GetMovesForPiece and collect the moves.
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

//find a move in given moves list, based on:
//source and destination positions, and also promotion identity.
//if found the move, returns 1 and updates move_return.
//otherwise returns 0.
int FindMoveInList 	(ListNode * moves, position_t src, position_t dest,
		char promotion_identity, move_t * move_return, int * move_index);

//updates the position of color's king on board (by reference).
//if king is not found returns 0.
//otherwise returns 1.
int GetKingPosition (game_state_t * game, color_t color, position_t * pos);

//return whether there is a 'check' state against player "color"
//(if player's king is threatened).
int IsCheckState (game_state_t * game, color_t color);


//will check if the following move from player "color", will reveal it's king
//(lead to check state).
//if so will return 1 , otherwise 0.
int IsMoveRevealingKing (game_state_t * state, color_t player, move_t * move);


//an enum for the status of the game.
//(such as- continue play, check, or end games)
typedef enum {
	STATUS_CONTINUE_PLAY,
	STATUS_TIE,
	STATUS_PLAYER_IN_CHECK,
	STATUS_OPPONENT_IN_CHECK,
	STATUS_PLAYER_IN_CHECKMATE,
	STATUS_OPPONENT_IN_CHECKMATE
} play_status_t;


//will return a play status, based on game state.
play_status_t GetPlayStatus (game_state_t * game, color_t current_player);

#endif /* EX3_GAME_H_ */
