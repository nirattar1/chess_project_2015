#include "Game_Mgr.h"
#include "Game.h"
#include "Memory.h"
#include "Console_ui.h"
#include "Gui.h"
#include "Minimax.h"
#include <string.h>
#include <math.h>
#include <sys/stat.h>

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
int 		_RunModeIsGui 			= 0; //default=console
game_mgr_handlers_t _GameMgrHandlers;



game_mgr_handlers_t GetGameMgrHandlers (int runModeIsGui)
{
	game_mgr_handlers_t handlers;

	//TODO all handlers should be not null gui handlers.
	//TODO all calls to handlers should check they're not null.

	if (runModeIsGui)
	{
		handlers.DisplayUpdatedBoardFunc = Gui_UpdateBoard;
		handlers.SelectUserMoveFunc = Gui_SelectUserMove;
		handlers.HandleCheck = Gui_HandleCheck;
		handlers.HandleEnd = Gui_HandleEnd;
	}

	else
	{
		//default - console UI.
		handlers.DisplayUpdatedBoardFunc = PrintBoard;
		handlers.SelectUserMoveFunc = Menu_PlayUser;
		handlers.HandleCheck = ConsoleUI_GameHandleCheck;
		handlers.HandleEnd = ConsoleUI_GameHandleEnd;
	}

	return handlers;
}


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
	//board is saved on stack at top level.
	game_state_t game;
	char board [BOARD_SIZE][BOARD_SIZE];

	//clear board, put default layout
	GameInit(&game, (char **) board);
	GameDefaultLayout(&game);

	//determine wanted run mode from argument

	if (argc==2)
	{
		if (strcmp(argv[1], "gui")==0)
		{
			_RunModeIsGui = 1;
		}
		else if (strcmp(argv[1], "console")==0)
		{
			_RunModeIsGui = 0;
		}
		else
		{
			//invalid run mode
		}
	}

	//register handlers based on game mode.
	_GameMgrHandlers = GetGameMgrHandlers (_RunModeIsGui);


	//start game in correct ui
	if (_RunModeIsGui)
	{
		//start GUI
		Gui_Main (&game);
	}
	else
	{
		//start console ui
		Menu_Settings(&game, (char **) board);
	}

	memory_print();

	return 0;
}



int DoesFileExist(const char *filename)
{
    struct stat st;
    int result = stat(filename, &st);
    return (result == 0);
}

int FileCanOpenForWriting (const char * filename)
{
	FILE * pFile;
	pFile = fopen (filename,"w");
	if (pFile!=NULL)
	{
		fclose (pFile);
		return 1;
	}
	return 0;
}

