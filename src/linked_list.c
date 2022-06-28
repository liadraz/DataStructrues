
/*******************************************************************************
***************************** - LINKED LIST - **********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		linked list Implementation
*	AUTHOR 			Liad Raz
*
*******************************************************************************/


#include <stdlib.h>		/* malloc, free*/
#include <assert.h>		/* assert */

#include "utilities.h" /* INVALID_PTR, DEBUG_MODE, SUCCESS, FUNC_FAILED */
#include "linked_list.h"

#define SIZE_OF_LINK_LIST 	sizeof(llist_ty)
#define SIZE_OF_NODE		sizeof(node_ty)

#define ASSERT_IS_ALLOC(ptr)									\
		assert (0 != (llist_ty *)ptr && "LIST is not allocated");

#define SUCCESS 0
#define FUNC_FAILED 2

struct node
{
    void *data; 
    node_ty *next; 
}; 

/* IMPORTANT end points to the TAIL dummy node; Its data points the last node */
/* last node will always be the node before the Tail dummy */
struct llist
{
    node_ty *head;
    node_ty *end;
    
	DEBUG_MODE(int version_number;)
}; 

/* side function */
static node_ty *CreateNode(void);
static llist_itr_ty TailDummy(llist_itr_ty iterator);


/*******************************************************************************
****************************** LList Create ***********************************/
llist_ty *LListCreate(void)
{
	/* Create handle */
	llist_ty *list = (llist_ty *)malloc(SIZE_OF_LINK_LIST);
	node_ty *tail = NULL;
	
	if (NULL == list)
	{
		return NULL;
	}
	
	/* Allocate two dummy nodes HEAD and TAIL */
	list->head = CreateNode();
	if (NULL == list->head)
	{
		free(list);
		return NULL;
	}
	
	tail = CreateNode();
	if (NULL == tail)
	{
		free(list->head);
		free(list);
		return NULL;
	}	
	
	/* Init HEAD dummy */
	list->head->data = INVALID_PTR;
	list->head->next = tail;
	
	/* Init TAIL dummy; data will store the address of the last node */
	tail->data = list->head;
	tail->next = NULL;
	
	/* Init end node member; Will always points the TAIL dummy */
	list->end = tail;
	
	return list;
}

/*******************************************************************************
***************************** LList Destroy ***********************************/
void LListDestroy(llist_ty *llist)
{
	node_ty *node_to_free = NULL;
	node_ty *list_holder = NULL;
	
	ASSERT_IS_ALLOC(llist);
	
	/* Start from the Head dummy */
	list_holder = llist->head;
	
	while (NULL != list_holder)
	{
		node_to_free = list_holder;
		list_holder = list_holder->next;
		
		DEBUG_MODE(
		node_to_free->data = INVALID_PTR;
		node_to_free->next = INVALID_PTR;
		)
		free(node_to_free);
	}
	
	DEBUG_MODE(
	llist->end = INVALID_PTR;
	)
	free(llist);
}


/*******************************************************************************
***************************** LList Insert ************************************/
/* NOTICE 	The actual iterator points to one before it */
/* 			Insertion adds a node after the actual iterator */
llist_itr_ty LListInsert(llist_itr_ty iterator, void *user_input)
{
	node_ty *new_node = CreateNode();
	llist_itr_ty is_tail_dummy_itr = {NULL};
	
	assert (NULL != iterator.pointing_node && "Iterator is invalid");
	
	/* In case node allocation did not succeed */
	if (NULL == new_node)
	{
		return TailDummy(iterator);
	}
	
	/* new node will point to the next node from the actual iterator */
	new_node->data = user_input;
	new_node->next = iterator.pointing_node->next;
	
	/* connect the actual iterator to the new_node */
	iterator.pointing_node->next = new_node;
	
	/* Inserting a node at the end of the list invalidates it 
	=> END data must be updated */
	is_tail_dummy_itr.pointing_node = iterator.pointing_node->next->next;
	if (is_tail_dummy_itr.pointing_node->next == NULL)
	{
		is_tail_dummy_itr.pointing_node->data = iterator.pointing_node->next;
	}
	
	return iterator;
}


