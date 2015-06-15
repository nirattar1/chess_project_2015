/*
 * Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */
#include "Minimax.h"
#include <stdio.h>





//find max in array
ArrFindMax (int * arr, int arr_size)
{
	return 10;
}
ArrFindMin (int * arr, int arr_size)
{
	return -10;
}

void MinimaxChoose (STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int (*ScoringFunction)(STATE_TYPE *),
		ListNode * (*TestChildrenGenerate)(STATE_TYPE *),
		int * chosenSon, int * chosenValue) //by reference, will update these for caller.
{

	//halt condition (leaf node)
	if (current_depth==max_depth)
	{
		//compute this node's value and return it.
		*chosenSon = 0;
		*chosenValue = (ScoringFunction(&current_depth));
	}

	//get children.
	//(on first level - from argument, otherwise generate from state.)
	ListNode * pChildren;
	if (current_depth==0)
	{
		pChildren = RootChildren;
	}
	else
	{
		pChildren = TestChildrenGenerate (state);
	}


	//for each child :
	int numChildren = ListCount(pChildren);
	int iChild = 0;
	int * Scores = (int *) mymalloc (numChildren * (sizeof (int)) );
	for ( ; pChildren !=0; pChildren = pChildren->next )
	{
		printf ("element value: %d\n", *((CHILD_TYPE *) pChildren->data));

		//create static copy of state.
		STATE_TYPE newState;
		memcpy (&newState, state, sizeof (STATE_TYPE));

		//update state based on child (play move)
		//UpdateState (newState, pChildren);

		//call recursively to determine the score from this child.

		int childIndex;	//not really used.
		int childScore;
		MinimaxChoose(&newState, NULL, current_depth+1, max_depth,
				ScoringFunction, TestChildrenGenerate, &childIndex, &childScore);

		Scores [iChild] = childScore;
		iChild++;
	}

	//choose the child with the max / min level.
	//even depth (maximizer)- choose max.
	if (current_depth%2 == 0)
	{
		ArrFindMax (Scores, numChildren);
	}
	//odd depth (minimizer) - choose min.
	else
	{
		ArrFindMin (Scores, numChildren);
	}

}

