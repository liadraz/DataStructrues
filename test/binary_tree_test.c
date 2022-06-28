/*******************************************************************************
****************************** - BINARY_TREE - ********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Tests Binary Sorted Tree
*	AUTHOR 			Liad Raz
*
*******************************************************************************/


#include <stdio.h>		/* printf, puts, size_t */
#include <stdlib.h>		/* rand, srand  */
#include <string.h>		/* strlen, strncmp */
#include <time.h>			/* time_t, time */

#include "utilities.h"
#include "binary_tree.h"

#define TREE_SIZE			15
#define ARR_SIZE			8

int nums[TREE_SIZE] = {0};
int nums_hard_coded[] = {286, 220, 300, 332, 270, 254, 137, 100};

void TestBSTCreate(void);
void TestBSTInsert(void);
void TestBSTRemove(void);
void TestBSTSize(void);
void TestBSTIsEmpty(void);
void TestBSTBegin(void);
void TestBSTEnd(void);
void TestBSTPrev(void);
void TestBSTNext(void);
void TestBSTFind(void);
void TestBSTForEach(void);


static int CmpNumbersIMP(const void *num1_, const void *num2_, const void *);
static void GenerateRandomNumsIMP(int arr_[], size_t size);
static bst_ty *CreateTreeIMP(void);

int main(void)
{
	PRINT_MSG(\n\t--- Tests Binary Sorted Tree ---\n);
	
	GenerateRandomNumsIMP(nums, TREE_SIZE);

	TestBSTCreate();
	TestBSTInsert();
	TestBSTRemove();
	TestBSTSize();
	TestBSTIsEmpty();
	TestBSTBegin();
	TestBSTEnd();
	TestBSTPrev();
	TestBSTNext();
	TestBSTFind();
	TestBSTForEach();

	NEW_LINE;
	NEW_LINE;
	
	return 0;
}


/*******************************************************************************
********************************** Tests **************************************/
void TestBSTCreate(void)
{
	bst_ty *tree = NULL;
	void *param = NULL;
	size_t test_counter = 0;

	tree = BSTCreate(CmpNumbersIMP, param);
	
	if (NULL == tree)
	{
		puts("Memory Allocation Failed");
		return;
	}
		
	if (tree != NULL)
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Create: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Create: FAILED);
		DEFAULT;
	}
	
	BTSDestroy(tree);
}

void TestBSTInsert(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty ret = {NULL};
	size_t test_counter = 0;
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		ret = BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
	
	if (*(int *)(BSTGetData(ret)) == nums_hard_coded[ARR_SIZE - 1])
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Insert: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Insert: FAILED);
		DEFAULT;
	}
	
	BTSDestroy(tree);
}

void TestBSTRemove(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty remove_leaf = {NULL};
	bst_itr_ty remove_1_child = {NULL};
	bst_itr_ty remove_2_children_bigger = {NULL};
	bst_itr_ty remove_2_children_next = {NULL};
	bst_itr_ty next = {NULL};
	size_t test_counter = 0;
	int num1 = 280;
	int num2 = 260;
	int num3 = 270;
	
/*	{286, 220, 300, 332, 270, 254, 137, 100};*/
	
	BSTInsert(tree, (void *)&nums_hard_coded[0]);
	remove_2_children_bigger = BSTInsert(tree, (void *)&nums_hard_coded[1]);
	BSTInsert(tree, (void *)&nums_hard_coded[2]);
	remove_leaf = BSTInsert(tree, (void *)&nums_hard_coded[3]);
	remove_1_child = BSTInsert(tree, (void *)&nums_hard_coded[4]);
	remove_2_children_next = BSTInsert(tree, (void *)&nums_hard_coded[5]);
	BSTInsert(tree, (void *)&nums_hard_coded[6]);
	BSTInsert(tree, (void *)&nums_hard_coded[7]);
	
	/* One Leaf */
	next = BSTRemove(remove_leaf);
	if ((void *)BSTGetData(next) == INVALID_PTR)
	{ ++test_counter; }
	
	/* Node with One Child */
	next = BSTRemove(remove_1_child);
	if (*(int *)BSTGetData(next) == 286)
	{ ++test_counter; }
	
	/* Node with Two Children */
	next = BSTRemove(remove_2_children_bigger);
	if (*(int *)BSTGetData(next) == 254)
	{ ++test_counter; }
	
	/* Node with Two Children and different next */
	BSTInsert(tree, (void *)&num1);
	BSTInsert(tree, (void *)&num2);
	BSTInsert(tree, (void *)&num3);
	next = BSTRemove(remove_2_children_next);
	if (*(int *)BSTGetData(next) == 260)
	{ ++test_counter; }
	
	if (test_counter == 4)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Remove: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Remove: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}

void TestBSTSize(void)
{
	bst_ty *tree = CreateTreeIMP();
	size_t test_counter = 0;
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
		
	if (BSTSize(tree) == ARR_SIZE)
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Size: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Size: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}