/*******************************************************************************
***************************** LList Remove ************************************/
/* NOTICE The actual iterator points to one before it */
llist_itr_ty LListRemove(llist_itr_ty iterator)
{
	/* Point the actual node that will be removed */
	node_ty *node_to_remove = iterator.pointing_node->next;
	llist_itr_ty is_tail_dummy_itr = {NULL};
	
	assert (node_to_remove->next != NULL && "List is empty nothing to remove");
	
	/* connect current iterator to point the node follows node_to_remove */
	iterator.pointing_node->next = node_to_remove->next;
	
	DEBUG_MODE(
		node_to_remove->data = INVALID_PTR;
		node_to_remove->next = INVALID_PTR;
	)
	free(node_to_remove);
	
	/* Removing a node at the end of the list invalidates it 
	=> END data must be updated */
	is_tail_dummy_itr.pointing_node = iterator.pointing_node->next;
	if (is_tail_dummy_itr.pointing_node->next == NULL)
	{
		is_tail_dummy_itr.pointing_node->data = iterator.pointing_node;
	}
	
	return iterator;
}

/*******************************************************************************
***************************** LList Begin *************************************/
/* Begin Iterator Actually Points to the HEAD DUMMY */
llist_itr_ty LListBegin(const llist_ty *llist)
{
	llist_itr_ty begin_itr = {NULL};
	
	ASSERT_IS_ALLOC(llist);
	
	begin_itr.pointing_node = llist->head;
	
	return begin_itr;
}

/*******************************************************************************
***************************** LList END ***************************************/
/* End Iterator points the Last node (One before the TAIL DUMMY) */
llist_itr_ty LListEnd(const llist_ty *llist)
{
	llist_itr_ty end_itr = {NULL};
	
	ASSERT_IS_ALLOC(llist);
	
	end_itr.pointing_node = llist->end->data;
	
	return end_itr;
}

/*******************************************************************************
***************************** LList IsEmpty ***********************************/
int LListIsEmpty(const llist_ty *llist)
{
	ASSERT_IS_ALLOC(llist);
	
	return ((llist->head->next)->next == NULL);
}


/*******************************************************************************
***************************** LList Count *************************************/
size_t LListCount(const llist_ty *llist)
{
	size_t counter = 0;

	llist_itr_ty itr_runner = {NULL};
	llist_itr_ty itr_end = {NULL};
	
	ASSERT_IS_ALLOC(llist);
	
	itr_runner = LListBegin(llist);
	itr_end = LListEnd(llist);
	
	while (!(LListIsSameIter(itr_runner, itr_end)))
	{
		++counter;
		itr_runner = LListNext(itr_runner);
	}
	
	return counter;
}

/*******************************************************************************
******************************* LList Next ************************************/
llist_itr_ty LListNext(llist_itr_ty iterator)
{
	assert (iterator.pointing_node->next != NULL && "Next: Out of boundary");
	
	iterator.pointing_node = iterator.pointing_node->next;
	
	return iterator;
}

/*******************************************************************************
****************************** LList GetData **********************************/
void *LListGetData(llist_itr_ty iterator)
{
	return iterator.pointing_node->next->data;
}

/*******************************************************************************
****************************** LList SetData **********************************/
void LListSetData(llist_itr_ty iterator, void *user_input)
{
	assert (NULL != iterator.pointing_node->next && "List is Empty");
	
	LListNext(iterator);
	iterator.pointing_node->data = user_input;
}

/*******************************************************************************
****************************** LList IsSame ***********************************/
int LListIsSameIter(llist_itr_ty iterator1, llist_itr_ty iterator2)
{
	DEBUG_MODE(if (iterator1.llist_ty != iterator2.llist_ty)
	{
		return 0;
	})
	
	return (iterator1.pointing_node == iterator2.pointing_node);
}


/*******************************************************************************
***************************** LList ForEach ***********************************/
int LListForEach(llist_itr_ty from, llist_itr_ty to, void *param, ExeFunc exe_func)
{
	int err_check = SUCCESS; 
	
	assert (NULL != from.pointing_node->next && "iterator_from is: Out of boundary");
	assert (NULL != to.pointing_node->next && "iterator_to is: Out of boundary");

	while (!LListIsSameIter(from, to))
	{
		LListNext(from);
		
		err_check = exe_func(from.pointing_node->data, param);

		if (FUNC_FAILED == err_check)
		{
			return err_check;
		}
	}
	
	return err_check; /* 0 For SUCCESS */
}


