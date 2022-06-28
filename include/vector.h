/*******************************************************************************
******************************* - VECTOR - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION 	vector API
*	NAME 			Liad Raz
*
*******************************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct vector vector_ty;


/*******************************************************************************
* DESCRIPTION	Creates a vector data struct for void* pointers.
* RETURN	 	returns NULL in case of memory failure 
* IMPORTANT	 	- User needs to free the allocated vector struct
				- size of array cannot be bigger than the
				allocated memory (undefine behavior).
				- In case max_allocated_memory is zero, value of size
				will assigned to it.  

* Time Complexity 	O(1)
*******************************************************************************/
vector_ty *VectorCreate(size_t max_allocated_memory, size_t size_of_array);


/*******************************************************************************
* DESCRIPTION	Changes an element value at a given index.
* IMPORTANT	 	index value MUST be in range of the array size

* Time Complexity 	O(1)
*******************************************************************************/
void VectorSet(vector_ty *vector, void *value, size_t index);

/*******************************************************************************
* DESCRIPTION	Returns the value of element in a given index.
* IMPORTANT		Undefined behavior when trying to get 
			an element that is out of vector's range.

* Time Complexity: O(1)
*******************************************************************************/
void *VectorGet(const vector_ty *vector, size_t index);


/*******************************************************************************
* Description	Checks the amount of allocated memory in vector struct

* Time Complexity: O(1)
*******************************************************************************/
size_t VectorCapacity(const vector_ty *vector);


/*******************************************************************************
* Description	Checks how many elements are stored in vector struct.

* Time Complexity: O(1)
*******************************************************************************/
size_t VectorSize(const vector_ty *vector);


/*******************************************************************************
* Description	return the allocated vector's address in memory

* Time Complexity: O(1)
*******************************************************************************/
void **VectorGetArray(const vector_ty *vector);


/*******************************************************************************
* Description	Change the size of the underlying array
* IMPORTANT	 	- In case new size is bigger than capacity
				new memory will be allocated.
				- In case new_size is bigger than size and smaller than capacity
				vector[size] to vector[new_size-1] - will be set to NULL

* Time Complexity:
	new_size < size O(1)
	new_size > capacity O(new_size)
	new_size < capacity && new_size > size  O(new_size - size)
*******************************************************************************/
int VectorResize(vector_ty *vector, size_t new_size_of_array);


/*******************************************************************************
* Description	Reallocate memory to change the capacity size
* IMPORTANT	 	- returns NULL in case of memory failure
				- New Capacity value must be bigger than size
				- new_capacity < capacity will cause an undefined behaviour

* Time Complexity: O(new_capacity)
*******************************************************************************/
int VectorReserve(vector_ty *vector, size_t new_max_allocated_memory);


/*******************************************************************************
* DESCRIPTION	Shrinks the capacity of vector struct to its size value
* IMPORTANT	 	returns NULL in case of memory failure

* Time Complexity 	O(size)
*******************************************************************************/
int VectorShrink(vector_ty *vector);


/******************************************************************************
* DESCRIPTION	Adds an element to the last address in vector struct.
* RETURN        status => 0 SUCCESS; non-zero value FAILURE.
* IMPORTANT 	pushing an element to last capacity place 
				will call a memory allocation.

* Time Complexity: 
	size < capacity O(1)
	average O(n)
*******************************************************************************/
int VectorPushBack(vector_ty *vector, void *element);


/******************************************************************************
* DESCRIPTION	Removes an element from the last address in vector struct.
* IMPORTANT		Undefined behavior when trying to pop the first element.

* Time Complexity O(1)
*******************************************************************************/
void *VectorPopBack(vector_ty *vector);


/*******************************************************************************
* DESCRIPTION	Frees vector struct from memory

* Time Complexity 	O(1)
*******************************************************************************/
void VectorDestroy(vector_ty *vector);


#endif /* __VECTOR_H__ */

