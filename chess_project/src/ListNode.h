/*
 * ListNode.h
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

#include <stdlib.h>

/*************
 * LIST DATA TYPE
************/
typedef void * DATA;


/*************
 * LIST DEFINITION
************/

struct ListNode
{
	DATA data;
	struct ListNode *next;
};


typedef struct ListNode ELEMENT;
typedef struct ListNode ListNode;
typedef ELEMENT* LINK;


/*************
 * LIST FUNCTIONS
************/



//create a new link.
//does not fill its data.
LINK ListCreateNode (void * data, int elemSize);

//add next (not used)
void ListInsertAfterLink(LINK l1, LINK l2);

//concatenate
void ListConcat( LINK *a, LINK b );


//gets pointer to data, and its size.
//creates a new node and add it at end of list.
//may alter the head.
void ListPushBackElement (LINK * head, void * data, size_t elem_size);

//counts elements
int ListCount( LINK head );


//counts elements
void ListPrint( LINK head );

//free list elements
void ListFreeElements ( LINK head, void (*free_function)(void *) );

#endif /* LISTNODE_H_ */
