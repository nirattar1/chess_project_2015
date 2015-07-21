
/*
 * ListNode.c
 	 list implementations.
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#include "ListNode.h"
#include "Memory.h"

//create a new link.
//does not fill its data.
LINK ListCreateNode (void * data, int elemSize)
{
	LINK node = (ELEMENT*) mymalloc( sizeof( ELEMENT ) );
	node->data = data;
	node->next = NULL;
	return node;
}

//add next
void ListInsertAfterLink(LINK l1, LINK l2)
{
	//preserve l1 next.
	l2->next = l1->next;

	//update l1 to point to l2.
	l1-> next = l2;
}

void ListPushBackElement (LINK *head, void * data, size_t elem_size)
{
	//creates a new node around the data
	ListNode * newnode = ListCreateNode( data , elem_size);

	//if head points to null
	if (!(*head))
	{
		//list is empty- make node head of list.
		*head = newnode;
	}
	else
	{
		//list is not empty- concat.
		ListConcat( head, newnode);
	}

}

//concat
void ListConcat( LINK *a, LINK b )
{
	if (*a == NULL)
	{
		*a = b;
	}
	else if ( (*a)->next == NULL )
	{
		(*a)->next = b;
	}
	else
	{
		ListConcat( &((*a)->next), b );
	}
}

//counts elements
int ListCount( LINK head )
{
	int cnt = 0;
	for ( ; head !=0; head = head->next )
	{
		++cnt;
	}

	return cnt;
}


void ListFreeElements ( LINK head, void (*free_function)(void *) )
{


	LINK last;
	while (head !=0)
	{
		//call free function to get rid of data.
		(*free_function)(head->data);

		//save last element.
		last = head;

		head = head->next ;
		myfree (last);
	}

}



//original addition algorithm from class
//LINK string_to_list( )
//{
//	DATA s;
//	LINK head = NULL, tail = NULL;
//	if ( (s=getchar())!=EOF )
//	{
//		head = ( ELEMENT* )malloc( sizeof( ELEMENT ) );
//		assert(head!=NULL);
//		head->d = s;
//		tail = head;
//
//
//		while ( (s=getchar())!=EOF )
//		{
//		tail->next = ( ELEMENT* )malloc( sizeof( ELEMENT ) );
//		tail = tail->next;
//		assert(tail!=NULL);
//		tail->d = s;
//		}
//		tail->next = NULL;
//	}
//	return head;
//}


