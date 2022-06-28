/*******************************************************************************
******************************** - HEAP - **************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Test Heap
*	AUTHOR 			Liad Raz
*
*******************************************************************************/


#include <stdio.h>		/* printf, puts, putchar, size_t */

#include "utilities.h"
#include "heap.h"


int nums[] = {16, 14, 10, 8, 7, 9, 3, 2, 4, 1};
size_t nums_size = sizeof(nums) / sizeof(nums[0]);


void TestHeapCreate(void);
void TestHeapPush(void);
void TestHeapPop(void);
void TestHeapPeek(void);
void TestHeapSize(void);
void TestHeapIsEmpty(void);
void TestHeapRemove(void);


static int CmpNumbersIMP(const void *num1_, const void *num2_, const void *param_);
static int ShouldRemove(const void *data, const void *param);
static void PrintTestStatusIMP(size_t tcounter_, size_t num_of_tests_, char *test_name_);


int main(void)
{
	PRINT_MSG(\n\t--- Tests heap ---\n);
	
	TestHeapCreate();
	TestHeapPush();
	TestHeapPop();
	TestHeapPeek();

	return 0;
}

void TestHeapCreate(void)
{
	heap_ty *heap = NULL;
	void *param = NULL;
	size_t tcounter = 0;

	heap = HeapCreate(CmpNumbersIMP, param);

	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	if (heap != NULL)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 1, "Create");

	HeapDestroy(heap);
}


void TestHeapPush(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;

	int ret = 0;

	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	if (ret == 0)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 1, "Push");

	HeapDestroy(heap);
}


void TestHeapPop(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;

	int ret = 0;

	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	HeapPop(heap);
	if (*(int *)(HeapPeek(heap)) == 14)
	{ ++tcounter; }

	HeapPop(heap);
	if (*(int *)(HeapPeek(heap)) == 10)
	{ ++tcounter; }

	HeapPop(heap);
	if (*(int *)(HeapPeek(heap)) == 9)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 3, "Pop");

	HeapDestroy(heap);
}

void TestHeapPeek(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;

	int ret = 0;

	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	if (*(int *)(HeapPeek(heap)) == 16)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 1, "Peek");

	HeapDestroy(heap);
}


void TestHeapSize(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;

	int ret = 0;

	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	if (HeapSize(heap) == nums_size)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 1, "Size");

	HeapDestroy(heap);
}


void TestHeapIsEmpty(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;

	int ret = 0;

	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	if (HeapIsEmpty(heap) == 1)
	{ ++tcounter; }


	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	if (HeapIsEmpty(heap) == 0)
	{ ++tcounter; }


	PrintTestStatusIMP(tcounter, 2, "IsEmpty");

	HeapDestroy(heap);
}


void TestHeapRemove(void)
{
	/* Create heap */
	heap_ty *heap = NULL;
	void *param = NULL;
	void *removed = NULL;
	int *runner = nums;
	size_t i = 0;
	size_t tcounter = 0;
	int ret = 0;


	heap = HeapCreate(CmpNumbersIMP, param);
	if (NULL == heap)
	{
		puts("Memory Allocation Failed");
		return;
	}

	for (i = 0; i < nums_size; ++i)
	{
		ret = HeapPush(heap, (void *)runner);
		++runner;

		if (ret) { break; }
	}

	removed = HeapRemove(heap, ShouldRemove, param);

	if (*(int *)removed == 3)
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 1, "Remove");

	HeapDestroy(heap);
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*-------------------------------CallBack Funcs ------------------------------*/
static int CmpNumbersIMP(const void *num1_, const void *num2_, const void *param_)
{
	UNUSED(param_);
	
	return (*(int *)num1_ - *(int *)num2_);
}


static int ShouldRemove(const void *data_, const void *param_)
{
	UNUSED(param_);

	return (*(int *)data_ < 7);	
}

/*-------------------------------Side Functions ------------------------------*/
static void PrintTestStatusIMP(size_t tcounter_, size_t num_of_tests_, char *test_name_)
{
	if (tcounter_ == num_of_tests_)
	{
		GREEN;
		printf("\tTest %s: SUCCESS", test_name_);
		DEFAULT;
	}
	else
	{
		RED;
		printf("\tTest %s: FAILED", test_name_);
		DEFAULT;
	}

	NEW_LINE;
}