void print_line(){
	int i;
	printf(" |");
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
		printf((j < 9 ? "%d" : "%d"), j+1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", game->pieces[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("  ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}


void Settings_ResetDefaults()
{
	Settings_GameMode_Set(DEFAULT_GAME_MODE);
	Settings_MaxDepth_Set(DEFAULT_MAX_DEPTH);
	Settings_NextPlayer_Set(DEFAULT_NEXT_PLAYER);
	Settings_UserColor_Set(DEFAULT_USER_COLOR);
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


int BestDepthCompute (game_state_t * game, color_t current_player)
{

	int cntPiecesPlayer;//will count how many pieces player has.
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //player can have at most 20 pieces.
	GetAllPieces (game, current_player, piecesPlayer, &cntPiecesPlayer);

	//get the number of possible moves (upper bound) based on the current player.
	//this number is the branching factor b for the tree.
	int possible_moves_per_level = 0;
	//loop on all pieces, each piece has a constant number of possible moves
	for (int i=0; i<cntPiecesPlayer; i++)
	{
		if (IsMan (piecesPlayer[i]))
		{
			//mean value between 3 (pawns moves) and 27 (queen's moves)
			//assuming that pawn promoted to a queen sometime
			possible_moves_per_level += 15;
		}
		else if (IsKnight (piecesPlayer[i]) )
		{
			possible_moves_per_level += 8;
		}
		else if (IsBishop (piecesPlayer[i]) )
		{
			possible_moves_per_level += 13;
		}
		else if (IsRook (piecesPlayer[i]) )
		{
			possible_moves_per_level += 14;
		}
		else if (IsQueen (piecesPlayer[i]) )
		{
			possible_moves_per_level += 27;
		}
		else if (IsKing (piecesPlayer[i]) )
		{
			possible_moves_per_level += 8;
		}
	}


	//compute the depth so that the number of leaves is less than 10^6
	//depth = logarithm of 10^6 base b = log(10^6)/log(b)
	double depth = (log(pow(10,6)))/(log(possible_moves_per_level));

	//cast the result into int
	int dInt = (int) floor(depth);

	DEBUG_PRINT(("Computed best depth of : %d \n", dInt));

	return dInt;
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
	piece_t piecesPlayer [MAX_PIECES_PLAYER]; //player can have at most 16 pieces.
	GetAllPieces (game, maximizing_player, piecesPlayer, &cntPiecesPlayer);

	//get opposite color's pieces
	int cntPiecesOpposite;//will count how many pieces player has.
	piece_t piecesOpposite [MAX_PIECES_PLAYER]; //player can have at most 16 pieces.
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

	//determine depth.
	//get settings for max depth
	int max_depth = Settings_MaxDepth_Get();

	//if user has configured "best" option, we need to compute the real depth now.
	if (max_depth == MAX_DEPTH_BEST_VALUE)
	{
		max_depth = BestDepthCompute(game, color);
	}
	//otherwise stay with the constant value


//	//get initial children list from given state.
//	//(allowed moves for CPU.)
//	ListNode * RootChildren = GetMovesForPlayer(game, color);
//
//	//child with best score will decide what move to do .
//	int childIndex;
//	int childScore;

//	MinimaxChoose (game,  RootChildren, 0, max_depth,
//				1, MIN_SCORE, MAX_SCORE,	//pruning=true
//				color,1,					//define CPU color as maximizing, start as maximizer
//				BasicScoringFunction, GetMovesForPlayer,
//				&childIndex, &childScore);

	//get list of best moves.
	ListNode * bestMoves = MinimaxMain(game, max_depth, color,
			BasicScoringFunction, GetMovesForPlayer);

	ListNode * pChild = bestMoves;

	//DEBUG_PRINT( ("index %d was chosen. will lead to score of %d\n", childIndex, childScore));
//
//	//do the relevant move
//	//TODO use function for that- find move in list
//	int i=0;
//	//find index in list
//	ListNode * pChild = RootChildren;
//	while (i<childIndex && pChild!=NULL )
//	{
//		pChild = pChild->next;
//		i++;
//	}

	//do the first move in list
	if (pChild)
	{
		move_t * move = (move_t *) (pChild->data);
		DoMove(move, game);

		//print
		//"Computer: move <x,y> to <i,j>[<k,l>...]\n".
		printf("Computer: move ");
		MovePrint(move); //print the move according to format
		printf("\n");

	}

	//free moves list
	ListFreeElements(bestMoves, MoveFree);

}


int UserTurn (game_state_t * game)
{

	move_t selectedMove;
	//use the appropriate UI to select a move from the user.
	int move_ready = 0;
	if (_GameMgrHandlers.SelectUserMoveFunc)
	{
		move_ready = _GameMgrHandlers.SelectUserMoveFunc(game, &selectedMove);
		//there are cases we exit this function without a move.
	}


	//if there is a move, do it.
	if (move_ready)
	{
		DoMove(&selectedMove, game);
		return 1;
	}
	else
	{
		//quit scenario
		return 0;
	}
}


void DoGame(game_state_t * game)
{

	//TODO can start a game when 1 side loses/tie.

	//find out who starts
	color_t next_player = Settings_NextPlayer_Get();

	//play while you can
	//(allow one side to be in "check").
	play_status_t play_status = GetPlayStatus(game, next_player);
	while (play_status==STATUS_CONTINUE_PLAY
			|| play_status==STATUS_OPPONENT_IN_CHECK
			|| play_status==STATUS_PLAYER_IN_CHECK)
	{

		//do turn of next player. determine if it's of User or CPU.
		if (Settings_GameMode_Get()==GAME_MODE_PLAYER_VS_CPU
				&& Settings_CPUColor_Get()==next_player)
		{
			//if we are in player vs.cpu mode, and it is CPU's turn.
			CPUTurn(game);
		}
		else
		{
			//in any other case, it is user turn.
			//this function waits for IO (GUI / console).
			if (!UserTurn (game))
			{
				return; //allow user to quit
			}
		}


		//display updated board after turn
		if (_GameMgrHandlers.DisplayUpdatedBoardFunc)
		{
			_GameMgrHandlers.DisplayUpdatedBoardFunc(game);
		}


		//switch player (also save on global)
		next_player = GetOppositeColor(next_player);
		Settings_NextPlayer_Set(next_player);

		//update play status for the new player
		play_status=GetPlayStatus(game, next_player);

		//handle "check" state for new player (if it is).
		//will not print on checkmate.
		if (_GameMgrHandlers.HandleCheck)
		{
			_GameMgrHandlers.HandleCheck(play_status, next_player);
		}
	}

	//handle end game.
	if (_GameMgrHandlers.HandleEnd)
	{
		_GameMgrHandlers.HandleEnd(game, play_status, next_player);
	}

	//go back to caller

}

//prints moves one after the other, according to format.
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
	//avoiding null
	if (!move)
	{
		return;
	}

	//print move src.
	printf ("<%c,%d>", move->src.x, move->src.y);

	printf (" to ");
	//print move destination.
	printf ("<%c,%d>", move->dest.x, move->dest.y);

	if (move->promote_to_identity != 0)
	{
		printf (" %s", GetPromotionIdentityName(move->promote_to_identity));
	}

	if (move->num_captures>0)
	{
		DEBUG_PRINT((" (CAPTURE)"));
	}

}


