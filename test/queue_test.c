/*******************************************************************************
**************************** - QUEUE - *****************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Tests
*	AUTHOR 			Liad Raz
* 
*******************************************************************************/

#include <stdio.h> /* printf, puts */
#include <stddef.h> /* size_t */

#include "utilities.h" /*RED, GREEN, DEFAULT*/
#include "queue.h"

/*Test Functions*/
void TestQueuCreate(void);
void TestQueueEnqueue(void);
void TestQueueDequeue(void);
void TestQueueIsEmpty(void);
void TestQueueAppend(void);

/*side function*/
void PrintQueue(queue_ty *queue);

int main(void)
{
	TestQueuCreate();
	TestQueueEnqueue();
	TestQueueDequeue();
	TestQueueIsEmpty();
/*	TestQueueAppend();*/
	
	return 0;
}


void TestQueuCreate(void)
{
	queue_ty *new_queue = QueueCreate();
	
	puts("----- Test QueueCreate -----");
	
	if (NULL == new_queue)
	{
		RED;
		puts("- Queu Creation Fail -");
	}
	else
	{
		GREEN;
		puts("- Queu Creation Success -");
	}
	
	DEFAULT;
	QueueDestroy(new_queue);
}


void TestQueueEnqueue(void)
{
	queue_ty *new_queue = QueueCreate();
	char *person1 = "Amram";
	char *person2 = "Adir";
	char *person3 = "Yanal";	
	
	if (NULL == new_queue)
	{
		RED;
		puts("- Queu Creation Fail -");
		DEFAULT;
	}
	
	puts("\n ----- Test QueueEnqueue -----");
	
	QueueEnqueue(new_queue, (void *)person1);
	QueueEnqueue(new_queue, (void *)person2);
	QueueEnqueue(new_queue, (void *)person3);
	
	if (person1 == QueuePeek(new_queue))
	{
		GREEN;
		puts("\t- Success -");
	}
	else
	{
		RED;
		puts("\t- Failure -");
	}
	
	DEFAULT;
	
	PrintQueue(new_queue);
	
	QueueDestroy(new_queue);
}


void TestQueueDequeue(void)
{
	queue_ty *new_queue = QueueCreate();
	char *person1 = "Amram";
	char *person2 = "Adir";
	char *person3 = "Yanal";	
	
	if (NULL == new_queue)
	{
		RED;
		puts("- Queu Creation Fail -");
		DEFAULT;
	}
	
	puts("\n ----- Test QueueDequeue -----");
	
	QueueEnqueue(new_queue, (void *)person1);
	QueueEnqueue(new_queue, (void *)person2);
	QueueEnqueue(new_queue, (void *)person3);
	
	puts("\n- before QueueDequeue -");
	
	printf("Queu size = %lu\n", QueueSize(new_queue));
	
	puts("\n- after QueueDequeue twice -");

	QueueDequeue(new_queue);
	QueueDequeue(new_queue);
	
	printf("Queu size = %lu\n", QueueSize(new_queue));	
	
	if (1 == QueueSize(new_queue))
	{
		GREEN;
		puts("\t- Success -");
	}
	else
	{
		RED;
		puts("\t- Failure -");
	}
	
	DEFAULT;
	QueueDestroy(new_queue);	
}

 
void TestQueueIsEmpty(void)
{
	queue_ty *new_queue = QueueCreate();
	char *person1 = "Amram";

	if (NULL == new_queue)
	{
		RED;
		puts("- Queu Creation Fail -");
		DEFAULT;
	}
	
	puts("\n ----- Test QueueIsEmpty -----");
	
	QueueEnqueue(new_queue, (void *)person1);

	
	printf("Queu size = %lu\n", QueueSize(new_queue));
	printf("is empty status = %d\n", QueueIsEmpty(new_queue));
	
	if (!QueueIsEmpty(new_queue))
	{
		GREEN;
		puts("\t- Success -");
	}
	else
	{
		RED;
		puts("\t- Failure -");
	}
	

	QueueDequeue(new_queue);
	
	DEFAULT;
	puts("\n- second case -\n");
	printf("Queu size = %lu\n", QueueSize(new_queue));
	printf("is empty status = %d\n", QueueIsEmpty(new_queue));
	
	if (QueueIsEmpty(new_queue))
	{
		GREEN;
		puts("\t- Success -");
	}
	else
	{
		RED;
		puts("\t- Failure -");
	}
	
	DEFAULT;
	QueueDestroy(new_queue);
}

/*
void TestQueueAppend(void)
{	
	queue_ty *new_queue1 = QueueCreate();
	char *person1 = "Amram";
	char *person2 = "Adir";
	char *person3 = "Yanal";
	
	queue_ty *new_queue2 = QueueCreate();	
	char *person4 = "Yarden";
	char *person5 = "Moria";
	size_t size_after_append = 0;
	
	if (NULL == new_queue1)
	{
		RED;
		puts("- Queu Creation Fail -");
		DEFAULT;
	}
	
	if (NULL == new_queue2)
	{
		QueueDestroy(new_queue1);
		RED;
		puts("- Queu Creation Fail -");
		DEFAULT;
	}
	
	QueueEnqueue(new_queue1, (void *)person1);
	QueueEnqueue(new_queue1, (void *)person2);
	QueueEnqueue(new_queue1, (void *)person3);
	
	QueueEnqueue(new_queue2, (void *)person4);
	QueueEnqueue(new_queue2, (void *)person5);
	
	puts("\n ----- Test QueueAppend -----");
	
	puts("\n- before QueueAppend -");
	
	puts("\nqueue1:");
	PrintQueue(new_queue1);
	
	puts("\nqueue2:");
	PrintQueue(new_queue2);
	
	QueueDequeue(new_queue1);
	QueueDequeue(new_queue1);
	QueueDequeue(new_queue1);
	
	QueueDequeue(new_queue2);
	QueueDequeue(new_queue2);

	QueueEnqueue(new_queue1, (void *)person1);
	QueueEnqueue(new_queue1, (void *)person2);
	QueueEnqueue(new_queue1, (void *)person3);
	
	QueueEnqueue(new_queue2, (void *)person4);
	QueueEnqueue(new_queue2, (void *)person5);
	
	puts("\n- after QueueAppend -");
	
	
	QueueAppend(new_queue1, new_queue2);
	size_after_append = QueueSize(new_queue1);
	
	puts("\nqueue1:");
	PrintQueue(new_queue1);	
	
	puts("\nqueue2:");
	PrintQueue(new_queue2);
	
	if (5 == size_after_append)
	{
		GREEN;
		puts("\t- Success -");
	}
	else
	{
		RED;
		puts("- Queu Creation Fail -");
	}
	
	DEFAULT;
	QueueDestroy(new_queue1);
	QueueDestroy(new_queue2);
}
*/

/*- side functions -*/

void PrintQueue(queue_ty *queue)
{
	size_t queue_size = QueueSize(queue);
	size_t turn = 1;
	void *tmp = NULL;
	
	printf("Queu size = %lu\n", queue_size);
	
	while (queue_size > 0)
	{
		printf("turn position = %lu person = %s\n", turn, (char *)QueuePeek(queue));
		
		tmp = QueuePeek(queue);
		QueueEnqueue(queue, tmp);
		QueueDequeue(queue);
		--queue_size;
		++turn;
	}
}



