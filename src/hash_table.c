
/*******************************************************************************
****************************** - HASH_TABLE - **********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation of Hash Table container
*	AUTHOR			Liad Raz
*
*******************************************************************************/

#include <stdio.h>			/* stderr */
#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */

#include "utilities.h"
#include "hash_table.h"


/* Struct of hash table */
struct hash_table
{
	dlist_ty **m_lists;			/* Array of pointers to dlists */
	size_t m_htsize;			/* The table size */
	hash_func_ty hash_func;		/* Hash Value generator */
	is_same_key_ty is_same_key;	/* Used in Find function */
	const void *const m_param;	/* Used in the hash_func functions */
};


/* Auxiliary Functions */
static ht_itr_ty WrapToHashIMP(ht_ty *th_, size_t index_, dlist_itr_ty dlist_itr_);
static size_t HashToIndexIMP(ht_ty *th_, const void *to_add_);
ht_itr_ty GetBadIterIMP(size_t table_size_);


/*******************************************************************************
***************************** HashTableCreate *********************************/
ht_ty *HashTableCreate(hash_func_ty hash_func_, const size_t table_size_, \
	is_same_key_ty is_same_key_, const void *param_)
{
	ht_ty *ht = NULL;
	dlist_ty **lists = NULL;
	dlist_ty **runner = NULL;
	size_t i = 0;
	
	/* assert Functions are invalid */
	assert (NULL != hash_func_ && "HashTableCreate: Function pointer is invalid");
	assert (NULL != is_same_key_ && "HashTableCreate: Function pointer is invalid");
	assert (0 != table_size_ && "HashTableCreate: Size Cannot be Zero");
	
	/* Allocate memory for hash table struct */
	ht = (ht_ty *)malloc(sizeof(ht_ty));
	RETURN_IF_BAD(ht, "Allocation Faild", NULL);
		
	/* Allocate memory for array of pointers to lists */
	lists = (dlist_ty **)malloc(sizeof(dlist_ty **) * table_size_);
	RETURN_IF_BAD_NESTED(ht, "Allocation Faild", NULL, ht);
	
	/* Create a list for each element in the hash table */
	for (runner = lists; runner < (lists + table_size_); ++runner)
	{
		/* In case, create of list faild destorey the whole hash table */
		*runner = DListCreate();
		if (NULL == *runner)
		{
			HashTableDestroy(ht);
			return NULL;
		}
	}

	/* Init hash table managment struct fields */
	ht->m_lists = lists;
	ht->m_htsize = table_size_;
	ht->hash_func = hash_func_;
	ht->is_same_key = is_same_key_;
	*(void **)&ht->m_param = *(void **)&param_;

	return ht;
}


/*******************************************************************************
***************************** HashTableDestroy ********************************/
void HashTableDestroy(ht_ty *th_)
{
	dlist_ty **to_free = NULL;
	dlist_ty **lists = NULL;
	size_t index = 0;
	size_t table_size = 0;
	
	ASSERT_NOT_NULL(th_, "HashTableDestroy: HashTable is not allocated");
	
	lists = th_->m_lists;
	table_size = th_->m_htsize;

	/* traverse the hash table from backwards */
	for (to_free = lists + (table_size - 1); to_free >= lists; --to_free)
	{
		/* Destroy each allocated list in table */
		DListDestroy(*to_free);
	}

	/* Free dlists array */
	free(th_->m_lists);

	/* Break the allocated struct hash table fields */
	DEBUG_MODE
	(
		th_->m_lists = INVALID_PTR;
		*(void **)&th_->m_param = INVALID_PTR;
	)

	/* Free the hash table sruct */
	free(th_);
}


/*******************************************************************************
***************************** HashTableInsert *********************************/
ht_itr_ty HashTableInsert(ht_ty *th_, void *to_add_)
{
	dlist_itr_ty dlist_ret = {NULL};
	size_t index = 0;
	
	ASSERT_NOT_NULL(th_, 					\
	"HashTableInsert: HashTable is not allocated");
	ASSERT_NOT_NULL(th_->m_lists, 			\
	"HashTableInsert: Array of lists is not allocated");
	
	index = HashToIndexIMP(th_, to_add_);
	
	ASSERT_NOT_NULL(th_->m_lists[index], 		\
	"HashTableInsert: list is not allocated");

	/* Insert the provided data to the doubly link list at index */
	dlist_ret = DListInsert(DListBegin(th_->m_lists[index]), to_add_);
	
	/* Check return value, In case of failure return iterator to END of table */
	if (DListIsSameIter(dlist_ret, DListEnd(th_->m_lists[index])))
	{
		return HashTableEnd(th_);
	}
	
	return WrapToHashIMP(th_, index, dlist_ret);
}


/*******************************************************************************
***************************** HashTableRemove *********************************/
ht_itr_ty HashTableRemove(ht_itr_ty to_remove_)
{
	dlist_itr_ty dlist_ret = {NULL};
	dlist_itr_ty dlist_end = {NULL};
	size_t table_index = to_remove_.table_index;

	/* TODO assert is in range of list */
	ASSERT_NOT_NULL(to_remove_.hash_table, 			\
	"HashTableRemove: HashTable is not allocated");
	
	/* Remove an element from a specific dlist */
	dlist_ret = DListRemove(to_remove_.element_itr);
	
	/* Get the end list iterator where to_remove exists */
	dlist_end = DListEnd(to_remove_.hash_table->m_lists[table_index]);
	
	/* In case, removing list's element failed,
		Return the next hash table element */
	if (DListIsSameIter(dlist_ret, dlist_end))
	{
		table_index = table_index + 1;
		
		dlist_ret = DListBegin(to_remove_.hash_table->m_lists[table_index]);
	}
	
	return WrapToHashIMP(to_remove_.hash_table, table_index, dlist_ret);
}