void TestBSTIsEmpty(void)
{
	void *param = NULL;
	bst_ty *tree_full = CreateTreeIMP();
	bst_ty *tree_empty = BSTCreate(CmpNumbersIMP, param);
	size_t test_counter = 0;
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		BSTInsert(tree_full, (void *)&nums_hard_coded[i]);
	}
	
	if (BSTIsEmpty(tree_empty))
	{ ++test_counter; }
	
	if (!BSTIsEmpty(tree_full))
	{ ++test_counter; }
	
	if (test_counter == 2)
	{
		GREEN;
		PRINT_STATUS_MSG(Test IsEmpty: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test IsEmpty: FAILED);
		DEFAULT;
	}	

	BTSDestroy(tree_full);
	BTSDestroy(tree_empty);
}


void TestBSTBegin(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty ret = {NULL};
	size_t test_counter = 0;
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		ret = BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
	
	if (BSTIterIsEqual(BSTBegin(tree), ret))
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Begin: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Begin: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}


void TestBSTEnd(void)
{
	bst_ty *tree = CreateTreeIMP();
	size_t test_counter = 0;
	
	if (BSTIterIsEqual(BSTBegin(tree), BSTEnd(tree)))
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test End: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test End: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}


void TestBSTPrev(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty ret = {NULL};
	size_t test_counter = 0;
	size_t i = 0;
	int num1 = 280;
	int num2 = 260;
	
	for (; i < ARR_SIZE; ++i)
	{
		ret = BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
	
	BSTInsert(tree, (void *)&num2);
	ret = BSTInsert(tree, (void *)&num1);
	
	ret = BSTPrev(ret);	
	ret = BSTPrev(ret);	
	
	if (*(int *)(BSTGetData(ret)) == num2)
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Prev: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Prev: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}


void TestBSTNext(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty ret = {NULL};
	size_t test_counter = 0;
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		ret = BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
	
	ret = BSTNext(ret);	/* 137 */
	ret = BSTNext(ret);	/* 220 */
	ret = BSTNext(ret);	/* 254 */
	ret = BSTNext(ret);	/* 270 */
	
	if (*(int *)(BSTGetData(ret)) == 270)
	{ ++test_counter; }
	
	if (test_counter == 1)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Next: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Next: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}


void TestBSTFind(void)
{
	bst_ty *tree = CreateTreeIMP();
	bst_itr_ty itr_test1 = {NULL};
	bst_itr_ty itr_test2 = {NULL};
	size_t test_counter = 0;
	size_t i = 0;
	int to_find1 = 100;
	int to_find2 = 284;
	int to_find3 = 888;
	
	
	for (; i < ARR_SIZE; ++i)
	{
		itr_test1 = BSTInsert(tree, (void *)&nums_hard_coded[i]);
	}
	
	/* When data is in tree */
	if (BSTIterIsEqual(BSTFind(tree, (void *)&to_find1), itr_test1))
	{ ++test_counter; }
	

	/* When data is in tree */
	itr_test2 = BSTInsert(tree, (void *)&to_find2);
	if (BSTIterIsEqual(BSTFind(tree, (void *)&to_find2), itr_test2))
	{ ++test_counter; }
	
	/* When data is not in tree */
	if (BSTIterIsEqual(BSTFind(tree, (void *)&to_find3), BSTEnd(tree)))
	{ ++test_counter; }
	
	if (test_counter == 3)
	{
		GREEN;
		PRINT_STATUS_MSG(Test Find: SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Test Find: FAILED);
		DEFAULT;
	}

	BTSDestroy(tree);
}


void TestBSTForEach(void)
{


}



/*-------------------------------Side Functions ------------------------------*/
static bst_ty *CreateTreeIMP(void)
{
	bst_ty *tree = NULL;
	void *param = NULL;
	
	tree = BSTCreate(CmpNumbersIMP, param);

	if (NULL == tree)
	{
		puts("Memory Allocation Failed");
		return NULL;
	}
	
	return tree;
}


static int CmpNumbersIMP(const void *num1_, const void *num2_, const void *param)
{
	UNUSED(param);
	
	return (*(int *)num2_ - *(int *)num1_);
}


static void GenerateRandomNumsIMP(int arr_[], size_t size)
{
	size_t num = 0;
	
	srand((unsigned)time(NULL));
	
	for (num = 0; num < size; ++num)
	{
		arr_[num] = rand() % 444;
	}
}


/*
typedef struct wizard
{
	char *first_name;
	char *last_name;
	char *house;
} wizard_ty;

wizard_ty harry = {"Harry", "Potter", "Gryffindor"};
wizard_ty hermione = {"Hermione", "Granger", "Gryffindor"};
wizard_ty ron = {"Ron", "Weasley", "Gryffindor"};
wizard_ty albus = {"Albus", "Dumbledore", "Gryffindor"};
wizard_ty draco = {"Draco", "Malfoy", "Slytherin"};
wizard_ty voldemort = {"Voldemort", "Lord", "Slytherin"};
wizard_ty severus = {"Severus", "Snape", "Slytherin"};
wizard_ty cedric = {"Cedric", "Diggory", "Hufflepuff"};
wizard_ty luna = {"Luna", "Lovegood", "Ravenclaw"};

int CmpFirstNames(const void *bst_name, const void *name, const void *param);

int CmpFirstNames(const void *bst_name, const void *name, const void *param)
{
	UNUSED(param);
	
	return strncmp((char *)bst_name, (char *)name, strlen(bst_name));
}
*/
