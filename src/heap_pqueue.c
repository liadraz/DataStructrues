
/*******************************************************************************
****************************** - HEAP_PQUEUE - ********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation
*	AUTHOR 		Liad Raz
*
*******************************************************************************/

#include <stdio.h>			/* stderr */
#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */

#include "utilities.h"
#include "heap_pqueue.h"
#include "heap.h"

struct pqueue
{
    heap_ty *m_heap;
};


/*******************************************************************************
************************ Heap_PQueue Create ***********************************/
pqueue_ty *HeapPQueueCreate(PQCmpFunc cmp_func_p, const void *cmp_param)
{
      pqueue_ty *pqueue = NULL;

      ASSERT_NOT_NULL(cmp_func_p, "HeapPQueueCreate: Function is not valid");

      pqueue = (pqueue_ty *)malloc(sizeof(pqueue_ty));
      RETURN_IF_BAD(pqueue, "HeapPQueueCreate: Allocation Failure", NULL);

      pqueue->m_heap = HeapCreate(cmp_func_p, cmp_param);

      if (pqueue->m_heap == NULL)
      {
            free(pqueue);
            return NULL;
      }

      return pqueue;
}

/*******************************************************************************
************************ Heap_PQueue Destory **********************************/
void HeapPQueueDestroy(pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueDestroy: pqueue is not allocated");

      HeapDestroy(th_->m_heap);

      free(th_);
}

/*******************************************************************************
************************ Heap_PQueue Enqueue **********************************/
int HeapPQueueEnqueue(pqueue_ty *th_, void *data_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueEnqueue: pqueue is not allocated");

      return (HeapPush(th_->m_heap, data_));
}

/*******************************************************************************
************************ Heap_PQueue Dequeue **********************************/
void HeapPQueueDequeue(pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueDequeue: pqueue is not allocated");

      HeapPop(th_->m_heap);
}

/*******************************************************************************
************************ Heap_PQueue Peek *************************************/
void *HeapPQueuePeek(const pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueuePeek: pqueue is not allocated");

      return HeapPeek(th_->m_heap);
}

/*******************************************************************************
************************ Heap_PQueue IsEmpty **********************************/
int HeapPQueueIsEmpty(const pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueIsEmpty: pqueue is not allocated");

      return (HeapIsEmpty(th_->m_heap));
}

/*******************************************************************************
************************ Heap_PQueue Size *************************************/
size_t HeapPQueueSize(const pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueSize: pqueue is not allocated");

      return (HeapSize(th_->m_heap));
}

/*******************************************************************************
************************ Heap_PQueue Clear ************************************/
void HeapPQueueClear(pqueue_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueClear: pqueue is not allocated");

      while (!HeapPQueueIsEmpty(th_))
      {
            HeapPQueueDequeue(th_);
      }
}

/*******************************************************************************
************************ Heap_PQueue Erase ************************************/
void *HeapPQueueErase(pqueue_ty *th_, PQIsMatch match_func_, void *param_)
{
      ASSERT_NOT_NULL(th_, "HeapPQueueErase: pqueue is not allocated");
      ASSERT_NOT_NULL(match_func_, "HeapPQueueErase: Function is not valid");

      return HeapRemove(th_->m_heap, match_func_, param_);
}
