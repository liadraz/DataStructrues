
/*******************************************************************************
****************************** - BST_REC - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		API Binary Sort Tree (recursive ) - AVL Tree.
*	AUTHOR 		    Liad Raz
*	FILES			bst_rec.c bst_rec_test.c bst_rec.h
* 
*******************************************************************************/

#ifndef __BST_REC_H__
#define __BST_REC_H__

#include "dlinked_list.h"

typedef struct avl      avl_ty;
/* Traversing Order options => PRE: Root-L-R; IN: L-Root-R; POST L-R-Root */
typedef enum ordering { PRE, IN, POST } ordering_ty;


/*******************************************************************************
* DESCRIPTION     Compare two data variables. Used in AVLCreate Func.
* RETURN	 	0 or Positive SUCCESS   (data_a >= data_b); 
                  negative FAILURE        (data_a < data_b)
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
avl_ty *AVLCreate(cmp_func_ty CmpFunc, const void *param);


/*******************************************************************************
* DESCRIPTION	Frees binary tree container.
*
* Time Complexity 	O(n)
*******************************************************************************/
void AVLDestory(avl_ty *avl);


/*******************************************************************************
* DESCRIPTION	Add new element to the tree and position it in the correct location.
* RETURN	 	status => 0 SUCCESS; non-zero value FAILURE.
* IMPORTANT	 	Undefined behavior when data already exists in binary tree.
*
* Time Complexity 	O(log n)
*******************************************************************************/
int AVLInsert(avl_ty *avl, void *new_data);


/*******************************************************************************
* DESCRIPTION	Remove a provided iterator.
* IMPORTANT		Undefined behavior 
                  - when trying to remove data that does not exists.

* Time Complexity 	O(log n)
*******************************************************************************/
void AVLRemove(avl_ty *avl, const void *data_to_remove);


/*******************************************************************************
* DESCRIPTION	Count how many elements exists in the binary tree.
*
* Time Complexity 	O(size)
*******************************************************************************/
size_t AVLSize(const avl_ty *avl);


/*******************************************************************************
* DESCRIPTION	Check if binary consists elements.
* RETURN		boolean => 	1 EMPTY; 0 NOT EMPTY. 	
*
* Time Complexity 	O(1)
*******************************************************************************/
int AVLIsEmpty(const avl_ty *avl);


/*******************************************************************************
* DESCRIPTION	Match element's data in binary tree with data provided by the user.
* RETURN	 	Data to the found element; NULL If not found.
*
* Time Complexity 	O(log n)
*******************************************************************************/
void *AVLFind(const avl_ty *avl, const void *key);


/*******************************************************************************
* DESCRIPTION	Execute this provided function. 
                  Used in AVLForEach.
* RETURN	 	status => 0 SUCCESS; non-zero value FAILURE
*******************************************************************************/
typedef int (*exe_func_ty)(void *data , const void *param);

/*******************************************************************************
* DESCRIPTION	Traverse the tree and execute a given function on each element.
                  The user can choose the traversing order in which ForEach will operate - parameter ordering (PRE, IN, POST).
* RETURN	 	status => 0 SUCCESS; non-zero value FAILURE (returned by ExeFunc)
*
* Time Complexity 	O(n)
*******************************************************************************/
int AVLForEach(const avl_ty *avl, ordering_ty ordering, exe_func_ty ExeFunc ,const void *param);


/*******************************************************************************
* DESCRIPTION	Get the height of a given tree.
*
* Time Complexity 	O(1)
*******************************************************************************/
size_t AVLHeight(const avl_ty *avl);


/*******************************************************************************
* DESCRIPTION	Used in AVLRemoveIf.
* RETURN	 	status => 0 Match; non-zero value NOT Match. 
*******************************************************************************/
typedef int (*match_func_ty)(const void *data, const void *param);

/*******************************************************************************
* DESCRIPTION	Remove specifec elements that match the provided param and function
                  and insert these elements into a linked list.
* RETURN	 	status => 0 SUCCESS; non-zero value FAILURE.
* IMPORTANT	 	Undefined behavior
*
* Time Complexity 	O(size)
*******************************************************************************/
int AVLRemoveIf(avl_ty *avl, match_func_ty ShouldRemove, const void *param, const dlist_ty *removed_elements);




/*******************************************************************************
* DESCRIPTION	Print Tree
*                 Tests Only
*******************************************************************************/
void PrintTree(avl_ty *tree);

#endif /* __BST_REC_H__ */

