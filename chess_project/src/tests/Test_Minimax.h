/*
 * Test_Minimax.h
 *
 *  Created on: Sep 30, 2015
 *      Author: nirattar
 */

#ifndef SRC_TESTS_TEST_MINIMAX_H_
#define SRC_TESTS_TEST_MINIMAX_H_

/*
 * Test_Minimax.c
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "../Minimax.h"
#include "Test_List.h"



//will return a scoring for state.
int TestScoring (STATE_TYPE * state);


//will create a list of children based on state.
//and return a pointer to them.
//this always returns a fixed number of children (2).
ListNode * TestChildrenGenerate (STATE_TYPE state);

void Test_Minimax ();

void TestUpdateState(int * state, int depth, int iChild);

#endif /* SRC_TESTS_TEST_MINIMAX_H_ */
