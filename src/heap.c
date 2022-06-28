
/*******************************************************************************
******************************** - HEAP - **************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation Heap
*	AUTHOR 		Liad Raz
*
*******************************************************************************/

#include <stdio.h>                  /* stderr */
#include <stdlib.h>			/* malloc, free*/
#include <assert.h>			/* assert */

#include "utilities.h"
#include "vector.h"
#include "heap.h"

#define VECTOR_CAPACITY       100LU /* Size of the allocated vector container */
#define VECTOR_SIZE           0LU 
#define ROOT_INDEX            0LU
#define FIRST_ELEMENT         0

#define PARENT_INDEX(i)       (i - 1) / 2
#define LEFT_CHILD_INDEX(i)   (2 * i) + 1
#define RIGHT_CHILD_INDEX(i)  (2 * i) + 2


struct heap
{
      vector_ty *m_elements;
      heap_cmp_ty m_cmp_func;
      const void *m_param;
};


static void SwapElements(vector_ty *th_, size_t first_index, size_t second_index);
static void PopIndexedElementIMP(heap_ty *th_, size_t index_);
static int BiggestChildIMP(heap_ty *th_, size_t first_index, size_t second_index);

static void HeapifyUpImp(heap_ty *th_, size_t index_);
static void HeapifyDownIMP(heap_ty *th_, size_t index_);


/*******************************************************************************
***************************** HeapCreate **************************************/
heap_ty *HeapCreate(heap_cmp_ty heap_cmp_func_ ,const void *param_)
{
      heap_ty *heap = NULL;
      vector_ty *elements = NULL;

      ASSERT_NOT_NULL(heap_cmp_func_, "HeapCreate: Function is not valid");

      heap = (heap_ty*)malloc(sizeof(heap_ty));
      RETURN_IF_BAD(heap, "HeapCreate: Allocation Error", NULL);

      elements = VectorCreate(VECTOR_CAPACITY, VECTOR_SIZE);
      if (!elements)
      {
           free(heap);
           return NULL;
      }

      heap->m_elements = elements;
      heap->m_cmp_func = heap_cmp_func_;
      heap->m_param = param_;

      return heap;
}

/*******************************************************************************
***************************** HeapDestroy *************************************/
void HeapDestroy(heap_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapDestroy: Heap is not allocated");

      /* Destroy the vector container */
      VectorDestroy(th_->m_elements);

      /* In dbg mode, break the heap structure members */
      DEBUG_MODE(
            th_->m_elements = DEAD_MEM(vector_ty *); 
            th_->m_cmp_func = NULL;
            th_->m_param = DEAD_MEM(void *);
      )

      /* free the heap structure */
      free(th_);
}


/*******************************************************************************
******************************* HeapPush **************************************/
int HeapPush(heap_ty *th_, void *to_add_)
{
      int ret_push = -1;

      ASSERT_NOT_NULL(th_, "HeapPush: Heap is not allocated");

      /* Push data to the end of the heap */
      ret_push = VectorPushBack(th_->m_elements, to_add_);

      /* check the return value, In case of failure return status */
      if (ret_push)
      {
            return ret_push;
      }

      /* heapifyUp the element which reorder the heap in order to find its right location */
      HeapifyUpImp(th_, VectorSize(th_->m_elements) - 1);

      return ret_push;
}


/*******************************************************************************
******************************* HeapPop ***************************************/
void HeapPop(heap_ty *th_)
{
      void *end_element = NULL;

      ASSERT_NOT_NULL(th_, "HeapPop: Heap is not allocated");
      assert(1 != HeapIsEmpty(th_) && "HeapPop: Cannot Pop an empty heap");

      /* Get the last element */
      end_element = VectorGet(th_->m_elements, VectorSize(th_->m_elements) - 1);

      /* Replace the root element with the last element */
      VectorSet(th_->m_elements, end_element, FIRST_ELEMENT);

      /* pop the last element - which currently exists in the root data */
      VectorPopBack(th_->m_elements);
      
      /* heapifyDown the current root element to the right location in heap */
      HeapifyDownIMP(th_, 0);
}


/*******************************************************************************
****************************** HeapPeek ***************************************/
void *HeapPeek(const heap_ty *th_)
{
      void *ret_data = NULL;

      ASSERT_NOT_NULL(th_, "HeapPeek: Heap is not allocated");
      assert(1 != HeapIsEmpty(th_) && "HeapPeek: Cannot Pop an empty heap");

      ret_data = VectorGet(th_->m_elements, FIRST_ELEMENT);

      return ret_data;
}


/*******************************************************************************
****************************** HeapSize ***************************************/
size_t HeapSize(const heap_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapSize: Heap is not allocated");

      return VectorSize(th_->m_elements);
}


