/*******************************************************************************
****************************** - BINARY_TREE - *********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		API Binary Sorted Tree
*	AUTHOR			Liad Raz
*	FILES			binary_tree.c binary_tree_test.c binary_tree.h
* 
*******************************************************************************/

#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__	

#include "utilities.h"

typedef struct bst bst_ty;
typedef struct bst_itr bst_itr_ty;


/*******************************************************************************
* DESCRIPTION	Used in BSTCreate, BSTFind
* RETURN		0 or Positive SUCCESS 	(data_a >= data_b); 
				negative FAILURE 		data_a < data_b)
*******************************************************************************/
typedef int (*cmp_func_ty)(const void *data_a, const void *data_b, const void *param);

/*******************************************************************************
* DESCRIPTION	Creates binary tree container.
* RETURN		NULL if memory allocation failed.
				NULL if provided function does not exist.
* IMPORTANT	 	User needs to destroy the allocated container.
*
* Time Complexity 	O(1)
*******************************************************************************/
bst_ty *BSTCreate(cmp_func_ty CmpFunc_, void *param_);


/*******************************************************************************
* DESCRIPTION	Frees binary tree container.
*
* Time Complexity 	O(n)
*******************************************************************************/
void BTSDestroy(bst_ty *th_);


/*******************************************************************************
* DESCRIPTION	Add new element to the tree and position it in the correct location.
* RETURN		Invalid iterator (iterator that refers to END) if insertion fails.
* IMPORTANT	 	Undefined behavior when data already exists in binary tree.
*
* Time Complexity 	O(log n)
*******************************************************************************/
bst_itr_ty BSTInsert(bst_ty *th_, void *new_data_);


/*******************************************************************************
* DESCRIPTION	Remove a provided iterator.
* RETURN		next element
* IMPORTANT		Undefined behavior 
				- when trying to remove an invalid iterator.
				- when iterator refereed to an element that does not exist in tree.	
*
* Time Complexity 	O(1)
*******************************************************************************/
bst_itr_ty BSTRemove(bst_itr_ty to_remove_);


/*******************************************************************************
* DESCRIPTION	Count how many elements exists in the binary tree.
*
* Time Complexity 	O(n)
*******************************************************************************/
size_t BSTSize(const bst_ty *th_);


/*******************************************************************************
* DESCRIPTION	Check if binary consists elements.
* RETURN		boolean => 	1 EMPTY; 0 NOT EMPTY.
*
* Time Complexity 	O(1)
*******************************************************************************/
int BSTIsEmpty(const bst_ty *th_);


/*******************************************************************************
* DESCRIPTION	Get iterator to the smallest priority's value element.
* RETURN		Invalid iterator in case tree is empty
* IMPORTANT		Undefined behavior when trying to GetPrev
*
* Time Complexity 	O(n)
*******************************************************************************/
bst_itr_ty BSTBegin(bst_ty *th_);


/*******************************************************************************
* DESCRIPTION	Get iterator to an invalid iterator,
				(One after the biggest priority's value element).
* IMPORTANT		Undefined behavior when trying to get its data.
*
* Time Complexity 	O(1)
*******************************************************************************/
bst_itr_ty BSTEnd(bst_ty *th_);


/*******************************************************************************
* DESCRIPTION	Get iterator to a smaller element. 
* IMPORTANT		Undefined behavior when trying to prev the Begin iterator.
*
* Time Complexity 	O(1)
*******************************************************************************/
bst_itr_ty BSTPrev(bst_itr_ty where_);


/*******************************************************************************
* DESCRIPTION	Get iterator to a bigger element
* IMPORTANT		Undefined behavior when trying to next the END iterator.
*
* Time Complexity 	O(1)
*******************************************************************************/
bst_itr_ty BSTNext(bst_itr_ty where_);


/*******************************************************************************
* DESCRIPTION	Check if two iterators are equal.
* RETURN		boolean => 1 EQUAL;	0 NOT_EQUAL
*
* Time Complexity 	O(1)
*******************************************************************************/
int BSTIterIsEqual(bst_itr_ty itr1, bst_itr_ty itr2);


/*******************************************************************************
* DESCRIPTION	Get data of the current element.
* IMPORTANT		Undefined behavior 
				- When iterator is invalid.
				- Trying to invoke on iterator refers to END.
				- Trying to change the returned data.
*
* Time Complexity 	O(1)
*******************************************************************************/
void *BSTGetData(bst_itr_ty where_);


/*******************************************************************************
* DESCRIPTION	Match element's data in binary tree with data provided by the user.
* RETURN		Iterator to the first found; If not found iterator to the end.

* Time Complexity 	O(log n)
*******************************************************************************/
bst_itr_ty BSTFind(bst_ty *th_, const void *data_);


/*******************************************************************************
* DESCRIPTION	Used in BSTForeach.
* RETURN		status => 0 SUCCESS; non-zero value FAILURE

* Time Complexity 	O(1)
*******************************************************************************/
typedef int (*exe_func_ty)(void *data_, void *param_);

/*******************************************************************************
* DESCRIPTION	Traverse in range and execute a given function on each element.
* RETURN 		status => 0 SUCCESS; non-zero value FAILURE (returned by ExeFunc)
* IMPORTANT		Undefined behavior 
				- when iterators refer to different binary trees.
*				- when from_ iterator refers to the END of binary tree.

* Time Complexity 	O(n)
*******************************************************************************/
int BSTForEach(bst_itr_ty from_, bst_itr_ty to_, exe_func_ty ExeFunc_, void *param_);




/*******************************************************************************
>>>>>>>>>>>>>>>>>>>>>>>>> AREA 51 - Restricted AREA <<<<<<<<<<<<<<<<<<<<<<<<<<*/

typedef struct node node_ty;

struct bst_itr
{
	node_ty *node;
	
	DEBUG_MODE(bst_ty *tree;)
};



/*******************************************************************************
* DESCRIPTION	Print Tree
*
*******************************************************************************/
void PrintTree(bst_ty *tree);

int PrintTreeRecursive(node_ty *node, int is_left, int offset, int depth, char s[20][255]);


#endif /* __BINARY_TREE_H__ */

