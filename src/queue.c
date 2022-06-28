/*******************************************************************************
******************************** - QUEUE - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation of Queue ADT using a linked_list
*	NAME 			Liad Raz

*******************************************************************************/

#include <stdlib.h>			/* malloc, free*/
#include <assert.h>			/* assert */

#include "utilities.h"		/* INVALID_PTR, SUCCESS */
#include "queue.h"
#include "linked_list.h"	/* LListCreate, LListDestroy, LListInsert, 
							LListIsSameIter, LListBegin, LListEnd, 
							LListNext, LListsAppend */

#define ASSERT_IS_ALLOC(ptr)									\
		assert (0 != (queue_ty *)ptr && "QUEUE is not allocated");

struct queue
{
    llist_ty *front;         /* LIST POINTING the FIRST NODE */
    llist_itr_ty back;  	/* ITERATOR POINTING the LAST NODE */
};


/*******************************************************************************
****************************** Queue Create ***********************************/
queue_ty *QueueCreate(void)
{
	queue_ty *queue = (queue_ty *)malloc(sizeof(queue_ty));
	
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->front = LListCreate();
	
	if (NULL == queue->front)
	{
		free(queue);
		return NULL;
	}
	
	/* Iterator back will point the last node (one before the TAIL DUMMY)*/
	queue->back = LListEnd(queue->front);
	
	return queue;
}


/*******************************************************************************
****************************** Queue Destroy **********************************/
void QueueDestroy(queue_ty *queue)
{
	ASSERT_IS_ALLOC(queue);
	
	LListDestroy(queue->front);
	DEBUG_MODE(queue->front = INVALID_PTR;)
	
	free(queue);
}


/*******************************************************************************
****************************** Queue Enqueue **********************************/
int QueueEnqueue(queue_ty *queue, void *user_input)
{
	ASSERT_IS_ALLOC(queue);
	
	if (LListIsSameIter(queue->back, LListEnd(queue->front)))
	{
		return ALLOC_ERR;
	}
	
	/* update the last node */
	queue->back = LListNext(queue->back);
	UNUSED(user_input);
	
	return SUCCESS;
}


/*******************************************************************************
****************************** Queue Dequeue **********************************/
void QueueDequeue(queue_ty *queue)
{
	ASSERT_IS_ALLOC(queue);
	assert (0 == QueueIsEmpty(queue) && "Cannot Dequeue when queue is empty");
	
	LListRemove(LListBegin(queue->front));
	
	/* Checks if the queue is empty, prevents invalidate back iterator */
	if (LListIsEmpty(queue->front))
	{
		queue->back = LListBegin(queue->front);
	}
}


/*******************************************************************************
****************************** Queue Peek *************************************/
void *QueuePeek(const queue_ty *queue)
{
	ASSERT_IS_ALLOC(queue);
	
	return LListGetData(LListBegin(queue->front));
}


/*******************************************************************************
****************************** Queue Size *************************************/
size_t QueueSize(const queue_ty *queue)
{
	ASSERT_IS_ALLOC(queue);
	
	return LListCount(queue->front);
}


/*******************************************************************************
****************************** Queue IsEmpty **********************************/
int QueueIsEmpty(const queue_ty *queue)
{
	return LListIsEmpty(queue->front);
}


/*******************************************************************************
****************************** Queue Append ***********************************/
/*	TODO TODO
queue_ty *QueueAppend(queue_ty *queue_dest, queue_ty *queue_src)
{
	queue_dest->back = LListEnd(queue_src->front);
	queue_dest->front = LListsAppend(queue_dest->front, queue_src->front);
	
	return queue_dest;
}
*/



/*******************************************************************************
****************************** PsuedoCode **************************************
********************************************************************************

Queue struct consists :
llist struct 

 	(list->front)					(back.node_itr)
            |							 |
            |							 |
        +---+---+     +---+---+     +----+------+
        | 1 | o----->| 2 | o-----> | 3 | NULL |
        +---+---+     +---+---+     +----+------+ 
		FRONT						BACK

        REMOVE(DEQUEUE) -> FRONT
        INSERT(ENQUEUE) -> BACK

--------------------------------------------------------------------------------
	QueueCreate
	
	malloc queue struct
	create list and assign it to the queue list member
	address back itertaor to point the last node in list
	
--------------------------------------------------------------------------------
	QueueDestroy
	
	free link list with llist Destroy
	free queue struct
	
--------------------------------------------------------------------------------
	QueueEnqueue
	
	call func LListInsert and insert new node to the back of the queue.
	assign the new iterator the temporary iterator
	
	update the last node
	
	check the return value from the insert function
	if tmp iterator is not NULL return 0
	
--------------------------------------------------------------------------------
	QueueDequeue
	
	call func LListRemove and remove the first node in the queue
	
*******************************************************************************/

