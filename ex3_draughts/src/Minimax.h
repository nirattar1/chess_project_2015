/*
 * Minimax.h
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "ListNode.h"

#ifndef MINIMAX_H_
#define MINIMAX_H_

//for testing
#define STATE_TYPE int
#define CHILD_TYPE int
extern int _NUM_LEAVES;

//gets a list of children. will determine the best option out of them.
void MinimaxChoose (STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int (*ScoringFunction)(STATE_TYPE *),
		ListNode * (*TestChildrenGenerate)(STATE_TYPE *),
		int * chosenSon, int * chosenValue); //by reference, will update these for caller.

int ArrFindMaxOrMin (int * array, int arr_size,
		int find_max, int * chosenValue, int * chosenIndex);

#endif /* MINIMAX_H_ */
