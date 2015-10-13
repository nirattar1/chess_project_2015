/*
 * Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */
#include "Minimax.h"
#include <stdio.h>
#include <string.h>
#include "Memory.h"
#include "tests/Test_Minimax.h"
#include <math.h>

#include "Game_Mgr.h"
//initialize global
int _NUM_LEAVES = 0;


static void UpdateState(STATE_TYPE * state, STATE_TYPE * newState, ListNode * pChildren,
		int iChild)
{


	CopyGameState(newState, state);
//	//for debug ints:
//	memcpy (newState, state, sizeof (STATE_TYPE));


	//update state based on child (play move)
	DoMove( (move_t *) pChildren->data , newState);
//	//for debug ints:
//	TestUpdateState (newState, current_depth, iChild);

	//debug
	DEBUG_PRINT(("if i play move %d board will look like:\n ", iChild));
	if (IS_DEBUG)
	{
		PrintBoard(newState);
	}
}

//note: maximizing player==root player.
void MinimaxChoose (
		STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int enable_pruning, int alpha, int beta,
		color_t maximizing_player, int current_turn_is_maximizing,
		int (*ScoringFunction)(STATE_TYPE *, color_t, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * chosenSon, int * chosenValue) //by reference, will update these for caller.
{


	//debug (only for ints)
	//DEBUG_PRINT(("current state: %d\n", *((int *) state)));

	//get current player from argument
	color_t current_player = (current_turn_is_maximizing) ? maximizing_player : GetOppositeColor(maximizing_player) ;

	//get children of node.
	//(generate from state.)
	ListNode * Children = NULL;

	if (current_depth!=max_depth)	//compute children for any non-leaf
	{
		Children = ChildGenerateFunction (state, current_player);
	}

	//halt condition- 1 of the following:
	//1. if max depth was reached
	//2. no more children from this node.
	//compute scoring function and return.
	if (current_depth==max_depth || !Children)
	{
		*chosenSon = 0;//has no meaning here.
		//call scoring function to compute based on state.
		*chosenValue = (ScoringFunction(state, maximizing_player, current_player));

		//debug
		DEBUG_PRINT(("scoring function value: %d\n", *chosenValue));

		//increment global leaves counter. (for debug)
		_NUM_LEAVES ++ ;
		DEBUG_PRINT(("number of leaves: %d\n", _NUM_LEAVES));
		return;
	}

	//initial value
	int v = 0;
	if (current_turn_is_maximizing)
	{
		//MAXIMIZING PLAYER
		//get initial value
		v = MIN_SCORE;
		//for each child (move):
		int iChild = 0;
		ListNode * pChildren = NULL;
		for (pChildren = Children; pChildren !=NULL; pChildren = pChildren->next )
		{

			//create static copy of state and update it.
			STATE_TYPE newState;
			char newBoard [BOARD_SIZE][BOARD_SIZE];
			newState.pieces = (board_column *) newBoard;
			UpdateState(state, &newState, pChildren, iChild);

			//call recursively (next player's move) to determine the score from this child.

			int childIndex;	//not really used.
			int childScore;

			//compute next turn (opposite color, minimizing player)
			MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
			enable_pruning, alpha, beta,
			maximizing_player, 0,	//0 == minimizing.
			ScoringFunction, ChildGenerateFunction, &childIndex, &childScore);

			//update v and alpha to the max.
			if (childScore > v)
			{
				v = childScore ;
				*chosenSon = iChild ;
			}
			alpha = (v >= alpha) ? v : alpha;


			DEBUG_PRINT( ("current depth : %d. score from child %d: %d\n", current_depth, iChild, childScore));
			iChild++;

			//prune if needed
			//(don't continue to next child)
			if (enable_pruning && beta <= alpha)
			{
				DEBUG_PRINT(("beta cut off. pruning child.\n"));
				break;
			}
		}

	}
	else
	{
		//MINIMIZING PLAYER
		//get initial value
		v = MAX_SCORE;
		//for each child (move):
		int iChild = 0;
		ListNode * pChildren = NULL;
		for (pChildren = Children; pChildren !=NULL; pChildren = pChildren->next )
		{

			//create static copy of state.
			STATE_TYPE newState;
			char newBoard [BOARD_SIZE][BOARD_SIZE];
			newState.pieces = (board_column *) newBoard;
			UpdateState(state, &newState, pChildren, iChild);

			//call recursively (next player's move) to determine the score from this child.

			int childIndex;	//not really used.
			int childScore;

			//compute next turn (opposite color, maximizing player)
			MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
			enable_pruning, alpha, beta,
			maximizing_player, 1,	//1 == maximizing.
			ScoringFunction, ChildGenerateFunction, &childIndex, &childScore);

			//update v and beta to the min.
			if (childScore < v)
			{
				v = childScore ;
				*chosenSon = iChild ;
			}
			beta = (v <= beta) ? v : beta;


			DEBUG_PRINT( ("current depth : %d. score from child %d: %d\n", current_depth, iChild, childScore));
			iChild++;

			//prune if needed
			//(don't continue to next child)
			if (enable_pruning && beta <= alpha)
			{
				DEBUG_PRINT(("alpha cut off. pruning child.\n"));
				break;
			}
		}


	}

	//return value from this node up.
	*chosenValue = v;
	//chosenSon was already updated.

	//free all children (got what we need from them).
	//(only on depth > 0), because caller needs children in level 0
	if (current_depth!=0)
	{

		ListFreeElements(Children, MoveFree);
		//for debug ints
		//ListFreeElements(Children, intlist_free);
	}


	return;
}

