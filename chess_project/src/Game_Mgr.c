#include "Game_Mgr.h"
#include "Game.h"
#include "Memory.h"
#include "Console_ui.h"
#include "Minimax.h"
#include <string.h>

//TODO remove this include
#include "tests/Test_Gui_framework.h"

#ifdef TESTING
	#include "tests/Test_Main.h"
#endif


//default settings initialization.
int			SETTINGS_GAME_MODE		= DEFAULT_GAME_MODE;
color_t 	SETTINGS_USER_COLOR 	= DEFAULT_USER_COLOR;
color_t		SETTINGS_NEXT_PLAYER 	= DEFAULT_NEXT_PLAYER;
int 		SETTINGS_MAX_DEPTH 		= DEFAULT_MAX_DEPTH;




int main (int argc, char * argv[])
{

	//on testing, execute different main.
#ifdef TESTING
	Test_Main();
	exit (0);
#endif

	//init game rules
	RulesInit();

	//initialize game state.
	//(empty layout)
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];

	//clear board, put default layout
	GameInit(&game, (char **) board);
	GameDefaultLayout(&game);

	//determine wanted game mode from argument
	int is_gui = 0; //default=console
	if (argc==2)
	{
		if (strcmp(argv[1], "gui")==0)
		{
			is_gui = 1;
		}
		else if (strcmp(argv[1], "console")==0)
		{
			is_gui = 0;
		}
		else
		{
			//invalid game mode
		}
	}

	//start game in correct ui
	if (is_gui)
	{
		//start GUI
		Test_Gui_framework();
	}
	else
	{
		//start console ui
		Menu_Settings(&game, (char **) board);
	}

	memory_print();

	return 0;
}



//int original_main()
//{
//	char board[BOARD_SIZE][BOARD_SIZE];
//	init_board(board);
//	print_board(board);
//	print_message(WRONG_MINIMAX_DEPTH);
//	perror_message("TEST");
//	return 0;
//}


void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE*4; i++){
		printf("-");
	}
	printf("|\n");
}

