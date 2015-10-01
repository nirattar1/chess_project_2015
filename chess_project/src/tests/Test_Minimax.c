/*
 * Test_Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "../Minimax.h"
#include "Test_List.h"

#define MAX_TEST_VALUES 16
int test_values [MAX_TEST_VALUES];

void Test_Minimax ()
{

	//Test_Minimax_simple_tree();

	Test_Minimax_tree2_for_pruning();
}

//will return a scoring for state.
//is based on a global array of static values test_values.
//returns the value in the array , according to global leaves index.
int TestScoring (STATE_TYPE * state)
{
	int score = test_values [_NUM_LEAVES];
	printf ("scoring according to state: %d, score = %d\n", *state, score);
	//ignore state, just give according to leaves.
	return score;
}


//will create a list of children based on state.
//and return a pointer to them.
//this always returns a fixed number of children (2).
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

void Test_Minimax_simple_tree ()
{
	int max_depth  = 3;

	//{10 , -10 , 5, -7, 9, 20, -21, -24, 9, 11, -3 , 4, -20};

	//update global test values
	test_values[0] = 10;
	test_values[1] = -10;
	test_values[2] = 5;
	test_values[3] = -7;
	test_values[4] = 9;
	test_values[5] = 20;
	test_values[6] = -21;
	test_values[7] = -24;
	//get initial children list from given state.
	STATE_TYPE state = 7000;
	ListNode * RootChildren = TestChildrenGenerate(state);


	int childIndex;	//used only on top level.
	int childScore;

	MinimaxChoose (&state, COLOR_WHITE, RootChildren, 0, max_depth,
			TestScoring, TestChildrenGenerate,
			&childIndex, &childScore);

	printf ("index %d was chosen. will lead to score of %d\n", childIndex, childScore);

}


void Test_Minimax_tree2_for_pruning ()
{
	int max_depth  = 3;

	//{10 , -10 , 11, 12, 9, 8, -21, -24};

	//update global test values
	test_values[0] = 10;
	test_values[1] = -10;
	test_values[2] = 11;
	test_values[3] = 12;
	test_values[4] = 9;
	test_values[5] = 8;
	test_values[6] = -21;
	test_values[7] = -24;
	//get initial children list from given state.
	STATE_TYPE state = 7000;
	ListNode * RootChildren = TestChildrenGenerate(state);


	int childIndex;	//used only on top level.
	int childScore;

	MinimaxChoose (&state, COLOR_WHITE, RootChildren, 0, max_depth,
			TestScoring, TestChildrenGenerate,
			&childIndex, &childScore);

	printf ("index %d was chosen. will lead to score of %d\n", childIndex, childScore);

}

//for testing. remove this function
//test function , updates child based on depth and child index.
void TestUpdateState(int * state, int depth, int iChild)
{
	*state = *state + (depth * 10) + (iChild+1) ;
}
