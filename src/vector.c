/*******************************************************************************
******************************* - VECTOR - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION 	Build vector data structure
*	NAME 			Liad Raz
*
*	gc src/vector.c test/vector_test.c -I ./include/ -I ../
*******************************************************************************/

#include <stddef.h>  		/* size_t */
#include <stdlib.h>		/* malloc, realloc */
#include <string.h>		/* memset */
#include <assert.h>		/* assert */

#include <stdio.h>		/* printf */

#include "vector.h"
#include "utilities.h" 	/* SIZE_PTR, SIZEOF_TYPE(type), SUCCESS, ALLOC_ERR */

#define SIZEOF_STRUCT	SIZEOF_TYPE(vector_ty)
#define GROW_FACTOR		2
#define MIN_CAPACITY	1LU

#define SUCCESS			0
#define ALLOC_ERR 		1
#define FUNC_FAILED 		2

#define ASSERT_VECTOR_ALLOC	assert (0 != vector && "VECTOR is not allocated");

struct vector
{
	void **curr_element;
	void **first;
	size_t capacity;
};

static int GrowVectorCapacity(vector_ty *vector, size_t new_capacity);
static int ShrinkVectorCapacity(vector_ty *vector, size_t new_capacity);


/*******************************************************************************
****************************** VectorCreate ***********************************/
vector_ty *VectorCreate(size_t capacity, const size_t size)
{ 
	vector_ty *vector = (vector_ty *)malloc(SIZEOF_STRUCT);
	
	if (NULL == vector)
	{
		return NULL;
	}
	
	/* Ensures create capacity does not equal to zero */
	if (0 == capacity)
	{
		capacity = (0 != size) ? size : MIN_CAPACITY; 
	}
	
	vector->first = (void **)malloc(capacity * SIZE_PTR);
	
	if (NULL == vector->first)
	{
		free(vector);
		return NULL;
	}
	
	/* Initi elements in size_array to zero value */
	memset(vector->first, 0, (size * SIZE_PTR));

	vector->curr_element = vector->first + size;
	vector->capacity = capacity;
	
	return vector;
}

/*******************************************************************************
******************************* VectorSet *************************************/
/* Changes an element value at a given index */
void VectorSet(vector_ty *vector, void *value, size_t index)
{
	ASSERT_VECTOR_ALLOC;
	assert (index < vector->capacity && "Index is out of vector capacity range");
	
	*(vector->first + index) = value;
}


/*******************************************************************************
***************************** VectorGet ***************************************/
void *VectorGet(const vector_ty *vector, size_t index)
{
	ASSERT_VECTOR_ALLOC;
	assert (index < VectorSize(vector) && "Index must be in size value range");
	
	return vector->first[index];
}


/*******************************************************************************
**************************** VectorCapacity ***********************************/
size_t VectorCapacity(const vector_ty *vector)
{
	ASSERT_VECTOR_ALLOC;
	
	return vector->capacity;
}


/*******************************************************************************
***************************** VectorSize **************************************/
size_t VectorSize(const vector_ty *vector)
{
	ASSERT_VECTOR_ALLOC;
	
	return vector->curr_element - vector->first;
}


/*******************************************************************************
***************************** VectorGetArray **********************************/
void **VectorGetArray(const vector_ty *vector)
{
	ASSERT_VECTOR_ALLOC;
	
	return vector->first;
}


/*******************************************************************************
****************************** VectorShrink ***********************************/
int VectorShrink(vector_ty *vector)
{
	size_t new_size = vector->curr_element - vector->first;
	int err = SUCCESS;
	
	ASSERT_VECTOR_ALLOC;
	assert (vector->curr_element != vector->first &&
	"Vector cannot be shrinked when size equals zero");
	
	err = ShrinkVectorCapacity(vector, new_size);

	return err;
}


