/*
 * Test_Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "Minimax.h"

int test_values [] = {10 , -10 , 5, -7, 9, 20, 21, 24};



//will return a scoring for state.
int TestScoring (int * leaf_num)
{
	return test_values [*leaf_num];
}
d

//will create a list of children, and return a pointer to them.
ListNode * TestChildrenGenerate (STATE_TYPE state)
{
	//prepare empty list.
	ListNode * list = NULL;

	//allocate children
	int * child1 = (int *) mymalloc (sizeof (int));
	*child1 = 7;
	int * child2 = (int *) mymalloc (sizeof (int));
	*child2 = 8;

	//add children to the list.
	ListNode ** listp = &list;
	ListPushBackElement (listp, (void *) child1, sizeof (int));
	list = *listp;

	listp = &list;
	ListPushBackElement (listp, (void *) child2, sizeof (int));
	list = *listp;

	return list;

}

void Test_Minimax ()
{

	int sons = 2;
	int max_depth  = 2;

	//get initial children list from given state.
	STATE_TYPE state = 7000;
	ListNode * RootChildren = TestChildrenGenerate(state);


	int childIndex;	//used only on top level.
	int childScore;

	MinimaxChoose (&state, RootChildren, 0, max_depth,
			TestScoring, TestChildrenGenerate,
			&childIndex, &childScore);

	printf ("index %d was chosen.\n", childIndex);

}
