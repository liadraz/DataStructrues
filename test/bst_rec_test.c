/*******************************************************************************
****************************** - BST_REC - ********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Tests Binary Sort Tree (recursive ) - AVL Tree.
*	AUTHOR 			Liad Raz
*
*******************************************************************************/


#include <stdio.h>		/* printf, puts, putchar, size_t */
#include <stdlib.h>		/* rand, srand */
#include <time.h>			/* time_t, time, clock_t, clock, CLOCKS_PER_SEC */

#include "utilities.h"
#include "bst_rec.h"

/* Definitions fot Sorting Test */
#define FIVE_KB			(10)
#define SIZE_ARR(LIST)		(sizeof(LIST) / sizeof(int))

/* Global Variables to Create tree */
int nums[] = {42, 86, 16, 8, 77, 92, 25};
size_t nums_size = sizeof(nums) / sizeof(nums[0]);

typedef struct sorting
{
	int *m_sorted;
	size_t m_index;
} sorting_ty;

void TestAVLCreate(void);
void TestAVLInsert(void);
void TestAVLRemove(void);
void TestAVLSize(void);
void TestAVLIsEmpty(void);
void TestAVLFind(void);
void TestAVLForEach(void);
void TestSortNumbers(void);


/* Comparison Callback function of the tree */
static int CmpNumbersIMP(const void *num1, const void *num2, const void *param);
static int PrintForEeach(void *data, const void *NullParam);
static int CopySortedToArray(void *data, const void *to_sort);

/* Cheaks status of specific Test */
static void PrintTestStatusIMP(size_t tcounter_, size_t num_of_tests_, char *test_name_);

static size_t BuildTree(avl_ty *th_);
static void GenerateRandomNumsIMP(int arr_[], size_t size);


int main(void)
{
	PRINT_MSG(\n\t--- Tests bst_rec ---\n);
	
	TestAVLCreate();
	TestAVLInsert();
	TestAVLRemove();
	TestAVLSize();
	TestAVLIsEmpty();
	TestAVLFind();
	TestAVLForEach();
	TestSortNumbers();

	NEW_LINE;
	NEW_LINE;
	
	return 0;
}



void TestAVLCreate(void)
{
	avl_ty *bst = NULL;
	void *param = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, param);
	
	if (NULL == bst)
	{
		puts("Memory Allocation Failed");
		return;
	}
		
	if (bst != NULL)
	{ ++tcounter; }
	
	PrintTestStatusIMP(tcounter, 1, "Create");

	AVLDestory(bst);
}

void TestAVLInsert(void)
{
	avl_ty *bst = NULL;
	void *param = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, param);
	if (NULL == bst)
	{
		puts("TestAVLInsert: Memory Allocation Failed");
		return;
	}

	tcounter = BuildTree(bst);
	PrintTree(bst);
	PrintTestStatusIMP(tcounter, nums_size, "Insert");

	AVLDestory(bst);
}

void TestAVLRemove(void)
{
	avl_ty *bst = NULL;
	void *param = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, param);
	if (NULL == bst)
	{
		puts("TestAVLInsert: Memory Allocation Failed");
		return;
	}

	BuildTree(bst);
	
	/* Remove leaf node */
	AVLRemove(bst, (nums + 4)); 		/* {42, 86, 16, 8, 77, 92, 25}; */
	if (6 == AVLSize(bst))
	{ ++tcounter; }
	
	/* Remove node with Child Right */
	AVLRemove(bst, (nums + 1));
	if (5 == AVLSize(bst))
	{ ++tcounter; }

	/* Remove node with Child Left */
	AVLRemove(bst, (nums + 6));
	AVLRemove(bst, (nums + 2));
	if (3 == AVLSize(bst))
	{ ++tcounter; }

	AVLDestory(bst);

	bst = AVLCreate(CmpNumbersIMP, param);
	PrintTree(bst);

	/* Remove node with Two Children */
	/*AVLRemove(bst, nums);
	if (2 == AVLSize(bst))
	{ ++tcounter; }
	*/

	PrintTestStatusIMP(tcounter, 4, "Remove");

	AVLDestory(bst);
}

void TestAVLSize(void)
{
	avl_ty *bst = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, NULL);
	
	if (NULL == bst)
	{
		puts("Memory Allocation Failed");
		return;
	}
		
	if (0 == AVLSize(bst))
	{ ++tcounter; }

	BuildTree(bst);
		
	if (7 == AVLSize(bst))
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 2, "Size");

	AVLDestory(bst);

}

