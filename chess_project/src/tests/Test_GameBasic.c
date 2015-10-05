/*
 * GameTest.c
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include "../Chess.h"
#include "../Game.h"
#include "../Memory.h"

void Test_PiecesDirections (game_state_t * game);
void GetMovesBasicTest(game_state_t * game);
void GetMoves1Capture(game_state_t * game);
void Test_Pawn_Promotion_1(game_state_t * game);
void Test_Pawn_Promotion_2(game_state_t * game);
void DoMoveTest_1(game_state_t * game);
void DoMoveTest_2_capture(game_state_t * game);
void DoMoveTest_3_promotions(game_state_t * game);
void Test_Kings(game_state_t * game);
void Test_GetAllPieces(game_state_t * game);
void Test_GetAllPiecesMoves_1(game_state_t * game);
void Test_GetAllPiecesMoves_2(game_state_t * game);
void Test_ScoringFunction(game_state_t * game);
static void MovesListPerform( LINK head , game_state_t * game);


void GameTest ()
{
	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];


	//GameDefaultLayout(&game);

	GameInit(&game, (char **)board);
	Test_PiecesDirections(&game);

	GameInit(&game, (char **)board);
	GetMovesBasicTest(&game);

	GameInit(&game, (char **)board);
	GetMoves1Capture(& game);

	GameInit(&game, (char **)board);
	Test_GetAllPieces(&game);

	GameInit(&game, (char **)board);
	Test_GetAllPiecesMoves_1(&game);

	GameInit(&game, (char **)board);
	Test_GetAllPiecesMoves_2(&game);

	GameInit(&game, (char **)board);
	Test_Pawn_Promotion_1 (&game);

	GameInit(&game, (char **)board);
	Test_Pawn_Promotion_2 (&game);

	GameInit(&game, (char **)board);
	DoMoveTest_1(&game);

	GameInit(&game, (char **)board);
	DoMoveTest_2_capture(&game);

	GameInit(&game, (char **)board);
	DoMoveTest_3_promotions(&game);


//	GameInit(&game, (char **)board);
//	Test_ScoringFunction(&game);
//
//	GameInit(&game, (char **)board);
//	Test_Kings(&game);



	memory_print();
}


void Test_PiecesDirections (game_state_t * game)
{

	//print empty board
	PrintBoard(game);

	//put all of them on board.
	SetPiece(Position ('h', 2), WHITE_M, game);
	SetPiece(Position ('h', 3), WHITE_B, game);
	SetPiece(Position ('h', 4), WHITE_N, game);
	SetPiece(Position ('h', 5), WHITE_R, game);
	SetPiece(Position ('h', 6), WHITE_Q, game);
	SetPiece(Position ('h', 7), WHITE_K, game);
	SetPiece(Position ('a', 8), BLACK_M, game);
	SetPiece(Position ('b', 7), BLACK_B, game);
	SetPiece(Position ('c', 6), BLACK_N, game);
	SetPiece(Position ('d', 5), BLACK_R, game);
	SetPiece(Position ('e', 4), BLACK_Q, game);
	SetPiece(Position ('f', 3), BLACK_K, game);
	PrintBoard(game);

	//get piece allowed directions.
	char piece = GetPiece(Position ('c',6), game);
	direction_t * dirs = GetPieceDirections(piece);
	printf ("for piece %c: ", piece);
	while (*dirs != 0)
	{
		printf ("allowed direction : %d ,", *dirs);
		dirs++;
	}
	printf ("\n");

}




void GetMovesBasicRook()
{

}

//get moves for all pieces types
void GetMovesBasicTest(game_state_t * game)
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

	pos = Position ('g', 3);
	identity = WHITE_K;
	SetPiece(pos, identity, game);
	piece_t piece2 ;
	piece2.identity = identity;
	piece2.position = pos;


	PrintBoard(game);


	ListNode * list1 = GetMovesForPiece(game, piece1);

	ListNode * list2 = GetMovesForPiece(game, piece2);

	MovesListPrint( list1);

	MovesListPrint( list2);

	ListFreeElements(list1, MoveFree);

	ListFreeElements(list2, MoveFree);

}



void GetMoves1Capture(game_state_t * game)
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

	PrintBoard(game);

	ListNode * list1 = GetMovesForPiece(game, piece1);

	ListNode * list2 = GetMovesForPiece(game, piece2);

	MovesListPrint( list1);

	MovesListPrint( list2);

	ListFreeElements(list1, MoveFree);

	ListFreeElements(list2, MoveFree);
}





//will test promotion of pawn.
//pawn has 4 options - queen, bishop, rook, knight.
void Test_Pawn_Promotion_1(game_state_t * game)
{

	//start with 1 pawn approaching end

	SetPiece(Position('a', 7), WHITE_M, game);
	PrintBoard(game);

	//get the moves. they should contain all the 4 options for pawn.
	piece_t piece; 	piece.position = Position('a', 7); piece.identity = WHITE_M;
	ListNode * list1 = GetMovesForPiece(game, piece);

	MovesListPrint( list1);

	ListFreeElements(list1, MoveFree);
}


//will test promotion of pawn.
//this time with capture and other color
void Test_Pawn_Promotion_2(game_state_t * game)
{

	//start with 1 pawn approaching end

	SetPiece(Position('d', 2), BLACK_M, game);
	SetPiece(Position('e', 1), WHITE_B, game);
	PrintBoard(game);

	//get the moves. they should contain all the 4 options for pawn.
	piece_t piece1; 	piece1.position = Position('d', 2); piece1.identity = BLACK_M;
	ListNode * list1 = GetMovesForPiece(game, piece1);
	piece_t piece2; 	piece2.position = Position('e', 1); piece2.identity = WHITE_B;
	ListNode * list2 = GetMovesForPiece(game, piece2);
	MovesListPrint(list1);
	MovesListPrint(list2);
	ListFreeElements(list1, MoveFree);
	ListFreeElements(list2, MoveFree);


}

//test method perform the moves one after the other
static void MovesListPerform( LINK head , game_state_t * game)
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
void Test_GetAllPieces(game_state_t * game)
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
	identity = WHITE_M;
	SetPiece(pos, identity, game);
	piece_t piece3 ;
	piece3.identity = identity;
	piece3.position = pos;

	//b,2
	pos = Position ('b', 2);
	identity = WHITE_M;
	SetPiece(pos, identity, game);

	PrintBoard(game);

	color_t color;
	color = COLOR_BLACK;
	int cnt_black;
	piece_t pieces_black [20]; //player can have at least 20 moves.
	GetAllPieces (game, color, pieces_black, &cnt_black);


	color = COLOR_WHITE;
	int cnt_white;
	piece_t pieces_white [20]; //player can have at least 20 moves.
	GetAllPieces (game, color, pieces_white, &cnt_white);


}



//get all moves for color.
void Test_GetAllPiecesMoves_1(game_state_t * game)
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
	identity = WHITE_M;
	SetPiece(pos, identity, game);
	piece_t piece3 ;
	piece3.identity = identity;
	piece3.position = pos;

	//b,2
	pos = Position ('b', 2);
	identity = WHITE_M;
	SetPiece(pos, identity, game);

	PrintBoard(game);

	color_t color;
	color = COLOR_BLACK;
	ListNode * moves_black = GetMovesForPlayer(game, color);

	color = COLOR_WHITE;
	ListNode * moves_white = GetMovesForPlayer(game, color);


	printf("\n black's moves:\n");
	MovesListPrint( moves_black);

	printf("\n white's moves:\n");
	MovesListPrint( moves_white);

	//free list of moves.
	ListFreeElements(moves_black, MoveFree);

	ListFreeElements(moves_white, MoveFree);

}


//get all pieces moves - more pieces
void Test_GetAllPiecesMoves_2(game_state_t * game)
{

	//put all of them on board.
	SetPiece(Position ('h', 2), WHITE_M, game);
	SetPiece(Position ('h', 3), WHITE_B, game);
	SetPiece(Position ('h', 4), WHITE_N, game);
	SetPiece(Position ('h', 5), WHITE_R, game);
	SetPiece(Position ('h', 6), WHITE_Q, game);
	SetPiece(Position ('h', 7), WHITE_K, game);
	SetPiece(Position ('a', 8), BLACK_M, game);
	SetPiece(Position ('b', 7), BLACK_B, game);
	SetPiece(Position ('c', 6), BLACK_N, game);
	SetPiece(Position ('d', 5), BLACK_R, game);
	SetPiece(Position ('e', 4), BLACK_Q, game);
	SetPiece(Position ('f', 3), BLACK_K, game);

	PrintBoard(game);

	color_t color;
	color = COLOR_BLACK;
	ListNode * moves_black = GetMovesForPlayer(game, color);

	color = COLOR_WHITE;
	ListNode * moves_white = GetMovesForPlayer(game, color);


	printf("\n black's moves:\n");
	MovesListPrint( moves_black);

	printf("\n white's moves:\n");
	MovesListPrint( moves_white);

	//free list of moves.
	ListFreeElements(moves_black, MoveFree);

	ListFreeElements(moves_white, MoveFree);

}

//do move - move 1 piece (simple move).
void DoMoveTest_1(game_state_t * game)
{

	//position
	// (h, 2)
	position_t pos = Position ('a', 2);
	//put something on board.
	SetPiece(pos, WHITE_R, game);
	PrintBoard(game);

	//generate move
	move_t * move1 = (move_t *) mymalloc(sizeof (move_t));
	move1->src = pos;
	move1->num_captures = 0;
	move1->dest[0] = Position('a',5);

	//do 1 move.
	DoMove(move1, game);
	PrintBoard(game);

	myfree(move1);

}


//do move
//1. select move from get_moves list.
//2. do 1 simple move, 1 capture move.
void DoMoveTest_2_capture(game_state_t * game)
{

	position_t pos1 = Position ('e', 5);
	SetPiece(pos1, WHITE_B, game);
	position_t pos2 = Position ('f', 7);
	SetPiece(pos2, BLACK_N, game);

	PrintBoard(game);

	color_t color;
	color = COLOR_BLACK;
	ListNode * moves_black = GetMovesForPlayer(game, color);

	color = COLOR_WHITE;
	ListNode * moves_white = GetMovesForPlayer(game, color);

	printf("\n black's moves:\n");
	MovesListPrint( moves_black);

	printf("\n white's moves:\n");
	MovesListPrint( moves_white);


	//do 1 simple move (bishop)

	PrintBoard(game);

	//free list of moves.
	ListFreeElements(moves_black, MoveFree);

	ListFreeElements(moves_white, MoveFree);


	color = COLOR_BLACK;
	moves_black = GetMovesForPlayer(game, color);

	color = COLOR_WHITE;
	moves_white = GetMovesForPlayer(game, color);

	printf("\n black's moves:\n");
	MovesListPrint( moves_black);

	printf("\n white's moves:\n");
	MovesListPrint( moves_white);

	//do 1 capture move (knight captures bishop)

	//free list of moves.
	ListFreeElements(moves_black, MoveFree);

	ListFreeElements(moves_white, MoveFree);


}

void DoMoveTest_3_promotions(game_state_t * game)
{
;
}

//get pieces
void Test_ScoringFunction(game_state_t * game)
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


	int score = DraughtsScoringFunction(game, COLOR_BLACK);
	printf("score for black: %d\n", score);

	score = DraughtsScoringFunction(game, COLOR_WHITE);
	printf("score for white: %d\n", score);

}


void Test_Kings(game_state_t * game)
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
	identity = BLACK_K;
	SetPiece(pos, identity, game);

	PrintBoard(game);


	color_t color;
	color = COLOR_BLACK;
	ListNode * moves_black = GetMovesForPlayer(game, color);

	color = COLOR_WHITE;
	ListNode * moves_white = GetMovesForPlayer(game, color);



	printf("\n black's moves:\n");
	MovesListPrint( moves_black);

	printf("\n white's moves:\n");
	MovesListPrint( moves_white);

	//free list of moves.
	ListFreeElements(moves_black, MoveFree);

	ListFreeElements(moves_white, MoveFree);


}

