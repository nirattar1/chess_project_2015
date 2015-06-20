/*
 * Test_Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "Minimax.h"
#include "Test_List.h"


int test_values [] = {10 , -10 , 5, -7, 9, 20, -21, -24, 9, 11, -3 , 4, -20};



//will return a scoring for state.
int TestScoring (STATE_TYPE * state)
{
	int score = test_values [_NUM_LEAVES];
	printf ("scoring according to state: %d, score = %d\n", *state, score);
	//ignore state, just give according to leaves.
	return score;
}


//will create a list of children based on state.
//and return a pointer to them.
//this always returns a fixed number of children.
ListNode * TestChildrenGenerate (STATE_TYPE state)
{
	//prepare empty list.
	ListNode * list = NULL;

	//allocate children (no meaning to values on the way)
	int * child1 = (int *) mymalloc (sizeof (int));
	*child1 = 0;
	int * child2 = (int *) mymalloc (sizeof (int));
	*child2 = 0;

	//add children to the list.
	ListNode ** listp = &list;
	ListPushBackElement (listp, (void *) child1, sizeof (int));
	list = *listp;

	listp = &list;
	ListPushBackElement (listp, (void *) child2, sizeof (int));
	list = *listp;

	return list;

}

//void Test_Minimax ()
//{
//	int max_depth  = 3;
//
//	//get initial children list from given state.
//	STATE_TYPE state = 7000;
//	ListNode * RootChildren = TestChildrenGenerate(state);
//
//
//	int childIndex;	//used only on top level.
//	int childScore;
//
//	MinimaxChoose (&state, RootChildren, 0, max_depth,
//			TestScoring, TestChildrenGenerate,
//			&childIndex, &childScore);
//
//	printf ("index %d was chosen. will lead to score of %d\n", childIndex, childScore);
//
//}
