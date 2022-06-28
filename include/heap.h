/*******************************************************************************
********************************* - HEAP - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		API
*	AUTHOR 		    Liad Raz
*	FILES			heap.c heap_test.c heap.h
* 
*******************************************************************************/

#ifndef __HEAP_H__
#define __HEAP_H__


typedef struct heap heap_ty;


/*******************************************************************************
* DESCRIPTION     Compare two data variables. Used in HeapCreate Func.
* RETURN          0 or Positive SUCCESS   (data_a >= data_b); 
                  negative FAILURE        (data_a < data_b)
*******************************************************************************/
typedef int (*heap_cmp_ty)(const void *data_a, const void *data_b, const void *param);

/*******************************************************************************
* DESCRIPTION	Creates a heap container.
* RETURN		NULL if memory allocation failed.
                  NULL if provided function does not exist.
* IMPORTANT	 	User needs to destroy the allocated container.
*
* Time Complexity 	O(1)
*******************************************************************************/
heap_ty *HeapCreate(heap_cmp_ty heap_cmp_func ,const void *param);


/*******************************************************************************
* DESCRIPTION	Frees the heap container.
*
* Time Complexity 	O(1)
*******************************************************************************/
void HeapDestroy(heap_ty *heap);


/*******************************************************************************
* DESCRIPTION	Insert element to the heap in the correct location.
* RETURN          status => 0 SUCCESS; non-zero value FAILURE.
* IMPORTANT	 	Undefined behavior when data already exists in the heap.
*
* Time Complexity 	O(log n), Worst Case O(n)
*******************************************************************************/
int HeapPush(heap_ty *heap, void *to_add);


/*******************************************************************************
* DESCRIPTION     Remove the root element of the heap.
* IMPORTANT       Undefined behavior when trying to pop an empty heap.
*
* Time Complexity 	O(log n)
*******************************************************************************/
void HeapPop(heap_ty *heap);


/*******************************************************************************
* DESCRIPTION	Get the root's data from the heap.
* IMPORTANT	 	Undefined behavior when trying to peek data from an empty heap.
*
* Time Complexity 	O(1)
*******************************************************************************/
void *HeapPeek(const heap_ty *heap);


/*******************************************************************************
* DESCRIPTION	Count how many elements exists in heap.
*
* Time Complexity 	O(1)
*******************************************************************************/
size_t HeapSize(const heap_ty *heap);


/*******************************************************************************
* DESCRIPTION	Check if heap consists elements.
* RETURN	 	boolean => 	1 EMPTY; 0 NOT EMPTY
*
* Time Complexity 	O(1)
*******************************************************************************/
int HeapIsEmpty(const heap_ty *heap);


/*******************************************************************************
* DESCRIPTION	Used in HeapRemove; Remove The First matched data.
* RETURN	 	boolean => 	1 Removed; 0 NOT Removed
*******************************************************************************/
typedef int (*should_remove_ty)(const void *data, const void *param);

/*******************************************************************************
* DESCRIPTION	Remove the first encountered element that 
                  satisfies the user param, and should_remove Callback function. 
* RETURN	 	Null when remove fails.
*
* Time Complexity 	O(n)
*******************************************************************************/
void *HeapRemove(heap_ty *heap, should_remove_ty remove_func, const void *param);


#endif /* __HEAP_H__ */

