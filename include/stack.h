/*******************************************************************************
******************************* - STACK - **************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION 	Build a fixed size stack data structure
*	NAME 			Liad Raz
*
*******************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

typedef struct stack stack_ty;

/*******************************************************************************
* DESCRIPTION	Create a fixed size stack for void* pointers.
* RETURN	 	returns NULL in case of memory failure 
* IMPORTANT	 	User needs to free the allocated stack

* Time Complexity 	O(1)
*******************************************************************************/
stack_ty *StackCreate(size_t max_size_of_stack_array);


/******************************************************************************
* DESCRIPTION	Frees stack array from memory

* Time Complexity O(1)
*******************************************************************************/
void StackDestory(stack_ty *stack);


/******************************************************************************
* DESCRIPTION	Removes the top most element of the stack.
* IMPORTANT		popping an empty stack will cause an undefine behavior.

* Time Complexity O(1)
*******************************************************************************/
void StackPop(stack_ty *stack);


/******************************************************************************
* DESCRIPTION	Adds an element to the top of the stack.
* IMPORTANT 	pushing an element to a full stack will cause an undefine behavior

* Time Complexity: O(1)
*******************************************************************************/
void StackPush(stack_ty *stack, void *element_value);


/******************************************************************************
* DESCRIPTION	returns the value of top most element without removig it
* IMPORTANT 	peeking from an empty stack will cause an undefined behavior

* Time Complexity: O(1)
*******************************************************************************/
void *StackPeek(const stack_ty *stack);


/******************************************************************************
* DESCRIPTION	Calculates the amount of existing elements stored in the stack

* Time Complexity: O(1)
*******************************************************************************/
size_t StackSize(const stack_ty *stack);


/******************************************************************************
* DESCRIPTION	Checks the stack status.

* Time Complexity: O(1)
*******************************************************************************/
int StackIsEmpty(const stack_ty *stack);


/******************************************************************************
* Description	Checks how many elements the stack can hold

* Time Complexity: O(1)
*******************************************************************************/
size_t StackCapacity(const stack_ty *stack);


#endif /* __STACK_H__ */
