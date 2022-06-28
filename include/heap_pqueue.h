/*******************************************************************************
****************************** - HEAP_PQUEUE - ********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		API
*	AUTHOR 			Liad Raz
*	FILES			heap_pqueue.c heap_pqueue_test.c heap_pqueue.h
* 
*******************************************************************************/

#ifndef __HEAP_PQUEUE_H__
#define __HEAP_PQUEUE_H__

typedef struct pqueue pqueue_ty;

/*******************************************************************************
* DESCRIPTION	Used in Create
* RETURN		0 SUCCESS; POSITIVE value obj1 > obj2; NEGATIVE value obj1 < obj2
*******************************************************************************/
typedef int (*PQCmpFunc)(const void *object1, const void *object2, const void *cmp_param);

/*******************************************************************************
* DESCRIPTION	Creates pqueue container.			
* RETURN		NULL when memory allocation failed.
				Undefined behavior when cmp_func_p is invalid	 	
* IMPORTANT		User needs to free the allocated list.	 	
*
* Time Complexity 	O(1)
*******************************************************************************/
pqueue_ty *HeapPQueueCreate(PQCmpFunc cmp_func_p, const void *cmp_param);

/*******************************************************************************
* DESCRIPTION	Free priority pqueue.
		
* Time Complexity   O(pqueue_size)
*******************************************************************************/
void HeapPQueueDestroy(pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Add new element and position it based on its unique ID.
* RETURN		status => 0 SUCCESS; non-zero value FAILURE
	
* Time Complexity   O(log n), Worst Case O(n)
*******************************************************************************/
int HeapPQueueEnqueue(pqueue_ty *pqueue, void *data);

/*******************************************************************************		
* DESCRIPTION	Remove element from priority pqueue and frees it from memory.

* Time Complexity   O(log n)
*******************************************************************************/
void HeapPQueueDequeue(pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Get the value with the highest priority in pqueue.
	
* Time Complexity   O(1)
*******************************************************************************/
void *HeapPQueuePeek(const pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Checks if elements are stored in pqueue
* RETURN		boolean => 	1 EMPTY; 0 NOT EMPTY.
		
* Time Complexity   O(1)
*******************************************************************************/
int HeapPQueueIsEmpty(const pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Obtain the number of elements in the pqueue.
		
* Time Complexity   O(1)
*******************************************************************************/
size_t HeapPQueueSize(const pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Remove all elements in pqueue.
		
* Time Complexity   O(pqueue_size)
*******************************************************************************/
void HeapPQueueClear(pqueue_ty *pqueue);

/*******************************************************************************
* DESCRIPTION	Used in PQueueErase
* RETURN		boolean => 1 FOUND;	0 NOT_FOUND
*******************************************************************************/
typedef int (*PQIsMatch)(const void *element_data, const void *param);

/*******************************************************************************
* DESCRIPTION	Remove specific element in pqueue.
* RETURN		NULL if cmp_param is not found
			Undefined behavior when match_func_p is invalid.
	
* Time Complexity   O(pqueue_size)
*******************************************************************************/
void *HeapPQueueErase(pqueue_ty *pqueue, PQIsMatch match_func_p, void *cmp_param);


#endif /* __HEAP_PQUEUE_H__ */

