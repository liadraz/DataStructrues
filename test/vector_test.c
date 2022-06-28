/*******************************************************************************
******************************* - VECTOR - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION 	Build vector data structure
*	NAME 			Liad Raz
*
*******************************************************************************/

#include <stdio.h> 		/* puts, printf */

#include "vector.h"

vector_ty *TestVectorCreate(void);
void TestVectorGetArray(vector_ty *vector);
void TestVectorSetAndGet(vector_ty *vector);
void TestVectorShrink(vector_ty *vector);
void TestVectorResize(vector_ty *vector);
void TestVectorReserve(vector_ty *vector);
void TestVectorPushBack(vector_ty *vector);
void TestVectorPopBack(vector_ty *vector);
void TestVectorDestroy(vector_ty *vector);


int main(void)
{
	vector_ty *vector = NULL;

	puts("\n\t~~~~~~~~ Data Structures - Vector ~~~~~~~~");

	vector = TestVectorCreate();
	
	TestVectorGetArray(vector);
	TestVectorSetAndGet(vector);
	TestVectorShrink(vector);
	TestVectorReserve(vector);
	TestVectorResize(vector);
	TestVectorPushBack(vector);
	TestVectorPopBack(vector);
	TestVectorDestroy(vector);

	return 0;
}


vector_ty *TestVectorCreate(void)
{
	vector_ty *vector = NULL;
	size_t size = 20;
	size_t capacity = 100;
	
	vector = VectorCreate(capacity, size);

	puts("\n==> Vector Create");
	printf("Address\t\t%p\n", (void *)vector);
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));
	
	return vector;
}	


void TestVectorGetArray(vector_ty *vector)
{
	puts("\n==> Vector Get Array");
	printf("Get Array\t%p\n", (void *)VectorGetArray(vector));
}
	

void TestVectorSetAndGet(vector_ty *vector)
{
	size_t index = 2;
	int num = 1024;
	int *num_ptr = &num;
	int *get_ptr = NULL;
	
	VectorSet(vector, (int *)num_ptr, index);
	get_ptr = VectorGet(vector, index);
	
	puts("\n==> Vector Set and Get");
	printf("index\t\t%lu\n", index);
	printf("value\t\t%d\n", *(int *)get_ptr);
	
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));
}


void TestVectorShrink(vector_ty *vector)
{
	VectorShrink(vector);

	puts("\n==> Vector Shrink");
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));
}


void TestVectorReserve(vector_ty *vector)
{
	size_t new_capacity = 200;
	
	VectorReserve(vector, new_capacity);
	
	puts("\n==> Vector Reserve");
	printf("new_Capacity\t%lu\n", new_capacity);
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));
}


void TestVectorResize(vector_ty *vector)
{
	size_t shrink_new_size = 28;
	size_t grow_new_size = 198;
	
	VectorResize(vector, shrink_new_size);
	
	puts("\n==> Vector Resize");
	printf("SHRINK_Size\t%lu\n", shrink_new_size);
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));

	VectorResize(vector, grow_new_size);

	printf("\nGROW_Size\t%lu\n", grow_new_size);
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n", VectorCapacity(vector));
}


void TestVectorPushBack(vector_ty *vector)
{
	size_t index_size = 0;
	size_t i = 0;
	int num = 922;
	
	puts("\n==> Vector PushBack");
	puts("Start Point");
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n\n", VectorCapacity(vector));
	
	puts("PUSHBACK\tINDEX\t\tVALUE");
	for (; i < 20; ++i)
	{
		index_size = VectorSize(vector);
		VectorPushBack(vector, (void *)&num);
	
		printf("%lu", (i + 1));
		printf("\t\t%lu", index_size);
		printf("\t\t%d\n", *(int *)VectorGet(vector, index_size));
		
		++num;
	}
	
	puts("\nEnd Point");
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n\n", VectorCapacity(vector));
}


void TestVectorPopBack(vector_ty *vector)
{
	size_t i = 0;
	void *popback_ptr = 0;	
	
	VectorResize(vector, 215);

	puts("==> Vector PopBack");
	puts("Start Point");
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n\n", VectorCapacity(vector));
	
	puts("POPBACK\t\tINDEX\t\tVALUE");
	for (; i < 10; ++i)
	{
		popback_ptr = VectorPopBack(vector);

		printf("%lu", (i + 1));
		printf("\t\t%lu", VectorSize(vector));
/*		printf("\t\t%d\n", popback_ptr);*/
		printf("\t\t%d\n", *(int *)popback_ptr);
	}
	
	puts("\nEnd Point");
	printf("Size\t\t%lu\n", VectorSize(vector));
	printf("Capacity\t%lu\n\n", VectorCapacity(vector));
}


void TestVectorDestroy(vector_ty *vector)
{
	puts("\n==> Vector Destroyed");
	VectorDestroy(vector);
}


