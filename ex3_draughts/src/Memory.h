/*
 * Memory.h
 *
 *  Created on: Jun 12, 2015
 *      Author: nirattar
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdlib.h>

extern int CNT_CALLS_MALLOC;

void * mymalloc (size_t size);
void * myfree (void * data);
void memory_print ();

#endif /* MEMORY_H_ */
