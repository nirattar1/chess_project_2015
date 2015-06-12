/*
 * ListNode.h
 *
 *  Created on: Jun 11, 2015
 *      Author: nirattar
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

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

//add next
void ListInsertAfterLink(LINK l1, LINK l2);

//counts elements
int ListCount( LINK head );


//counts elements
void ListPrint( LINK head );

//free list elements
void ListFreeElements ( LINK head, void (*free_function)(void *) );

#endif /* LISTNODE_H_ */
