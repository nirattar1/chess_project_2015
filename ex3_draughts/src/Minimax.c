/*
 * Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */
#include "Minimax.h"
#include <stdio.h>
#include "Test_List.h"
#include "Memory.h"

//initialize global
int _NUM_LEAVES = 0;



//for testing. remove this function
//test function , updates child based on depth and child index.
int TestUpdateState(int * state, int depth, int iChild)
{
	*state = *state + (depth * 10) + (iChild+1) ;
}


void MinimaxChoose (STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int (*ScoringFunction)(STATE_TYPE *),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *),
		int * chosenSon, int * chosenValue) //by reference, will update these for caller.
{


	//debug
	printf("current state: %d\n", *((int *) state));

	//halt condition (leaf node)
	//update based on scoring function and return.
	if (current_depth==max_depth)
	{
		//compute this node's value and return it.
		*chosenSon = 0;
		//call scoring function to compute based on state.
		*chosenValue = (ScoringFunction(state));

		//debug
		printf("scoring function value: %d\n", *chosenValue);

		//increment global leaves counter. (for debug)
		_NUM_LEAVES ++ ;
		printf("number of leaves: %d\n", _NUM_LEAVES);
		return;
	}

	//get children.
	//(on first level - from argument, otherwise generate from state.)
	ListNode * Children;
	if (current_depth==0)
	{
		Children = RootChildren;
	}
	else
	{
		Children = ChildGenerateFunction (state);
	}


	//for each child :
	int numChildren = ListCount(Children);
	int iChild = 0;
	int * Scores = (int *) mymalloc (numChildren * (sizeof (int)) );
	ListNode * pChildren;
	for (ListNode * pChildren = Children; pChildren !=NULL; pChildren = pChildren->next )
	{

		//create static copy of state.
		STATE_TYPE newState;
		memcpy (&newState, state, sizeof (STATE_TYPE));

		//update state based on child (play move)
		TestUpdateState (&newState, current_depth, iChild);

		//call recursively to determine the score from this child.

		int childIndex;	//not really used.
		int childScore;
		MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
				ScoringFunction, ChildGenerateFunction, &childIndex, &childScore);

		printf ("current depth : %d. score from child %d: %d\n", current_depth, iChild, childScore);
		Scores [iChild] = childScore;
		iChild++;
	}

	//choose the child with the max / min level.
	//even depth (maximizer)- choose max.
	if (current_depth%2 == 0)
	{
		ArrFindMaxOrMin(Scores, numChildren, 1, chosenValue, chosenSon);
	}
	//odd depth (minimizer) - choose min.
	else
	{
		ArrFindMaxOrMin(Scores, numChildren, 0, chosenValue, chosenSon);
	}

	//free all children (got what we need from them).
	ListFreeElements(Children, intlist_free);
	//free scores of children.
	myfree(Scores);

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

