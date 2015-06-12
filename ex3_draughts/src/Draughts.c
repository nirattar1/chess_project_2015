#include "Draughts.h"
#include "Game.h"





//contains main



int  ListTest ();
int  GameTest();
int original_main();


int main ()
{
	//ListTest ();

	GameTest ();

	//original_main();

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




