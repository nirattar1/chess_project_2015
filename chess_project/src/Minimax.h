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
//notes:
//the maximizer is always the root player (level 0).
//the root player's color is given in argument.

void MinimaxChoose (
		STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int enable_pruning, int alpha, int beta,
		color_t maximizing_player, int current_turn_is_maximizing,
		int (*ScoringFunction)(STATE_TYPE *, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * chosenSon, int * chosenValue); //by reference, will update these for caller.

#endif /* MINIMAX_H_ */
