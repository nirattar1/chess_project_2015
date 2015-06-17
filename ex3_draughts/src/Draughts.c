#include "Draughts.h"
#include "Game.h"
#include "Memory.h"

//default settings initialization.

color_t SETTINGS_USER_COLOR = DEFAULT_USER_COLOR;
int SETTINGS_MAX_DEPTH = DEFAULT_MAX_DEPTH;


//contains main



int ListTest ();
int GameTest();
void Test_Minimax();
void Test_GameWithMinimax();
int original_main();


int main ()
{

	RulesInit();

	//ListTest ();

	//GameTest ();

	//original_main();

	//Test_Minimax();


	Test_GameWithMinimax();

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