void TestAVLIsEmpty(void)
{
	avl_ty *bst = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, NULL);
	
	if (NULL == bst)
	{
		puts("Memory Allocation Failed");
		return;
	}
		
	if (1 == AVLIsEmpty(bst))
	{ ++tcounter; }

	BuildTree(bst);

	if (0 == AVLIsEmpty(bst))
	{ ++tcounter; }
	
	PrintTestStatusIMP(tcounter, 2, "IsEmpty");

	AVLDestory(bst);
}

void TestAVLFind(void)
{
	avl_ty *bst = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, NULL);
	
	if (NULL == bst)
	{
		puts("Memory Allocation Failed");
		return;
	}

	if (NULL == AVLFind(bst, nums))
	{ ++tcounter; }
	
	BuildTree(bst);

	if (nums[3] == *(int *)AVLFind(bst, (nums + 3)))
	{ ++tcounter; }

	if (77 == *(int *)AVLFind(bst, (nums + 4)))
	{ ++tcounter; }

	PrintTestStatusIMP(tcounter, 3, "Find");

	AVLDestory(bst);
}

void TestAVLForEach(void)
{
	avl_ty *bst = NULL;
	size_t tcounter = 0;

	bst = AVLCreate(CmpNumbersIMP, NULL);
	if (NULL == bst)
	{
		puts("Memory Allocation Failed");
		return;
	}

	BuildTree(bst);

	puts("PRE:");
	AVLForEach(bst, PRE, PrintForEeach, NULL);
	puts("\nIN:");
	AVLForEach(bst, IN, PrintForEeach, NULL);
	puts("\nPOST:");
	AVLForEach(bst, POST, PrintForEeach, NULL);
	puts("");

	PrintTestStatusIMP(tcounter, 0, "ForEach");

	AVLDestory(bst);
}

void TestSortNumbers(void)
{
	avl_ty *tree = NULL;
	sorting_ty nums = {NULL};
	int *random = NULL; 

	int *runner = NULL;
	size_t i = 0;

	random = (int *)malloc(FIVE_KB * sizeof(int));
	nums.m_sorted = (int *)malloc(FIVE_KB * sizeof(int));

	tree = AVLCreate(CmpNumbersIMP, NULL);
	if (NULL == tree)
	{
		puts("Memory Allocation Failed");
		return;
	}

	/* Insert random numbers to tree */
	GenerateRandomNumsIMP(random, FIVE_KB);
	runner = random;
	for (i = 0; i < FIVE_KB; ++i)
	{
		AVLInsert(tree, runner);
		++runner;
	}

	AVLForEach(tree, IN, CopySortedToArray, (void *)&nums);

	runner = nums.m_sorted;
	for (i = 0; i < FIVE_KB; ++i)
	{
		printf("%d  ", *(runner + i));
	}

	free(random);
	free(nums.m_sorted);
	AVLDestory(tree);
}	


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*-------------------------------CallBack Funcs ------------------------------*/
static int CmpNumbersIMP(const void *num1_, const void *num2_, const void *param_)
{
	UNUSED(param_);
	
	return (*(int *)num1_ - *(int *)num2_);
}

static int PrintForEeach(void *data, const void *NullParam)
{
	UNUSED(NullParam);

	return (0 > printf("->%d\t", *(int *)data));
}

static int CopySortedToArray(void *data, const void *arr)
{
	int index = ((sorting_ty *)arr)->m_index;
	((sorting_ty *)arr)->m_sorted[index] = *(int *)data;

	++((sorting_ty *)arr)->m_index;

	return 0;
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

static size_t BuildTree(avl_ty *th_)
{
	int *runner = nums;
	size_t tcounter = 0;
	size_t i = 0;
	int ret = 0;

	for (i = 0; i < nums_size; ++i)
	{
		ret = AVLInsert(th_, runner);
		++runner;
		
		if (ret != 1) 
		{ ++tcounter; }
	}

	return tcounter;
}

static void GenerateRandomNumsIMP(int arr_[], size_t size)
{
	size_t num = 0;
	
	srand((unsigned)time(NULL));
	
	for (num = 0; num < size; ++num)
	{
		arr_[num] = rand() % 5000;
	}
}