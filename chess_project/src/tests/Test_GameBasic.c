/*
 * GameTest.c
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include "../Game.h"
#include "../Draughts.h"
#include "../Memory.h"

void BasicTest (game_state_t * game);
void MoveTest(game_state_t * game);
void MoveListTest(game_state_t * game);
void GetMovesBasicTest(game_state_t * game);
void GetMoves1Capture(game_state_t * game);
void Test_CaptureBackwards(game_state_t * game);
void Test_Kings(game_state_t * game);

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

	GameInit(&game, (char **)board);
	GetMovesBasicTest(&game);

	GameInit(&game, (char **)board);
	GetMoves1Capture(& game);

	GameInit(&game, (char **)board);
	GetMoves1SuccessiveCapture(& game);

	GameInit(&game, (char **)board);
	Test_GetAllPieces(&game);

	GameInit(&game, (char **)board);
	Test_GetAllPiecesMoves(&game);


	GameInit(&game, (char **)board);
	Test_CaptureBackwards(&game);

	GameInit(&game, (char **)board);
	Test_ScoringFunction(&game);

	GameInit(&game, (char **)board);
	Test_Kings(&game);
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
	move1->dest[0] = Position('g',3);


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
	moves[0]->dest[0] = Position('a',5);

	moves[1] = (move_t *) mymalloc(sizeof (move_t));
	moves[1]->src = moves[0]->dest[0];
	moves[1]->num_captures = 0;
	moves[1]->dest[0] = Position('b',6);

	moves[2] = (move_t *) mymalloc(sizeof (move_t));
	moves[2]->src = moves[1]->dest[0];
	moves[2]->num_captures = 0;
	moves[2]->dest[0] = Position('c',7);


	//prepare a list for the moves.

	ListNode * list = NULL;
	//loop on array and add to list
	for (int i=0; i<NUM_TEST_MOVES; i++)
	{
		//point to existing data
		move_t * pMove = moves[i];

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


void GetMoves1SuccessiveCapture(game_state_t * game)
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

	ListNode * list1 = GetMovesForPiece(game, piece1);

	ListNode * list2 = GetMovesForPiece(game, piece2);

	ListNode * list3 = GetMovesForPiece(game, piece3);


	MovesListPrint( list1);

	MovesListPrint( list2);

	MovesListPrint( list3);

	ListFreeElements(list1, MoveFree);

	ListFreeElements(list2, MoveFree);

	ListFreeElements(list3, MoveFree);


}





//
////prints moves one after the other
//void MovesListPrint( LINK head , game_state_t * game)
//{
//	for ( ; head !=0; head = head->next )
//	{
//
//		move_t * move = (move_t *) head->data;
//		//print move.
//		printf ("move: source : (%c , %d) ", move->src.x, move->src.y);
//
//		position_t * dest = move->dest;
//		printf ("destinations: ");
//		printf ("(%c , %d) ", dest[0].x, dest[0].y);
//		if (move->num_captures>0)
//		{
//			printf("(CAPTURE) ");
//		}
//		for (int i=1; i<=(move->num_captures-1); i++)
//		{
//			printf ("(%c , %d) (CAPTURE) ", dest[i].x, dest[i].y);
//		}
//
//		if (move->num_captures>0)
//		{
//			printf("total %d captures.", move->num_captures);
//		}
//
//		printf ("\n");
//	}
//}


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

//get pieces
void Test_GetAllPiecesMoves(game_state_t * game)
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


void Test_CaptureBackwards(game_state_t * game)
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

	pos = Position ('d', 6);
	identity = WHITE_M;
	SetPiece(pos, identity, game);
	piece_t piece4 ;
	piece4.identity = identity;
	piece4.position = pos;


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
