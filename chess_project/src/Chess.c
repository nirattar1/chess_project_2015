#include "Chess.h"
#include "Game.h"
#include "Memory.h"
#include "Console_ui.h"
#include "Minimax.h"


#ifdef TESTING
	#include "tests/Test_Main.h"
#endif


//default settings initialization.

color_t SETTINGS_USER_COLOR = DEFAULT_USER_COLOR;
int SETTINGS_MAX_DEPTH = DEFAULT_MAX_DEPTH;



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

	//start console ui
	Menu_Settings(&game, (char **) board);

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


//return number if given color has won
//if 	0 : nobody won (continue play).
//		1 : given color has won.
int GameWinning(game_state_t * game, color_t color)
{

	if (DraughtsScoringFunction(game, color)==SCORE_WIN_PLAYER)
	{
		return 1;
	}
	return 0;
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
				DraughtsScoringFunction, GetMovesForPlayer,
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

	//white always start
	color_t current_player = COLOR_WHITE;

	//print board first time
	PrintBoard(game);

	//play until somebody won
	while (!GameWinning(game,COLOR_BLACK) && !GameWinning(game,COLOR_WHITE))
	{
		if (Settings_UserColor_Get()==current_player)
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
		current_player = (current_player==COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
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
void MovesListPrint( LINK head )
{
	for ( ; head !=0; head = head->next )
	{

		move_t * move = (move_t *) head->data;
		//print move.
		printf ("move: source : (%c , %d) ", move->src.x, move->src.y);

		position_t * dest = move->dest;
		printf ("destinations: ");
		printf ("(%c , %d) ", dest[0].x, dest[0].y);
		if (move->num_captures>0)
		{
			printf("(CAPTURE) ");
		}
		for (int i=1; i<=(move->num_captures-1); i++)
		{
			printf ("(%c , %d) (CAPTURE) ", dest[i].x, dest[i].y);
		}

		if (move->num_captures>0)
		{
			printf("total %d captures.", move->num_captures);
		}

		printf ("\n");
	}
}

