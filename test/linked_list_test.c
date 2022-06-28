/*******************************************************************************
***************************** - LINKED LIST - **********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Tests linked list data struct
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdio.h> /* printf, putchar, puts*/

#include "utilities.h"
#include "linked_list.h"


void TestLListCreate(void);
void TestLListDestroy(void);
void TestLListInsert(void);
void TestLListRemove(void);
void TestLListBegin(void);
void TestLListEnd(void);
void TestLListIsEmpty(void);
void TestLListCount(void);
void TestLListSetData(void);

static void PrintNodeIteratorStatus(llist_itr_ty iterator);
static void PrintLList(llist_ty *list);


int main(void)
{
	puts("\n\t~~~~~~~~ DS - LINKED LIST ~~~~~~~~");

	TestLListCreate();
	
	TestLListInsert();
	TestLListRemove();

	TestLListBegin();
	TestLListEnd();
	TestLListIsEmpty();
	TestLListCount();
	/*	
	TestLListSetData();
*/
	
	return 0;
}

void TestLListCreate(void)
{
	llist_ty *llist = LListCreate();

	puts("\n==> Create");
	PRINT_PTR_ADDRESS(llist);
	
	if (NULL != llist)
	{
		GREEN;
		PRINT_STATUS_MSG(Create SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Create FAILED);
		DEFAULT;
	}
	
	LListDestroy(llist);
	puts("==> Destroy");
}


void TestLListInsert(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty itr = LListBegin(llist);
	
	char *str1 = "THIRD\nAGE = 40\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 83\nName: Amram Even Tzur\n";
	
	puts("\n==> Insert");
	itr = LListInsert(itr, (void *)str1);
	itr = LListInsert(itr, (void *)str2);
	itr = LListInsert(itr, (void *)str3);
	
	PrintLList(llist);
	
	if (NULL != itr.pointing_node)
	{
		GREEN;
		PRINT_STATUS_MSG(Insert SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Insert FAILED);
		DEFAULT;	
	}
	
	LListDestroy(llist);
}

void TestLListRemove(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty itr = LListBegin(llist);
	
	char *str1 = "THIRD\nAGE = 40\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 83\nName: Amram Even Tzur\n";
	
	puts("\n==> Insert 3 Nodes");
	itr = LListInsert(itr, (void *)str1);
	itr = LListInsert(itr, (void *)str2);
	itr = LListInsert(itr, (void *)str3);
	
	puts("==> Remove 2 Nodes");
	itr = LListRemove(itr);
	itr = LListRemove(itr);
	
	PrintLList(llist);
	
	if (NULL != itr.pointing_node)
	{
		GREEN;
		PRINT_STATUS_MSG(Remove SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Remove FAILED);
		DEFAULT;	
	}
	
	LListDestroy(llist);
}

void TestLListBegin(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty begin_itr = {NULL};
	
	begin_itr = LListBegin(llist);

	puts("\n==> Begin");
	PRINTM_PTR_ADDRESS(Current Begin Address, begin_itr.pointing_node);
	
	PrintNodeIteratorStatus(begin_itr);
	
	LListDestroy(llist);
}

void TestLListEnd(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty end_itr = {NULL};
	
	end_itr = LListEnd(llist);

	puts("\n==> End");
	PRINTM_PTR_ADDRESS(Current End Address, end_itr.pointing_node);
	PRINTM_PTR_ADDRESS(Current Begin Address, LListBegin(llist).pointing_node);
	
	PrintNodeIteratorStatus(end_itr);
	
	LListDestroy(llist);
}

void TestLListIsEmpty(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty itr = LListBegin(llist);
	int num = 120;
	
	puts("\n==> IsEmpty");
	PRINT_MSG(list Is Empty);
	if (1 == LListIsEmpty(llist))
	{
		GREEN;
		PRINT_STATUS_MSG(Is Empty);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(Not Empty);
		DEFAULT;	
	}
	
	itr = LListInsert(itr, (void *)&num);
	itr = LListInsert(itr, (void *)&num);
	
	PRINT_MSG(Inserted 2 nodes);
	if (1 == LListIsEmpty(llist))
	{
		RED;
		PRINT_STATUS_MSG(Is Empty);
		DEFAULT;
	}
	else
	{
		GREEN;
		PRINT_STATUS_MSG(Not Empty);
		DEFAULT;	
	}
	
	LListDestroy(llist);
}

void TestLListCount(void)
{
	size_t counter = 0;
		
	llist_ty *llist = LListCreate();
	llist_itr_ty itr = LListBegin(llist);
	
	char *str1 = "THIRD\nAGE = 40\nName: Liad Raz\n";
	char *str2 = "SECOND\nAGE = 24\nName: Moria Gudiner\n";
	char *str3 = "FIRST\nAGE = 83\nName: Amram Even Tzur\n";
	
	itr = LListInsert(itr, (void *)str1);
	itr = LListInsert(itr, (void *)str2);
	itr = LListInsert(itr, (void *)str2);
	itr = LListRemove(itr);
	itr = LListInsert(itr, (void *)str3);
	itr = LListInsert(itr, (void *)str3);
	itr = LListRemove(itr);

	puts("\n==> Counter");
	counter = LListCount(llist);
	
	PRINT_MSG(5 Inserted 2 Nodes Removed);
	PRINTM_SIZE_T(Number Of Nodes, counter);
	
	LListDestroy(llist);
}

/* TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO */
/*
void TestLListSetData(void)
{
	llist_ty *llist = LListCreate();
	llist_itr_ty itr = {NULL};
	
	char *str = "NODE\nName: Liad Raz\n";
	char *new_str = "Biscuit The Cat\n";
	
	puts("\n==> GetData");
	PRINT_MSG(Data From Tail);
	PRINT_PTR_ADDRESS(LListGetData(LListBegin(llist));

	itr = LListInsert(LListBegin(llist), (void *)str1);

	puts("\n==> SetData");
	PRINT_MSG(Data From Tail);
	PRINT_PTR_ADDRESS(LListSetData(LListBegin(llist), (char *)new_str));

	PRINT_PTR_ADDRESS(LListGetData(LListBegin(llist)));
}
*/

static void PrintNodeIteratorStatus(llist_itr_ty iterator)
{
	if (NULL != iterator.pointing_node)
	{
		GREEN;
		PRINT_STATUS_MSG(SUCCESS);
		DEFAULT;
	}
	else
	{
		RED;
		PRINT_STATUS_MSG(FAILED);
		DEFAULT;	
	}
}


static void PrintLList(llist_ty *list)
{
	llist_itr_ty tmp_itr = LListBegin(list);
	size_t i = 0;	
	size_t len = LListCount(list);
	
	puts("------------------");
	for (;i < len; ++i)
	{
		printf("NODE (%lu)\n%s", i + 1, (char *)LListGetData(tmp_itr));		
		puts("------------------");
		puts("\tV");
		puts("------------------");
		
		tmp_itr = LListNext(tmp_itr);
	}
}

