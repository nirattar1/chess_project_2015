/*
 * Minimax.h
 *
 *  Created on: Jun 14, 2015
 *      Author: nirattar
 */

#include "ListNode.h"
#include "Game.h"

#ifndef MINIMAX_H_
#define MINIMAX_H_

//for testing
#define STATE_TYPE int
//for draughts
//#define STATE_TYPE game_state_t

//for testing
extern int _NUM_LEAVES;

//maximum score
#define MAX_SCORE 10000
#define MIN_SCORE -10000

//gets a list of children. will determine the best option out of them.
void MinimaxChoose (
		STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int enable_pruning, int alpha, int beta,
		color_t current_player, int is_maximizing,
		int (*ScoringFunction)(STATE_TYPE *, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * chosenSon, int * chosenValue); //by reference, will update these for caller.

int ArrFindMaxOrMin (int * array, int arr_size,
		int find_max, int * chosenValue, int * chosenIndex);

#endif /* MINIMAX_H_ */
