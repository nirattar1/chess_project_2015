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





#define NumbersListLen 7
ListNode * CreateNumbersList ()
{

	int numbers [NumbersListLen]= {5, 6, 7, 8, 9, 10, 11};

	ListNode * list = NULL;
	//loop on array and add to list
	for (int i=0; i<NumbersListLen; i++)
	{
		//copy data to new area
		int * pi = (int *) mymalloc (sizeof (int));
		*pi = numbers[i];

		//create new node
		ListNode * newnode = ListCreateNode( (void *) pi , sizeof (int));

		if (!list)
		{
			//list is empty- make node head of list.
			list = newnode;
		}
		else
		{
			//list is not empty- concat.
			ListNode ** listp = &list;
			ListConcat( listp, newnode);
			list = *listp;
		}
	}

	return list;
}



void ListTest ()
{

	ListNode * list = CreateNumbersList();


	intlist_print(list);
	printf ("num elements: %d\n", ListCount(list));

	memory_print ();


	ListFreeElements(list, intlist_free);
	list = NULL;


	memory_print ();


}



void SimpleList ()
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
