/*******************************************************************************
******************************* - STUCK - **************************************
***************************** Data Structures **********************************
*
*	DESCRIPTION 	Test Stack functions
*	NAME 			Liad Raz
*	COMPILE			gc src/stack.c test/stack_test.c -I ./include/
*
*******************************************************************************/

#include <stdio.h> /* printf, puts */

#include "stack.h"

void TestStack(void);

int main(void)
{
	puts("\n\t~~~~~~~~ Data Structures - Stack ~~~~~~~~\n");
	TestStack();
	
	return 0;
}

void TestStack(void)
{
	size_t capacity = 100;
	stack_ty *stack = NULL;
	
	int element1 = 12;
	float element2 = 45.8;
	
	/* Create Stack */
	stack = StackCreate(capacity);
	
	puts("\t -- Push 6 elements to stack --");
	/* Push Element */
	StackPush(stack, (void *)&element1);
	StackPush(stack, (void *)&element2);
	StackPush(stack, (void *)&element2);
	StackPush(stack, (void *)&element2);
	StackPush(stack, (void *)&element1);
	StackPush(stack, (void *)&element1);

	/* Size Element */
	printf("Amount of elements in Stack => \t%lu\n", StackSize(stack));

	/* Peek Element */
	printf("Current Top Element => \t\t%d\n", *((int *)StackPeek(stack)));
	
	puts("\n\t -- Pop 2 elements from stack --");
	/* Pop Element */
	StackPop(stack);
	StackPop(stack);
	
	/* Size Element */
	printf("Amount of elements in Stack => \t%lu\n", StackSize(stack));

	/* Peek Element */
	printf("Current Top Element => \t\t%f\n", *((float *)StackPeek(stack)));
	
	puts("\n\t -- Overall stack information --");
	/* Is Stack Empty */
	printf("Is Stack is empty => \t%s\n", (StackIsEmpty(stack)) ? "YES" : "NO");

	/* Capacity of Stack */
	printf("Capacity of Stack => \t%lu\n", StackCapacity(stack));

	/* Free Stack memory */
	StackDestory(stack);
}

