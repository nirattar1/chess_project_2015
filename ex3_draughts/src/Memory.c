/*
 * Memory.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include <stdlib.h>
#include "Memory.h"
#include <stdio.h>
#include "Draughts.h"

int CNT_CALLS_MALLOC = 0;
int CNT_CALLS_FREE = 0;


void * mymalloc (size_t size)
{
	CNT_CALLS_MALLOC ++;
	return malloc(size);
}

void myfree (void * data)
{
	CNT_CALLS_FREE ++;
	free(data);
}

void memory_print ()
{
	DEBUG_PRINT( ("\n*** memory ***\n"));
	DEBUG_PRINT(("%d malloc calls,", CNT_CALLS_MALLOC));
	DEBUG_PRINT(("%d free calls.\n", CNT_CALLS_FREE));
	DEBUG_PRINT(("balance : %d\n", (CNT_CALLS_MALLOC- CNT_CALLS_FREE)));
}

