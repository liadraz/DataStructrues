/*******************************************************************************
******************************* - STACK - **************************************
***************************** Data Structures **********************************
*
*	DESCRIPTION 	Stack functions implementations
*	NAME 			Liad Raz
*	COMPILE			gc src/stack.c test/stack_test.c -I ./include/
*
*******************************************************************************/

#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */

#include "stack.h"

#define SIZE_PTR 		sizeof(void *)
#define STRUCT_MEMBERS 	sizeof(stack_ty)

#define DEAD_MEM(POINTER) (POINTER)0xDEADBEEF

#ifdef NDEBUG
#define DEBUG_ONLY(x)
#else
#define DEBUG_ONLY(x) x
#endif


struct stack 
{
	void **top;
	void **start;
	void **end;
};


stack_ty *StackCreate(const size_t capacity)
{
	/* Create a stack struct the size of 3 pointer */
	stack_ty *stack = (stack_ty *)malloc(STRUCT_MEMBERS);

	if (NULL == stack)
	{
		return NULL;
	}
	
	/* Create a stack array in data buff member the size of capacity */
	stack->start = (void **)malloc(capacity * SIZE_PTR);

	if (NULL == stack->start)
	{
		free(stack);
		return NULL;
	}

	/* Check that capacity is above zero */
	assert (capacity != 0 && "stack size must be at least one");
	
	/* top pointer will point the start of the struct */
	stack->top = stack->start;
	
	/* end pointer will point the end */
	stack->end = stack->start + capacity;
	
	return stack;
}

void StackDestory(stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");

/*	DEAD_MEM(stack->top);*/
/*	DEAD_MEM(stack->start);*/
/*	DEAD_MEM(stack->end);*/
	
	free(stack->start);
	free(stack);
}

void StackPop(stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");
	assert (stack->top >= stack->start && "Pop cannot be executed while stack is empty");
	
/*	DEBUG_ONLY(*DEAD_MEM(stack->top));*/
	--stack->top;
}

void StackPush(stack_ty *stack, void *element)
{
	assert (stack != NULL && "stack is not allocated");
	assert (stack->top <= stack->end && "Push element will cause an overflow");
	
	*(stack->top) = element;
	++stack->top;
}

void *StackPeek(const stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");
	assert (stack->top >= stack->start && "Peek cannot be executed while stack is empty");
	
	return *(stack->top - 1);
}

size_t StackSize(const stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");
	
	return (stack->top - stack->start);
}

int StackIsEmpty(const stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");
	
	return !!(stack->start == stack->top);
}

size_t StackCapacity(const stack_ty *stack)
{
	assert (stack != NULL && "stack is not allocated");
	
	return (stack->end - stack->start);
}