//is responsible for choosing step to do the on the first level.
//then going on to selecting the best value out of the children.
ListNode * MinimaxMain (
		STATE_TYPE * state,
		int max_depth,
		color_t maximizing_player,
		int (*ScoringFunction)(STATE_TYPE *, color_t, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t))
{

	//will compute the best children and return them.
	ListNode * BestChildren = NULL;

	//always start from the maximizing

	//get initial children list from given state.
	//(allowed moves for player.)
	ListNode * RootChildren = GetMovesForPlayer(state, maximizing_player);

	//compute the children's scores to find the maximum.
	int childScoreMax = MIN_SCORE;
	int iChild = 0;
	ListNode * pChildren = RootChildren;
	//maintain an array of scores respective to the children.
	int * arrScores = (int *) mymalloc (sizeof (int));//free later

	for (; pChildren !=NULL; pChildren = pChildren->next )
	{

		//create static copy of state and update it.
		STATE_TYPE newState;
		char newBoard [BOARD_SIZE][BOARD_SIZE];
		newState.pieces = (board_column *) newBoard;
		UpdateState(state, &newState, pChildren, iChild);

		//compute the score from this child.
		//(allocate place for child)
		arrScores = (int *) realloc(arrScores, (iChild+1)*(sizeof(int)));

		int childIndex;	//just for function call , not really used.

		//computing next turn (opposite color, minimizing player)
		MinimaxChoose(&newState, NULL, 1, max_depth,	//start from level 1
		1, MIN_SCORE, MAX_SCORE,	//pruning==true
		maximizing_player, 0,	//0 == next level is minimizing.
		ScoringFunction, ChildGenerateFunction, &childIndex, &arrScores[iChild]);

		//update max score found.
		if (arrScores[iChild] > childScoreMax)
		{
			childScoreMax = arrScores[iChild] ;
		}

		DEBUG_PRINT( ("current depth : 0. score from child %d: %d\n", iChild, arrScores[iChild]));
		iChild++;
	}

	//iterate through the moves, to find 1 (or more) moves that have the maximum score.

	iChild=0;
	for (pChildren = RootChildren; pChildren !=NULL; pChildren = pChildren->next )
	{
		if (arrScores[iChild] == childScoreMax)
		{
			ListNode ** listp = &BestChildren;
			//create a copy of child, and add it to the list.
			move_t * newMove = (move_t *) mymalloc (sizeof(move_t));
			MoveCopy(newMove, (move_t *) pChildren->data);
			//add new child (move) to the list.
			ListPushBackElement (listp, (void *) newMove, sizeof (move_t));
			BestChildren = *listp;
		}

		iChild++;
	}

	//free the scores array
	myfree(arrScores);
	//free temporaty list of moves for this computation.
	ListFreeElements(RootChildren, MoveFree);

	//caller becomes owner of the best moves list.
	return BestChildren;
}

