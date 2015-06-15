/*
 * Memory.c
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#include <stdlib.h>

int CNT_CALLS_MALLOC = 0;

void * mymalloc (size_t size)
{
	CNT_CALLS_MALLOC ++;
	return malloc(size);
}

void * myfree (void * data)
{
	CNT_CALLS_MALLOC --;
	free(data);
}

void memory_print ()
{
	printf("memory calls:  %d\n", CNT_CALLS_MALLOC);
}