void PrintBoard(game_state_t * game)
{

	int i,j;

	print_line();
	for (j = BOARD_SIZE-1; j >= 0 ; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j+1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", game->pieces[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

//get/set game mode.
int Settings_GameMode_Get()
{
	return SETTINGS_GAME_MODE;
}

void Settings_GameMode_Set(int mode)
{
	SETTINGS_GAME_MODE = mode;
}

//get/set next player.
color_t Settings_NextPlayer_Get()
{
	return SETTINGS_NEXT_PLAYER;
}

void Settings_NextPlayer_Set(color_t player)
{
	SETTINGS_NEXT_PLAYER = player;
}


//get color of user
color_t Settings_UserColor_Get()
{
	return SETTINGS_USER_COLOR;
}

//set color of user
void Settings_UserColor_Set(color_t color)
{
	SETTINGS_USER_COLOR = color;
}



//get color of CPU - opposite of user's color.
color_t Settings_CPUColor_Get()
{

	color_t user_color = Settings_UserColor_Get();

	return GetOppositeColor(user_color);

}

//get/set max depth for minimax.
int Settings_MaxDepth_Get()
{
	return SETTINGS_MAX_DEPTH;
}

void Settings_MaxDepth_Set(int max_depth)
{
	SETTINGS_MAX_DEPTH = max_depth;
}

//TODO remove this function
//return number if given color has won
//if 	0 : nobody won (continue play).
//		1 : given color has won.
int GameWinning(game_state_t * game, color_t color)
{


	return 0;
}


//scoring function to use with minimax.
//based on maximizing player color and game state.
//evaluates the state for the given player.
//(gives positive score to the maximizing player and negative to the minimizing player.)
int BasicScoringFunction (game_state_t * game, color_t maximizing_player, color_t current_player)
{

	int player_score=0, opposite_score=0;
	int current_player_is_maximizing = (current_player==maximizing_player);

	//get player's pieces
	int cntPiecesPlayer;//will count how many pieces player has.
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, maximizing_player, piecesPlayer, &cntPiecesPlayer);

	//get opposite color's pieces
	int cntPiecesOpposite;//will count how many pieces player has.
	piece_t piecesOpposite [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, GetOppositeColor(maximizing_player), piecesOpposite, &cntPiecesOpposite);


	//determine end game.
	play_status_t status = GetPlayStatus(game, current_player);

	//win for current player
	if (status==STATUS_OPPONENT_IN_CHECKMATE) //player wins
	{
		if (current_player_is_maximizing)
		{
			return 1000;
		}
		else
		{
			return -1000;
		}
	}
	else if (status==STATUS_PLAYER_IN_CHECKMATE)	//player loses
	{
		if (current_player_is_maximizing)
		{
			return -1000;
		}
		else
		{
			return 1000;
		}
	}
	else if (status==STATUS_TIE)	//either players has no moves
	{
		if (current_player_is_maximizing)
		{
			return -800;
		}
		else
		{
			return 800;
		}
	}

	else
	{
		//in any other case, count pieces to compute score
		//iterate and add on both sides.
		for (int i=0; i<cntPiecesPlayer; i++)
		{
			if (IsMan (piecesPlayer[i]))
			{
				player_score += 1;
			}
			else if (IsKnight (piecesPlayer[i]) )
			{
				player_score += 3;
			}
			else if (IsBishop (piecesPlayer[i]) )
			{
				player_score += 3;
			}
			else if (IsRook (piecesPlayer[i]) )
			{
				player_score += 5;
			}
			else if (IsQueen (piecesPlayer[i]) )
			{
				player_score += 9;
			}
			else if (IsKing (piecesPlayer[i]) )
			{
				player_score += 400;
			}
		}

		//update opposite score
		for (int i=0; i<cntPiecesOpposite; i++)
		{
			if (IsMan (piecesOpposite[i]))
			{
				opposite_score += 1;
			}
			else if (IsKnight (piecesOpposite[i]) )
			{
				opposite_score += 3;
			}
			else if (IsBishop (piecesOpposite[i]) )
			{
				opposite_score += 3;
			}
			else if (IsRook (piecesOpposite[i]) )
			{
				opposite_score += 5;
			}
			else if (IsQueen (piecesOpposite[i]) )
			{
				opposite_score += 9;
			}
			else if (IsKing (piecesOpposite[i]) )
			{
				opposite_score += 400;
			}
		}

		//return the difference between player's and opponent's score
		return ( player_score - opposite_score ) ;
	}
}



//one turn of the CPU.
void CPUTurn (game_state_t * game)
{
	//get CPU's color
	color_t color = Settings_CPUColor_Get();


	//choose the next move based on minimax:

	//get max depth based on settings
	int max_depth = Settings_MaxDepth_Get();

	//get initial children list from given state.
	//(allowed moves for CPU.)
	ListNode * RootChildren = GetMovesForPlayer(game, color);

	//child with best score will decide what move to do .
	int childIndex;
	int childScore;

	MinimaxChoose (game,  RootChildren, 0, max_depth,
				1, MIN_SCORE, MAX_SCORE,	//pruning=true
				color,1,					//define CPU color as maximizing, start as maximizer
				BasicScoringFunction, GetMovesForPlayer,
				&childIndex, &childScore);

	DEBUG_PRINT( ("index %d was chosen. will lead to score of %d\n", childIndex, childScore));

	//do the relevant move
	int i=0;
	//find index in list
	ListNode * pChild = RootChildren;
	while (i<childIndex && pChild!=NULL )
	{
		pChild = pChild->next;
		i++;
	}

	//do the move
	if (pChild)
	{
		move_t * move = (move_t *) (pChild->data);
		DoMove(move, game);

		//print
		//"Computer: move <x,y> to <i,j>[<k,l>...]\n".
		printf("Computer: move <%c,%d> to <%c,%d>",
				move->src.x, move->src.y, move->dest[0].x, move->dest[0].y);
		printf("\n");

	}

}


void UserTurn (game_state_t * game)
{
	move_t userMove = Menu_PlayUser(game);
	DoMove(&userMove, game);
}

void DoGame(game_state_t * game)
{

	//print board first time
	PrintBoard(game);

	//play until somebody won
	while (!GameWinning(game,COLOR_BLACK) && !GameWinning(game,COLOR_WHITE))
	{
		//do turn of next player.
		color_t next_player = Settings_NextPlayer_Get();
		if (next_player==Settings_UserColor_Get())
		{
			UserTurn (game);
		}
		else
		{
			CPUTurn(game);
		}

		//print board after turn
		PrintBoard(game);

		//switch player
		Settings_NextPlayer_Set(GetOppositeColor(next_player));
	}

	//print winning side
	if (GameWinning(game,COLOR_BLACK))
	{
		printf("black player wins!\n");
	}
	else
	{
		printf("white player wins!\n");
	}
}




//prints moves one after the other
//TODO make according to format
void MovesListPrint( LINK head )
{
	for ( ; head !=0; head = head->next )
	{
		//iterate on moves in list
		move_t * move = (move_t *) head->data;
		//print each move
		MovePrint (move);
		printf ("\n");
	}
}

//will print the move
void MovePrint (move_t * move)
{
	//print move src.
	printf ("<%c,%d>", move->src.x, move->src.y);

	position_t * dest = move->dest;
	printf (" to ");
	//print move destination.
	printf ("<%c,%d>", dest[0].x, dest[0].y);

	if (move->promote_to_identity != 0)
	{
		printf (" %s", GetIdentityName(move->promote_to_identity));
	}

	if (move->num_captures>0)
	{
		DEBUG_PRINT((" (CAPTURE)"));
	}

}


char * GetIdentityName (char identity)
{
	//TODO enter all .
	//TODO another function for set command. - maybe can unify.
	if (identity==WHITE_Q || identity==BLACK_Q)
	{
		return "queen";
	}
	if (identity==WHITE_B || identity==BLACK_B)
	{
		return "bishop";
	}
	if (identity==WHITE_R || identity==BLACK_R)
	{
		return "rook";
	}
	if (identity==WHITE_N || identity==BLACK_N)
	{
		return "knight";
	}
	return NULL;
}
