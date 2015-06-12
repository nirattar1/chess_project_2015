/*
 * ListTest.c
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include <stdlib.h>
#include "ListNode.h"
#include "Memory.h"

//a test for lists.

//prints elements
void intlist_print( LINK head )
{
	for ( ; head !=0; head = head->next )
	{
		printf ("element value: %d\n", *((int *) head->data));
	}
}

void intlist_free( void * data )
{
	int * p = (int * ) data;
	*p = 0;
	myfree (p);
}


void ListTest ()
{

	int * pi = (int *) mymalloc (sizeof (int));
	*pi = 8;
	int * pj = (int *) mymalloc (sizeof (int));
	*pj = 9;

	//create node
	ListNode * node1 = ListCreateNode( (void *) pi , sizeof (int));

	//add node
	ListNode * node2 = ListCreateNode( (void *) pj , sizeof (int));
	ListInsertAfterLink( node1, node2);

	intlist_print(node1);
	printf ("num elements: %d\n", ListCount(node1));

	memory_print ();


	ListFreeElements(node1, intlist_free);
	node1 = NULL;


	memory_print ();
}