/*******************************************************************************
***************************** HeapIsEmpty *************************************/
int HeapIsEmpty(const heap_ty *th_)
{
      ASSERT_NOT_NULL(th_, "HeapIsEmpty: Heap is not allocated");

      return (HeapSize(th_) == 0);
}


/*******************************************************************************
***************************** HeapRemove **************************************/
void *HeapRemove(heap_ty *th_, should_remove_ty remove_func_, const void *param_)
{
      void *removed_element = NULL;
      size_t index = 0;
      size_t vector_size = 0;
      int is_removed =-1;

      ASSERT_NOT_NULL(th_, "HeapRemsize_t index_e: Heap is not allocated");
      ASSERT_NOT_NULL(remove_func_, "HeapRemove: Function is not valid");

      vector_size = VectorSize(th_->m_elements);

      for (index = 0; index < vector_size; ++index)
      {
            /* Check if current element meets remove_func requirements */
            is_removed = remove_func_(VectorGet(th_->m_elements, index), param_);

            /* In case, curr element found */
            if (is_removed)
            {
                  /* keep the removed element */
                  removed_element = VectorGet(th_->m_elements, index);
                  /* Replace the last element in the removed element's index */
                  PopIndexedElementIMP(th_, index);

                  /* Reorder the heap to satisfy the heap property in each subtree */
                  HeapifyDownIMP(th_, index);
                  break;
            }
      }

      return removed_element;
}


/*******************************************************************************
*************************** Auxilary Functions ********************************/
static void HeapifyUpImp(heap_ty *th_, size_t index_)
{
      void *parent = NULL;
      void *child = NULL;
      size_t parent_index = PARENT_INDEX(index_);
      int cmp_ret = 0;

      /* In first insertion, no reordering is needed */
      if (!index_)
      {
            return;
      }

      parent = VectorGet(th_->m_elements ,parent_index);
      child = VectorGet(th_->m_elements ,index_);

      /* (parent < child) Negative return value */
      cmp_ret = th_->m_cmp_func(child, parent, th_->m_param);
      /* TODO: One of the child is not checked Check Before sending it Biggest Child*/

      /* Keep swap child with small parent */
      if (!cmp_ret)
      {
            SwapElements(th_->m_elements, index_, parent_index);
            HeapifyUpImp(th_, parent_index);
      }
}


static void HeapifyDownIMP(heap_ty *th_, size_t index_)
{
      void *parent = NULL;
      void *child = NULL;
      size_t big_child_index = 0;
      size_t left_index = RIGHT_CHILD_INDEX(index_);
      size_t right_index = LEFT_CHILD_INDEX(index_);
      int cmp_ret = 0;

      /* Trivial Case, When data has no more children return the function */
      /* TODO: size is out of range falls in GetVector */
      if (left_index >= VectorSize(th_->m_elements))
      {
            return;
      }

      /* Trivial Case, When data has only left child keep heapifing down */
      /*if (left_index == VectorSize(th_->m_elements))
      {
            right_index = left_index;
      }*/
      
      /* TODO: parent to child than result to parent */
      big_child_index = BiggestChildIMP(th_, right_index, left_index);

      parent = VectorGet(th_->m_elements, index_);
      child = VectorGet(th_->m_elements, big_child_index);

      /* (parent < child) Positive return value */
      cmp_ret = th_->m_cmp_func(child, parent, th_->m_param);

      /* In case, parent is smaller than chils
            Swap parent with child */
      if (cmp_ret)
      {
            SwapElements(th_->m_elements, big_child_index, index_);
            HeapifyDownIMP(th_, big_child_index);
      }
}

static void SwapElements(vector_ty *th_, size_t first_index, size_t second_index)
{
      void *first = VectorGet(th_, first_index);
      void *second = VectorGet(th_, second_index);

      VectorSet(th_, first, second_index);
      VectorSet(th_, second, first_index);
}

static void PopIndexedElementIMP(heap_ty *th_, size_t index_)
{
      void *end_element = NULL;

      /* Get the last element */
      end_element = VectorGet(th_->m_elements, VectorSize(th_->m_elements) - 1);
      
      /* Replace the indexed element with the last returned element */
      VectorSet(th_->m_elements, end_element, index_);

      /* pop the last element - which currently exists in the root data */
      VectorPopBack(th_->m_elements);
}

static int BiggestChildIMP(heap_ty *th_, size_t first_index, size_t second_index)
{
      void *first = VectorGet(th_->m_elements, first_index);
      void *second = VectorGet(th_->m_elements, second_index);

      int cmp_ret = th_->m_cmp_func(first, second, th_->m_param);

      return ((cmp_ret >= 0) * first_index + (cmp_ret < 0) * second_index);
}