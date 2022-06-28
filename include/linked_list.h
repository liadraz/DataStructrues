/*******************************************************************************
***************************** - LINKED LIST - **********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		linked list API
*	AUTHOR			Liad Raz
*
*******************************************************************************/
#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stddef.h> /* size_t */
#include "utilities.h" /* DEBUG_MODE */


typedef struct llist llist_ty; 
typedef struct llist_itr llist_itr_ty;


/*******************************************************************************
* DESCRIPTION	Creates a linked list container.
* RETURN		NULL if memory allocation failed
* IMPORTANT	 	User needs to free the allocated container.

* Time Complexity 	O(1)
*******************************************************************************/
llist_ty *LListCreate(void); 


/*******************************************************************************
* DESCRIPTION	Frees linked list container

* Time Complexity 	O(list_size)
*******************************************************************************/
void LListDestroy(llist_ty *llist); 


/*******************************************************************************
* DESCRIPTION	Adds a new element to the list, consists data from the user
* RETURN		On failure return invalid iterator, (iterator to end).
* IMPORTANT	 	The original iterator will be invalidate
	
* Time Complexity 	O(1); O(list_size) On failure
*******************************************************************************/
llist_itr_ty LListInsert(llist_itr_ty iterator, void *data); 


/*******************************************************************************
* IMPORTANT		Undefined behavior 
				- When removing an invalid iterator.
				- In case list is empty.
*	
* Time Complexity 	O(1)
*******************************************************************************/
llist_itr_ty LListRemove(llist_itr_ty iterator); 


/*******************************************************************************
* DESCRIPTION	Get iterator to the first valid element.
* IMPORTANT	 	If list does not exist, return invalid iterator.
	
* Time Complexity 	O(1)
*******************************************************************************/
llist_itr_ty LListBegin(const llist_ty *llist); 


/*******************************************************************************
* DESCRIPTION	Get iterator to the end of range.
* RETURN		An invalid iterator.
	
* Time Complexity 	O(1)
*******************************************************************************/
llist_itr_ty LListEnd(const llist_ty *llist);


/*******************************************************************************
* DESCRIPTION	Checks the existence of elements in the linked list.
* RETURN		boolean => 1 IS_EMPTY;	0 NOT_EMPTY

* Time Complexity 	O(1)
*******************************************************************************/
int LListIsEmpty(const llist_ty *llist); 


/*******************************************************************************
 DESCRIPTION	Obtain the number of elements in linked list

* Time Complexity 	O(list_size)
*******************************************************************************/
size_t LListCount(const llist_ty *llist); 


/*******************************************************************************
* DESCRIPTION	Get iterator to the next element.
* IMPORTANT:	Undefined behavior when iterator reffered to end of list.
	
* Time Complexity 	O(1)
*******************************************************************************/
llist_itr_ty LListNext(llist_itr_ty iterator); 


/*******************************************************************************
* DESCRIPTION	Get data of a specifiec element.
* IMPORTANT	 	Undefined behavior	when iterator reffered to end of list.
	
* Time Complexity 	O(1)
*******************************************************************************/
void *LListGetData(llist_itr_ty iterator); 


/*******************************************************************************
* DESCRIPTION	Change data of a specifiec element.
* IMPORTANT	 	Undefined behavior	when iterator refereed to end of list.
	
* Time Complexity 	O(1)
*******************************************************************************/
void LListSetData(llist_itr_ty iterator, void *user_input); 


/*******************************************************************************
* DESCRIPTION	Evalutes two iterators addresses
* RETURN		boolean => 	1 SAME; 0 DIFFERENT.
	
* Time Complexity 	O(1)
*******************************************************************************/
int LListIsSameIter(llist_itr_ty iterator1, llist_itr_ty iterator2); 


/*******************************************************************************
* DESCRIPTION	Used in ForEach function
* RETURN		status => 	0 SUCCESS; non-zero value FAILURE
*******************************************************************************/
typedef int (*ExeFunc)(void *data, void *param);

/*******************************************************************************
* DESCRIPTION	Iterate each element and invoke a given function on it.
* RETURN 		status => 0 SUCCESS; non-zero value FAILURE (returned by ExeFunc)
* IMPORTANT:	Undefined behavior 
				- when iterators referred to different lists
*				- from iterator reffered to the end of the list.
*
* Time Complexity 	O(range) 
*******************************************************************************/
int LListForEach(llist_itr_ty from, llist_itr_ty to, void *param, ExeFunc p_exe_func); 


/*******************************************************************************
* DESCRIPTION	for Find function
* RETURN		boolean => 	1 MATCH; 0 DIFFERENT.
*******************************************************************************/
typedef int (*IsMatchFunc)(const void *data, void *param); 

/*******************************************************************************
* DESCRIPTION	Match list element data with a data provided by the user.
* RETURN		Iterator to the first found; If not found iterator to the end.
*				In Debug mode - Also the address of the searched linked list.
	
* Time Complexity 	O(list_size)
*******************************************************************************/
llist_itr_ty LListFind(llist_ty *llist, void *param, IsMatchFunc match_func_p); 


/*******************************************************************************
* DESCRIPTION	Remove elements from source list, "from" to "to" (not included) 
				and place them in target list at "where" location.
* RETURN		An iterator to the start of the newly added portion.
* IMPORTANT:	Undefined behavior
*				- In case target_where iterator is inside the source range.
*				- src_from and src_to are not from the same list.
*				- src_from iterator refereed to the end.
*				- src_from iterator is located after src_to.
					
* Time Complexity 	O(1)
*******************************************************************************/
llist_ty *LListsAppendAndEmpty(llist_ty *to_append_to, llist_ty *to_empty); 
/*
llist_ty *LListsSpliceAndEmpty(llist_itr_ty target_where, llist_itr_ty src_from, llist_itr_ty src_to); */



/*******************************************************************************
>>>>>>>>>>>>>>>>>>>>>>>>> AREA 51 - Restricted AREA <<<<<<<<<<<<<<<<<<<<<<<<<<*/

typedef struct node node_ty;

struct llist_itr 
{
    node_ty *pointing_node;

    DEBUG_MODE(llist_ty *llist_ty;)
};

#endif /* __LINKED_LIST_H__ */
