/*******************************************************************************
****************************** - HASH_TABLE - ********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Hash Table API
*	AUTHOR 			Liad Raz
*	FILES			hash_table.c hash_table_test.c hash_table.h
* 
*******************************************************************************/

#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stddef.h>			/* size_t */

#include "dlinked_list.h"

typedef struct hash_table 		ht_ty;
typedef struct hash_table_itr 	ht_itr_ty;


/*******************************************************************************
* DESCRIPTION	Generate an hash value using provided data.
				/ Used in HashTableCreate Function
* RETURN	 	hash value on SUCCESS;
*
* Time Complexity 	O(n)
*******************************************************************************/
typedef size_t (*hash_func_ty)(const void *data, const void *param);

/*******************************************************************************
* DESCRIPTION	In case of collision, Checks if two data values has same key.
				/ Used in HashTableCreate and HashTableFind Functions.
				
* RETURN	 	boolean => 1 EQUAL;	0 NOT_EQUAL
*******************************************************************************/
typedef int (*is_same_key_ty)(const void *key1, const void *key2);

/*******************************************************************************
* DESCRIPTION	Creates an hash table container.
* RETURN	 	NULL at memory allocation failure.
* IMPORTANT	 	User needs to free the allocated container (use Destory func).
				It is recommended to provide table_size as a Prime number.
*
* Time Complexity 	O(n)
*******************************************************************************/
ht_ty *HashTableCreate(hash_func_ty HashFunc, size_t table_size, is_same_key_ty IsSameKey, const void *param);


/*******************************************************************************
* DESCRIPTION	Frees hash table container.
*
* Time Complexity 	O(n)
*******************************************************************************/
void HashTableDestroy(ht_ty *hash_table);


/*******************************************************************************
* DESCRIPTION	Adds a new element to the hash table.
* RETURN	 	On failure func returns an invalid iterator (iterator to END).
* IMPORTANT	 	Undefined behavior when data already exists in binary tree.
*
* Time Complexity 	O(1); 	Worst O(n);
*******************************************************************************/
ht_itr_ty HashTableInsert(ht_ty *hash_table, void *to_add);


/*******************************************************************************
* DESCRIPTION	Removes a provided iterator.
* RETURN	 	On failure func returns an invalid iterator (iterator to END).
* IMPORTANT		Undefined behavior 
				- when trying to remove an invalid iterator.
				- when iterator refers to an element that does not exist
				in the hash table.	
*
* Time Complexity 	O(1)
*******************************************************************************/
ht_itr_ty HashTableRemove(ht_itr_ty to_remove);


/*******************************************************************************
* DESCRIPTION	Obtain amount of elements exist in the hash table.
*
* Time Complexity 	O(elements)
*******************************************************************************/
size_t HashTableCount(ht_ty *hash_table);


/*******************************************************************************
* DESCRIPTION	Check the existence of elements in the hash table.
* RETURN	 	boolean => 1 IS_EMPTY;	0 NOT_EMPTY
*
* Time Complexity 	O(size_of_table)
*******************************************************************************/
int HashTableIsEmpty(ht_ty *hash_table);


/*******************************************************************************
* DESCRIPTION	Match hash table element with a data provided by the user.
* RETURN	 	Iterator to the first Found; iterator to END when Not Found 
*
* Time Complexity 	O(1)
*******************************************************************************/
ht_itr_ty HashTableFind(ht_ty *hash_table, const void *to_find);


/*******************************************************************************
* DESCRIPTION	Get iterator to the first valid element.
* IMPORTANT	 	If hash table is empty, return an invalid iterator 
				(iterator to END).
*
* Time Complexity 	O(size_of_table)
*******************************************************************************/
ht_itr_ty HashTableBegin(ht_ty *hash_table);


/*******************************************************************************
* DESCRIPTION	Get iterator to the end of range.
* RETURN	 	An invalid iterator.
*
* Time Complexity 	O(1)
*******************************************************************************/
ht_itr_ty HashTableEnd(ht_ty *hash_table);


/*******************************************************************************
* DESCRIPTION	An iterator to the next element.
* IMPORTANT	 	Undefined behavior when trying to next the last element.
*
* Time Complexity 	O(1)
*******************************************************************************/
ht_itr_ty HashTableNext(ht_itr_ty itr);


/*******************************************************************************
* DESCRIPTION	An iterator to the previous element.
* IMPORTANT	 	Undefined behavior when trying to prev the first element.
*
* Time Complexity 	O(1)
*******************************************************************************/
ht_itr_ty HashTablePrev(ht_itr_ty itr);


/*******************************************************************************
* DESCRIPTION	Get data of a specifiec element.
* IMPORTANT	 	Undefined behavior when iterator refers to bad iterator.
*
* Time Complexity 	O(1)
*******************************************************************************/
void *HashTableGetData(ht_itr_ty itr);


/*******************************************************************************
* DESCRIPTION	Compare if two iterators are equal.
* RETURN		boolean => 	1 SAME; 0 DIFFERENT.
*
* Time Complexity 	O(1)
*******************************************************************************/
int HashTableIsSameIter(ht_itr_ty itr1, ht_itr_ty itr2);


/*******************************************************************************
* DESCRIPTION	Check if iterator is not Valid.
*
* Time Complexity 	O(1)
*******************************************************************************/
int HashTableIsBadIter(ht_itr_ty itr);




/*******************************************************************************
>>>>>>>>>>>>>>>>>>>>>>>>> AREA 51 - Restricted AREA <<<<<<<<<<<<<<<<<<<<<<<<<<*/

struct hash_table_itr
{
	dlist_itr_ty element_itr;
	ht_ty *hash_table;
	size_t table_index;
};


#endif /* __HASH_TABLE_H__ */