/*******************************************************************************
******************************* LList Find ************************************/
llist_itr_ty LListFind(llist_ty *llist, void *param, IsMatchFunc is_match)
{
	node_ty *node_runner = llist->head;
	llist_itr_ty compared_iterator = {NULL};
	int result = 1; /* 0 when both values are equal */
	
	ASSERT_IS_ALLOC(llist);
	
	/* traverse list until last node */
	while (NULL != node_runner->next->next)
	{
		node_runner = node_runner->next;
		result = is_match(node_runner->data, param);
		
		if (!result)
		{
			compared_iterator.pointing_node = node_runner;
			
			DEBUG_MODE(compared_iterator.llist_ty = llist;)
			
			return compared_iterator;
		}
	}
	
	/* If not found return iterator to dummy node */
	return LListNext(LListEnd(llist));
}


/*******************************************************************************
***************************** LListsAppend ************************************/
llist_ty *LListsAppend(llist_ty *dest, llist_ty *src)
{
	node_ty *dest_last = NULL;
	node_ty *src_first = NULL;
	node_ty *end_tmp = NULL;
	
	assert (NULL != dest && "dest LIST is not allocated");
	assert (NULL != src && "src LIST is not allocated");
	
	/* get points of contact - dest last node -> src first node  */
	dest_last = dest->end->data;
	src_first = dest->head->next;
	
	/* connect dest last node with src first node */
	dest_last->next = src_first;

	/* aim src list head to dest TAIL dummy*/
	src->head->next = dest->end;
	
	/* Update both lists end member */
	end_tmp->data = dest;
	dest->end = src->end;
	src->end = end_tmp;
	
	/* TODO Updated Tail dummies data TODO */
	
	return dest;
}


/*******************************************************************************
***************************** Util Functions **********************************/
static node_ty *CreateNode(void)
{
	node_ty *node = (node_ty *)malloc(SIZE_OF_NODE);
	
	if (NULL == node)
	{
		return NULL;
	}
	
	return node;
}

static llist_itr_ty TailDummy(llist_itr_ty iterator)
{
	while (NULL != iterator.pointing_node->next)
	{
		iterator = LListNext(iterator);
	}
	
	return iterator;
}


/*******************************************************************************
	LList Create 
	
		malloc stract link_list_ty
		invoke createNode function which 
			(malloc two dummy nodes HEAD and TAIL)
		
		assign HEAD to point TAIL
		
--------------------------------------------------------------------------------
	 
	LList Destroy
		
		define node_to_free init with HEAD address
		define list_holder
		
		run loop on each node from head to tail
			assign the list_holder with the following node_to_free
			free node_to_free
			asssign node_to_free with list_holder
	
			
		free linklist stract (points to head)
		
		on DEBUG MODE only assign invalid addresses to the free nodes
		and linklist struct
		
		o(n)
		
--------------------------------------------------------------------------------

	LList IsEmpty
	
	check if list head member points to tail node which points to invalid address
	in memory.
	
--------------------------------------------------------------------------------

	LList Insert
	
	if list is empty => iterator should point tail
	
	create new_node
	assign to its data member the input from the user
	
	1. new node points to next_node
	get the address of the next node that iterator points at =>
	and assign it to new_node
	
	2. change iterator to to point new_node
	
	return iterator
	
--------------------------------------------------------------------------------

	LList Remove
	
	define node_to_remove pointer
	init it with the next node pointed by iterator
	
	change current iterator to point at the address node_to_remove points.
	
	free to_remove node
	
	return iterator		
	
--------------------------------------------------------------------------------

	LList Count
	
	define counter init with 0
	
	iterate each node until getting to TAIL (invalid iterator)
		increment counter 
	
	return (counter minus thw two dummies)

--------------------------------------------------------------------------------

	LList Begin
	
	define begin_itr iterator
	assign begin_itr to point list head node.
	
	return iterator


	LList END
	
	define end_itr iterator
	define node_ruuner init to point HEAD
	
	iterate from linklist HEAD member until reaching NULL 
		keep the last node
	
	assign end_itr to point to the followed current node.
	
	return end_itr

--------------------------------------------------------------------------------

	LList Next
	
	return iterator incremented by one

--------------------------------------------------------------------------------

	LList GetData
	
	return the data from node that pointed by the iterator
	
	
--------------------------------------------------------------------------------
	LListsAppend
	
		get the last node in dest
		connect it with the first node in src 

		remove the tail dummy in dest
		remove the head dummy in src
		
*******************************************************************************/
