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


//#define MINIMAX_TESTING_STATE_TYPE_INTS
#ifdef MINIMAX_TESTING_STATE_TYPE_INTS
	#define STATE_TYPE int
#else
	#define STATE_TYPE game_state_t
#endif


//for testing
extern int _NUM_LEAVES;

//maximum score
#define MAX_SCORE 1000000
#define MIN_SCORE -1000000

//gets a list of children. will determine the best option out of them.
//notes:
//the maximizer is always the root player (level 0).
//the root player's color is given in argument.

void MinimaxChoose (
		STATE_TYPE * state, ListNode * RootChildren,
		int current_depth, int max_depth,
		int enable_pruning, int alpha, int beta,
		color_t maximizing_player, int current_turn_is_maximizing,
		int (*ScoringFunction)(STATE_TYPE *, color_t, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * chosenSon, int * chosenValue); //by reference, will update these for caller.


//is responsible for choosing step to do the on the first level.
//then going on to selecting the best value out of the children.
ListNode * MinimaxMain (
		STATE_TYPE * state,
		int max_depth,
		color_t maximizing_player,
		int (*ScoringFunction)(STATE_TYPE *, color_t, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t));


//will make an array of scores, from a state and a given list of its children
//will also update argument childMax with the maximum value found.
//caller is owner of the array.
int * GetMinimaxScoresArrayFromState(STATE_TYPE * state, ListNode * RootChildren,
		int max_depth, color_t maximizing_player,
		int (*ScoringFunction)(STATE_TYPE *, color_t, color_t),
		ListNode * (*ChildGenerateFunction)(STATE_TYPE *, color_t),
		int * childScoreMax);

#endif /* MINIMAX_H_ */
