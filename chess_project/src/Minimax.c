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
#include "Draughts.h"
#include "tests/Test_Minimax.h"
#include <math.h>
//initialize global
int _NUM_LEAVES = 0;



//note: maximizing player==root player.
void MinimaxChoose (
		STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int enable_pruning, int alpha, int beta,
		color_t maximizing_player, int current_turn_is_maximizing,
		int (*ScoringFunction)(STATE_TYPE *, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * chosenSon, int * chosenValue) //by reference, will update these for caller.
{

	//TO FIX
	//debug (only for ints)
	//DEBUG_PRINT(("current state: %d\n", *((int *) state)));


	//get children of node.
	//(on first level - from argument, otherwise generate from state.)
	ListNode * Children = NULL;
	if (current_depth==0)
	{
		Children = RootChildren;
	}
	else if (current_depth!=max_depth)	//compute children for any non-leaf
	{
		//get current player from argument
		color_t current_player = (current_turn_is_maximizing) ? maximizing_player : GetOppositeColor(maximizing_player) ;
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
		*chosenValue = (ScoringFunction(state, maximizing_player));

		//debug
		DEBUG_PRINT(("scoring function value: %d\n", *chosenValue));

		//increment global leaves counter. (for debug)
		_NUM_LEAVES ++ ;
		DEBUG_PRINT(("number of leaves: %d\n", _NUM_LEAVES));
		return;
	}

	if (current_turn_is_maximizing)
	{
		//MAXIMIZING PLAYER
		//get initial value
		int v = MIN_SCORE;
		//for each child (move):
		int iChild = 0;
		ListNode * pChildren = NULL;
		for (pChildren = Children; pChildren !=NULL; pChildren = pChildren->next )
		{

			//create static copy of state.
			STATE_TYPE newState;
			//TO FIX
	//		char newBoard [BOARD_SIZE][BOARD_SIZE];
	//		newState.pieces = (board_column *) newBoard;
	//		CopyGameState(&newState, state);
			//for debug ints:
			memcpy (&newState, state, sizeof (STATE_TYPE));

			//TO FIX
			//update state based on child (play move)
			//DoMove( (move_t *) pChildren->data , &newState);
			//for debug ints:
			TestUpdateState (&newState, current_depth, iChild);

			//debug
			DEBUG_PRINT(("if i play move %d board will look like:\n ", iChild));
			if (IS_DEBUG)
			{
				//PrintBoard(&newState);
			}

			//call recursively (next player's move) to determine the score from this child.

			int childIndex;	//not really used.
			int childScore;


			//compute next turn (opposite color, minimizing player)
			MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
			enable_pruning, alpha, beta,
			maximizing_player, 0,	//0 == minimizing.
			ScoringFunction, ChildGenerateFunction, &childIndex, &childScore);

			//update v and alpha to the max.
			if (childScore >= v)
			{
				v = childScore ;
				*chosenSon = iChild ;
			}
			alpha = (v >= alpha) ? v : alpha;


			DEBUG_PRINT( ("current depth : %d. score from child %d: %d\n", current_depth, iChild, childScore));
			iChild++;

			//prune if needed
			//(don't continue to next child
			if (enable_pruning && beta <= alpha)
			{
				DEBUG_PRINT(("beta cut off. pruning child."));
				break;
			}
		}

		//return value from this node up.
		*chosenValue = v;
		//chosenSon was already updated.

	}
	else
	{
		//MINIMIZING PLAYER
		//get initial value
		int v = MAX_SCORE;
		//for each child (move):
		int iChild = 0;
		ListNode * pChildren = NULL;
		for (pChildren = Children; pChildren !=NULL; pChildren = pChildren->next )
		{

			//create static copy of state.
			STATE_TYPE newState;
			//TO FIX
	//		char newBoard [BOARD_SIZE][BOARD_SIZE];
	//		newState.pieces = (board_column *) newBoard;
	//		CopyGameState(&newState, state);
			//for debug ints:
			memcpy (&newState, state, sizeof (STATE_TYPE));

			//TO FIX
			//update state based on child (play move)
			//DoMove( (move_t *) pChildren->data , &newState);
			//for debug ints:
			TestUpdateState (&newState, current_depth, iChild);

			//debug
			DEBUG_PRINT(("if i play move %d board will look like:\n ", iChild));
			if (IS_DEBUG)
			{
				//PrintBoard(&newState);
			}

			//call recursively (next player's move) to determine the score from this child.

			int childIndex;	//not really used.
			int childScore;


			//compute next turn (opposite color, maximizing player)
			MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
			enable_pruning, alpha, beta,
			maximizing_player, 1,	//1 == maximizing.
			ScoringFunction, ChildGenerateFunction, &childIndex, &childScore);

			//update v and beta to the max.
			if (childScore <= v)
			{
				v = childScore ;
				*chosenSon = iChild ;
			}
			beta = (v <= beta) ? v : beta;


			DEBUG_PRINT( ("current depth : %d. score from child %d: %d\n", current_depth, iChild, childScore));
			iChild++;

			//prune if needed
			//(don't continue to next child
			if (enable_pruning && beta <= alpha)
			{
				DEBUG_PRINT(("alpha cut off. pruning child."));
				break;
			}
		}

		//return value from this node up.
		*chosenValue = v;
		//chosenSon was already updated.

	}


	//free all children (got what we need from them).
	//(only on depth > 0), because caller needs children in level 0
	if (current_depth!=0)
	{
		//TO FIX
		//ListFreeElements(Children, MoveFree);
		ListFreeElements(Children, intlist_free);
	}


	return;
}

//find max or min value in array
//if find_max = 1 -> will find max.
//if find_max = 0 -> will find min.
//will update : chosenValue with the value.
// chosenIndex with the index.
//if array is of length 0 , will return -1.
int ArrFindMaxOrMin (int * array, int arr_size,
		int find_max, int * chosenValue, int * chosenIndex)
{
	if (!array || arr_size==0)
	{
		return -1;
	}

	//initialize values
	int extremeValue = array[0];
	int extremeIndex = 0;

	//loop through array and find the extreme value.
	for (int i=0; i<arr_size; i++)
	{
		//if found a value that exceeds the extreme, change indication.
		if (((find_max==1) && array[i] > extremeValue)
			|| ((find_max==0) && array[i] < extremeValue))
		{
			extremeValue = array[i];
			extremeIndex = i;
		}
	}

	//update return arguments.
	*chosenValue = extremeValue;
	*chosenIndex = extremeIndex;
	return 1;
}