/*******************************************************************************
**************************** VectorResize *************************************/
int VectorResize(vector_ty *vector, size_t new_size)
{
	void **new_size_ptr = vector->first + new_size;	
	void **org_capacity_ptr = vector->first + vector->capacity;	
	int err = SUCCESS;
	
	ASSERT_VECTOR_ALLOC;
	
	/*	new_size < size */
	if (new_size_ptr < vector->curr_element)
	{
		vector->curr_element = new_size_ptr;
	}
	/*	(new_size > size) && (size < capacity) */
	else if (new_size_ptr < org_capacity_ptr)
	{
		/*	null add pointer*/
		memset(vector->curr_element, 0, (new_size_ptr - vector->curr_element) * SIZE_PTR - 1); 
		
		vector->curr_element = new_size_ptr;
	}
	
	/* new_size > capacity */
	if (new_size_ptr > org_capacity_ptr)
	{
		err = VectorReserve(vector, new_size);
	}
	
	return err;
}

/*******************************************************************************
**************************** VectorReserve ************************************/
int VectorReserve(vector_ty *vector, size_t new_capacity)
{
	int err = SUCCESS;
	
	ASSERT_VECTOR_ALLOC;
	assert (new_capacity > vector->capacity
			&& "New Capacity value must be bigger than size");
	
	err = GrowVectorCapacity(vector, new_capacity);
	
	return err;
}


/*******************************************************************************
***************************** VectorPushBack **********************************/
int VectorPushBack(vector_ty *vector, void *element)
{
	int err = SUCCESS;
	
	ASSERT_VECTOR_ALLOC;
	
	/* Enlarge the capacity of vector when vector is full */
	if (vector->curr_element == (vector->first + vector->capacity))
	{
		err = GrowVectorCapacity(vector, GROW_FACTOR * vector->capacity);

		if (0 != err)
		{
			return err;
		}
	}
	
	*(vector->curr_element) = element;
	++vector->curr_element;
	
	return err;
}

/*******************************************************************************
***************************** VectorPopBack ***********************************/
void *VectorPopBack(vector_ty *vector)
{
	ASSERT_VECTOR_ALLOC;
	assert (vector->curr_element != vector->first && "First element cannot be popped");
	
	--vector->curr_element;

	return (vector->curr_element);
}

/*******************************************************************************
****************************** VectorDestroy **********************************/
void VectorDestroy(vector_ty *vector)
{
	ASSERT_VECTOR_ALLOC;

	free(vector->first);
	DEBUG_MODE(
	vector->first = DEAD_MEM(void *);
	vector->curr_element = DEAD_MEM(void *);
	);

	free(vector);
	DEBUG_MODE(
	vector = DEAD_MEM(vector_ty *);
	); 
}


/*******************************************************************************
*************************** Side Functions ************************************/
static int GrowVectorCapacity(vector_ty *vector, size_t new_capacity)
{
	size_t curr_vector_size = vector->curr_element - vector->first;
	size_t new_capacity_bytes = new_capacity * SIZE_PTR;
	void **tmp_buffer = (void **)realloc(vector->first , new_capacity_bytes);
	
	if (NULL == tmp_buffer)
	{
		return ALLOC_ERR;
	}
	
	vector->first = tmp_buffer;
	vector->curr_element = vector->first + curr_vector_size;
	vector->capacity = new_capacity;
	
	return SUCCESS;
}


static int ShrinkVectorCapacity(vector_ty *vector, size_t new_capacity)
{
	size_t new_capacity_bytes = new_capacity * SIZE_PTR;
	void **tmp_buffer = (void **)malloc(new_capacity_bytes);
	
	if (NULL == tmp_buffer)
	{
		return ALLOC_ERR;
	}
	
	vector->first = tmp_buffer;
	vector->curr_element = vector->first + new_capacity;
	vector->capacity = new_capacity;
	
	free(tmp_buffer);
	DEBUG_MODE(
	tmp_buffer = DEAD_MEM(void **);
	); /* DEBUG_ONLY */ 

	return SUCCESS;
}




/*******************************************************************************
							Vector Create

vector_ty *VectorCreate(size_t size, size_t capacity);

	define vector stack pointer and allocate memory of its size
	
	allocate memory the size of capacity
		
	assign curr_element pointer to the first element address
	assign start pointer to point the address of
	assign capacity member with the capacity value

--------------------------------------------------------------------------------

							Vector Set

	assign value to the element in index in the vector array.

--------------------------------------------------------------------------------
	
							Vector Shrink
	
	realloc the capacity ?
		
--------------------------------------------------------------------------------

							Vector Capacity
	
	return the vector capacity value
--------------------------------------------------------------------------------

							Vector Size
	
	subtract the current element address with start pointer

*******************************************************************************/


