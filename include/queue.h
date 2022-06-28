/*******************************************************************************
******************************** - QUEUE - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Creates Queue abstract data structure API 
*	NAME 			Liad Raz
*
*******************************************************************************/

#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue queue_ty;

/*******************************************************************************
* DESCRIPTION	Creates queue data structure of void* pointers.
* RETURN	 	returns NULL in case of memory failure 
* IMPORTANT	 	User needs to free the allocated queue
*
* Time Complexity 	O(1)
*******************************************************************************/
queue_ty *QueueCreate(void);


/*******************************************************************************
* DESCRIPTION	Frees all queue allocated entities from memory 
*
* Time Complexity 	O(n)
*******************************************************************************/
void QueueDestroy(queue_ty *queue);


/*******************************************************************************
* DESCRIPTION	Insert entity at the back of the Queue
* RETURN	 	status; 0 means SUCCESS non zero failure
*
* Time Complexity 	O(1)
*******************************************************************************/
int QueueEnqueue(queue_ty *queue, void *entity_value);


/*******************************************************************************
* DESCRIPTION	Remove entity from the front of the Queue
* IMPORTANT		queue back iterator will be invalidate when dequeueing to emptyness

* Time Complexity 	O(1)
*******************************************************************************/
void QueueDequeue(queue_ty *queue);


/*******************************************************************************
* DESCRIPTION	Get the value from the front entity
*
* Time Complexity 	O(1)
*******************************************************************************/
void *QueuePeek(const queue_ty *queue);


/*******************************************************************************
* DESCRIPTION	Counts the amount of elements that exists in queue
*
* Time Complexity 	O(n)
*******************************************************************************/
size_t QueueSize(const queue_ty *queue);


/*******************************************************************************
* DESCRIPTION	Check if queue has no entities
* RETURN	 	boolean; 1 TRUE is empty; 0 FALSE is NOT empty
*
* Time Complexity 	O(1)
*******************************************************************************/
int QueueIsEmpty(const queue_ty *queue);


/*******************************************************************************
* DESCRIPTION	Append source list to destination list.
* IMPORTANT		queue src remains and emptied.
				It should be freed by the user.
*
* Time Complexity 	O(1)
*******************************************************************************/
queue_ty *QueueAppend(queue_ty *queue_dest, queue_ty *queue_src);


#endif /* __QUEUE_H__ */