/*******************************************************************************
***************************** HashTableCount **********************************/
size_t HashTableCount(ht_ty *th_)
{
	dlist_ty **runner = NULL;
	dlist_ty **hash_table_end = NULL;
	size_t counter = 0;
	
	ASSERT_NOT_NULL(th_, "HashTableCount: HashTable is not allocated");
	
	hash_table_end = th_->m_lists + th_->m_htsize;

	/* Traverse each existing element(dlist) in the table */
	for (runner = th_->m_lists; runner < hash_table_end ; ++runner)
	{
		/* Increment counter with DListCount returned value */
		counter += DListCount(*runner);
	}
	
	return counter;	
}


/*******************************************************************************
***************************** HashTableIsEmpty *********************************/
int HashTableIsEmpty(ht_ty *th_)
{
	ht_itr_ty runner = {NULL};
	ht_itr_ty end = {NULL};

	ASSERT_NOT_NULL(th_, "HashTableIsEmpty: HashTable is not allocated");
	
	/* define iterator to begin of hash table */
	runner = HashTableBegin(th_);
	
	/* define iterator to end of hash table */
	end = HashTableEnd(th_);
	
	/* traverse each element(dlist) until end of table or dlist is NOT empty */
	while (!HashTableIsSameIter(runner, end) && !DListIsEmpty(th_->m_lists[runner.table_index]))
	{
		/* advance runner to next element(dlist) in the array */
		runner = HashTableNext(runner);
	}
	
	/* The hash table is empty when runner reached to the END of index */
	return DListIsEmpty(runner.hash_table->m_lists[runner.table_index]);
}

/*******************************************************************************
***************************** HashTableFind ***********************************/
ht_itr_ty HashTableFind(ht_ty *th_, const void *to_find)
{
	dlist_itr_ty begin = {NULL};
	dlist_itr_ty end = {NULL};
	dlist_itr_ty dlist_ret = {NULL};
	ht_itr_ty ret = {NULL};
	size_t index = 0;
		
	ASSERT_NOT_NULL(th_, "HashTableFind: HashTable is not allocated");
	
	/* get the index */
	index = HashToIndexIMP(th_, to_find);
	
	begin = DListBegin(th_->m_lists[index]);
	end = DListEnd(th_->m_lists[index]);
	
	/* Invoke DlistFind on the dlist that occupies in the calculated index */
	dlist_ret = DListFind(begin, end, th_->is_same_key, to_find);
	
	/* In case, data is not in list return an iterator to END */
	if (DListIsSameIter(dlist_ret, end))
	{
		/* TODO return BadItr */
		return HashTableEnd(th_);
	}
	
	ret = WrapToHashIMP(th_, index, dlist_ret);

	return ret;
}


/*******************************************************************************
***************************** HashTableBegin **********************************/
ht_itr_ty HashTableBegin(ht_ty *hash_table)
{
	/* assert */

	/* traverse table until encountered an occupied dlist */
	
	/* return an iterator to this elemnet*/
}


/*******************************************************************************
***************************** HashTableEnd ************************************/
ht_itr_ty HashTableEnd(ht_ty *hash_table)
{
	/* assert */

	/* return an iterator to the out of table's range */
}



/*******************************************************************************
***************************** HashTableNext ***********************************/
ht_itr_ty HashTableNext(ht_itr_ty itr)
{
	/* assert */

	/* In case, next element in dlist is the last one */
		/* go to the next element in table, incrementing table index by one */
	
	/* otherwise, go to the next dlist element */

	/* return iterator to element */
}


/*******************************************************************************
***************************** HashTablePrev ***********************************/
ht_itr_ty HashTablePrev(ht_itr_ty itr)
{
	/* assert */

	/* In case, prev element in dlist is the first one */
		/* go to the prev element in table, decrementing table index by one */
	
	/* otherwise, go to the previous dlist element */

	/* return iterator to element */
}


/*******************************************************************************
*************************** HashTableGetData **********************************/
void *HashTableGetData(ht_itr_ty itr)
{
	ASSERT_NOT_NULL(itr.hash_table, 			\
	"HashTableGetData: HashTable is not allocated");
	
	return DListGetData(itr.element_itr);
}


/*******************************************************************************
************************* HashTableIsSameIter *********************************/
int HashTableIsSameIter(ht_itr_ty itr1, ht_itr_ty itr2)
{
	return DListIsSameIter(itr1.element_itr, itr2.element_itr);
}

/*******************************************************************************
************************* HashTableIsBadIter **********************************/
int HashTableIsBadIter(ht_itr_ty itr)
{
	ht_itr_ty end = HashTableEnd(itr.hash_table);
	
	return HashTableIsSameIter(itr, end);
}



/*******************************************************************************
****************************** Side-Funcs *************************************/
static ht_itr_ty WrapToHashIMP(ht_ty *th_, size_t index_, dlist_itr_ty dlist_itr_)
{
	ht_itr_ty ret = {NULL};
	
	/* Init hash_table_itr fields */
	ret.element_itr = dlist_itr_;
	ret.hash_table = th_;
	ret.table_index = index_;
	
	return ret;
}

static size_t HashToIndexIMP(ht_ty *th_, const void *to_add_)
{
	/* Call hash_func to get the hash value */
	size_t index = th_->hash_func(to_add_, th_->m_param);
	
	/* Convert the hash value into the array index */
	index %= th_->m_htsize;
	
	return index;
}


static ht_itr_ty BadIterIMP(size_t table_size_)
{
	ht_itr_ty ret = {NULL};
	
	ret.element_itr.to_node = INVALID_PTR;
	ret.hash_table = INVALID_PTR;
	ret.table_index = table_size_;
	
	return ret;
}





