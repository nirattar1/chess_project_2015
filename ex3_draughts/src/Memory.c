/*
 * Memory.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include <stdlib.h>
#include "Memory.h"

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
	printf("\n*** memory ***\n");
	printf("%d malloc calls,", CNT_CALLS_MALLOC);
	printf("%d free calls.\n", CNT_CALLS_FREE);
	printf("balance : %d\n", (CNT_CALLS_MALLOC- CNT_CALLS_FREE));
}

