
/*******************************************************************************
****************************** - BST_REC - *************************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation of Binary Sort Tree with Recursive. 
                        AVL Tree.
*	AUTHOR 		Liad Raz
*
*******************************************************************************/

#include <stdio.h>			/* stderr */
#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */

#include "utilities.h"
#include "bst_rec.h"

#define SUCCESS   0

typedef struct node avl_node_ty;
typedef enum side { LEFT = 0 , RIGHT = 1 } side_ty;


struct node
{
      avl_node_ty *m_children[2];
      void *m_data;
      size_t m_height;
      
      DEBUG_MODE(
            avl_ty *m_tree;
      ) 
};

struct avl
{
	avl_node_ty m_dummy;
	cmp_func_ty m_cmp_func;       /* Comparison two value data */
	const void *m_param;          /* */
};


/* Used in AVLDestroy, Destory nodes using a Post Order traverse */
static void DestroyNodesIMP(avl_node_ty *to_free_);

/* Used in AVLInsert, Insert a new Node in the correct location */
static avl_node_ty *InsertNodeIMP(avl_ty *th_, avl_node_ty *curr_, avl_node_ty *to_add_);
static avl_node_ty *CreateInitNodeIMP(void *data_);

/* Used in AVLRemove, Remove node from the specific location */
static avl_node_ty *RemoveNodeImp(avl_ty *th_, avl_node_ty *sub_tree_, const void *to_remove_);

/* Used in AVLSize, Count all elements in tree using recursion */
static size_t CountSizeImp(avl_node_ty *sub_tree_);

/* Used in AVLFind, look for a specific key in tree */
static void *FindDataIMP(const avl_ty *th_, avl_node_ty *sub_tree_, const void *to_find_);

/* Used in AVLForEach, execute the provided function on each node. */
static int PREForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_ , const void *param_);
static int INForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_ , const void *param_);
static int POSTForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_ ,const void *param_);

/* Auxilary Functions   */
static avl_node_ty *GetRootIMP(const avl_ty *th_);
static int GetCmpResultIMP(const avl_ty *th_, const void *data_a_, const void *data_b_);
static side_ty WhichSideToFollowIMP(int cmp_ret);

static int PrintTreeRecursive(node_ty *node, int is_left, int offset, int depth, char s[20][255]);

/*******************************************************************************
********************************* AVLCreate ***********************************/
avl_ty *AVLCreate(cmp_func_ty CmpFunc_, const void *param_)
{
      avl_ty *bst = NULL;

      ASSERT_NOT_NULL(CmpFunc_, "AVLCreate: Function is not valid");

      /* Allocate memory for the BST Managing struct */
      bst = (avl_ty *)malloc(sizeof(avl_ty));
      RETURN_IF_BAD(bst, "AVLCreate: Allocation Error", NULL);

      /* Init BST Managing struct fields */
      bst->m_dummy.m_children[LEFT] = NULL;
      bst->m_dummy.m_children[RIGHT] = DEAD_MEM(avl_node_ty *);
      bst->m_dummy.m_data = DEAD_MEM(void *);
      bst->m_dummy.m_height = 0;
      DEBUG_MODE(
            bst->m_dummy.m_tree = bst;
      )

      bst->m_cmp_func = CmpFunc_;
      bst->m_param = param_;

      return bst;
}


/*******************************************************************************
********************************* AVLDestory **********************************/
void AVLDestory(avl_ty *th_)
{
      ASSERT_NOT_NULL(th_, "AVLDestory: Function is not valid");

      /* destroy all nodes in tree */
      DestroyNodesIMP(GetRootIMP(th_));

      /* In dbg mode, break all tree's struct fields */
      DEBUG_MODE(
            th_->m_cmp_func = NULL;
            th_->m_param = DEAD_MEM(void *);
      )

      /* free the tree managing structure */
      free(th_);
}

static void DestroyNodesIMP(avl_node_ty *to_free_)
{
      /* Trivial Case, 
      return when getting to the end of a node */
      if (NULL == to_free_)
      {
            return;     
      }

      /* SubProblem, 
      Traverse the tree in post order (Left-Right-Root), 
      and free nodes respectively */
      DestroyNodesIMP(to_free_->m_children[LEFT]);
      DestroyNodesIMP(to_free_->m_children[RIGHT]);

      /* In dbg mode, break each node's struct fields */
      DEBUG_MODE(
            to_free_->m_children[LEFT] = DEAD_MEM(avl_node_ty *);
            to_free_->m_children[RIGHT] = DEAD_MEM(avl_node_ty *);
            to_free_->m_data = DEAD_MEM(void *);
      )
      
      free(to_free_);
}


/*******************************************************************************
********************************* AVLInsert ***********************************/
int AVLInsert(avl_ty *th_, void *new_data_)
{
      avl_node_ty *new_node = NULL;

      ASSERT_NOT_NULL(th_, "AVLInsert: Function is not valid");
      ASSERT_NOT_NULL(th_->m_cmp_func, "AVLInsert: Function is not valid");

      new_node = CreateInitNodeIMP(new_data_);
      /* In case Insertion failed return 1 as a failure */
      if (NULL == new_node)
      {
            return 1;
      }
      
      /* When tree is empty just insert the first node */
      if (AVLIsEmpty(th_))
      {
            th_->m_dummy.m_children[LEFT] = new_node;
            return 0;
      }

      InsertNodeIMP(th_, GetRootIMP(th_), new_node);

      return 0;
}

static avl_node_ty *InsertNodeIMP(avl_ty *th_, avl_node_ty *curr_, avl_node_ty *to_add_)
{
      side_ty side = 0;
      int cmp_ret = 0;

      /* Trivial Case, 
      When reached the end of a node, return a new node, 
      which will be the place to be inserted */
      if (curr_ == NULL)
      {
            return to_add_;
      }

      /* Compare two data using the cmp_function */
      cmp_ret = GetCmpResultIMP(th_, curr_->m_data, to_add_->m_data);

      assert(cmp_ret != 0 && "AVLInsert: Data already Exists in Tree");

      /* Get the side to follow */
      side = WhichSideToFollowIMP(cmp_ret);

      /* SubProblem,
      Follow the correct side path, 
      and on the way back assign curr parent to its correct side child */
      curr_->m_children[side] = InsertNodeIMP(th_, curr_->m_children[side], to_add_);

      return curr_; 
}

static avl_node_ty *CreateInitNodeIMP(void *data_)
{
      avl_node_ty *new_node = NULL;

      /* Allocate and check a new node */
      new_node = (avl_node_ty *)malloc(sizeof(avl_node_ty));
      RETURN_IF_BAD(new_node, "AVLInsert: Allocation Error", NULL);

      /* Init node's members */
      new_node->m_children[LEFT] = NULL;
      new_node->m_children[RIGHT] = NULL;
      new_node->m_data = data_;
      new_node->m_height = 0;

      return new_node;
}


/*******************************************************************************
********************************* AVLRemove ***********************************/
void AVLRemove(avl_ty *th_, const void *to_remove)
{
      ASSERT_NOT_NULL(th_, "AVLRemove: BST is NULL");
      ASSERT_NOT_NULL(th_->m_cmp_func, "AVLRemove: Function is not valid");

      RemoveNodeImp(th_, GetRootIMP(th_), to_remove);
}

static avl_node_ty *RemoveNodeImp(avl_ty *th_, avl_node_ty *sub_tree_, const void *to_remove_)
{
      avl_node_ty *left = NULL;
      avl_node_ty *right = NULL;
      side_ty side = 0;
      int cmp_ret = GetCmpResultIMP(th_, sub_tree_->m_data, to_remove_);
      
      ASSERT_NOT_NULL(sub_tree_, "AVLRemove: Data does not exist in tree");

      /* Trivial Case, 
      In case user and tree data are matched */
      if (0 == cmp_ret)
      {
            /* keep to_removed node children */
            left = sub_tree_->m_children[LEFT];
            right = sub_tree_->m_children[RIGHT];
            
            /* free to_removed node */
            free(sub_tree_);

            /* In these two cases, 
            1. when to_remove has 2 children
            2. when to_remove has only Right child.
                  Invoke InsertNodeIMP, which return
                  the right child of the sub_tree_ */
            if (right != NULL)
            {
                  return InsertNodeIMP(th_, left, right);
            }
            /* In these cases,
            - Node is a leaf (return will be NULL)
            - to_remove has only Left child return it */
            return left;
      }

      /* Get the side to follow */
      side = WhichSideToFollowIMP(cmp_ret);

      /* SubProblem, 
      Keep looking for to_remove's data 
      - When function starts to return ==>
            when to_remove has 2 children
            a left child will be assigned to the returned sub_tree_ */
      sub_tree_->m_children[side] = RemoveNodeImp(th_, sub_tree_->m_children[side], to_remove_);

      return sub_tree_;
}

/*******************************************************************************
********************************* AVLSize *************************************/
size_t AVLSize(const avl_ty *th_)
{
      ASSERT_NOT_NULL(th_, "AVLSize: BST is NULL");

      /* Invoke count size recursion function, with the first element in the tree */
      return CountSizeImp(GetRootIMP(th_));
}     

static size_t CountSizeImp(avl_node_ty *sub_tree_)
{
      size_t counter = 0;
      ++counter;

      /* Trivial Case, 
      start the counting with zero, when getting to the end of a node */
      if (sub_tree_ == NULL)
      {
            return 0;
      }

      /* SubProblem, 
      count each side of the subtree and add it to the total amount */
      counter += CountSizeImp(sub_tree_->m_children[LEFT]);
      counter += CountSizeImp(sub_tree_->m_children[RIGHT]);

      return counter;
}


/*******************************************************************************
********************************* AVLIsEmpty **********************************/
int AVLIsEmpty(const avl_ty *th_)
{
      ASSERT_NOT_NULL(th_, "AVLIsEmpty: BST is NULL");

      return (NULL == th_->m_dummy.m_children[LEFT]);
}

/*******************************************************************************
********************************* AVLFind *************************************/
void *AVLFind(const avl_ty *th_, const void *to_find_)
{
      ASSERT_NOT_NULL(th_, "AVLFind: BST is NULL");

      return FindDataIMP(th_, GetRootIMP(th_), to_find_);
}

static void *FindDataIMP                                                \
(const avl_ty *th_, avl_node_ty *sub_tree_, const void *to_find_)
{
      int cmp_ret = 0;
      side_ty side = 0;

      /* Trivial Case, 
      return NULL, if current Node is a leaf or the tree is Empty */
      if (!sub_tree_)
      {
            return NULL;
      }
      
      cmp_ret = GetCmpResultIMP(th_, sub_tree_->m_data, to_find_);

      /* Trivial Case, 
      return node's data when user and tree keys are matched */
      if (0 == cmp_ret)
      {
            return sub_tree_->m_data;
      }

      /* In case cmp_ret is positive to_find value is smaller than current sub_node
      go to the LEFT Side. */
      side = WhichSideToFollowIMP(cmp_ret);

      /* SubProblem, 
      Keep looking for to_find in corect child side */
      return FindDataIMP(th_, sub_tree_->m_children[side], to_find_);
}

/*******************************************************************************
********************************* AVLForEach **********************************/
int AVLForEach(const avl_ty *th_, ordering_ty ordering_, exe_func_ty ExeFunc_ ,const void *param_)
{
      int status = -1;

      ASSERT_NOT_NULL(th_, "AVLForEach: BST is NULL");
      ASSERT_NOT_NULL(ExeFunc_, "AVLForEach: Function is not valid");

      /* Invoke the desired ordering */
      switch (ordering_)
      {
            case (PRE):
            {
                  status = PREForEachIMP(GetRootIMP(th_), ExeFunc_, param_);
                  break;
            }
            case (IN):
            {
                  status = INForEachIMP(GetRootIMP(th_), ExeFunc_, param_);
                  break;
            }
            case (POST):
            {
                  status = POSTForEachIMP(GetRootIMP(th_), ExeFunc_, param_);
                  break;
            }
      }

      return status;
}

static int PREForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_, const void *param_)
{
      int status = 0;

      /* Trivial Case, 
      return 0 when reaching the end of a leaf node */
      if (NULL == sub_tree_)
      {
            return 0;
      }

      /* exe function on current sub_tree node (ROOT) */
      status = ExeFunc_(sub_tree_->m_data, param_);
      /* In case of failure return status */
      if (status != 0)
      {
            return status;
      }

      /*  Invoke func on LEFT side child */
      status = PREForEachIMP(sub_tree_->m_children[LEFT], ExeFunc_, param_);
      /* In case of failure return status */
      if (status != 0)
      {
            return status;
      }

      /*  Invoke func on RIGHT side child */
      status = PREForEachIMP(sub_tree_->m_children[RIGHT], ExeFunc_, param_);

      return status;
}


static int INForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_, const void *param_)
{
      int status = 0;

      /* Trivial Case, 
      return 0 when reaching the end of a leaf node */
      if (NULL == sub_tree_)
      {
            return 0;
      }

      /*  Invoke func on Left side child */
      status = INForEachIMP(sub_tree_->m_children[LEFT], ExeFunc_, param_);
      /* In case of failure return status */
      if (status != 0)
      {
            return status;
      }
      
      /* exe function on current sub_tree node (ROOT) */
      status = ExeFunc_(sub_tree_->m_data, param_);
      if (status != 0)
      {
            return status;
      }

      /*  Invoke func on Right side child */
      status = INForEachIMP(sub_tree_->m_children[RIGHT], ExeFunc_, param_);

      return status;
}


static int POSTForEachIMP(avl_node_ty *sub_tree_, exe_func_ty ExeFunc_, const void *param_)
{
      int status = 0;

      /* Trivial Case, 
      return 0 when reaching the end of a leaf node */
      if (NULL == sub_tree_)
      {
            return 0;
      }

      /*  Invoke func on LEFT side child */
      status = POSTForEachIMP(sub_tree_->m_children[LEFT], ExeFunc_, param_);
      /* In case of failure return status */
      if (status != 0)
      {
            return status;
      }
      
      /*  Invoke func on RIGHT side child */
      status = POSTForEachIMP(sub_tree_->m_children[RIGHT], ExeFunc_, param_);
      /* In case of failure return status */
      if (status != 0)
      {
            return status;
      }
      
      /* exe function on current sub_tree node (ROOT) */
      status = ExeFunc_(sub_tree_->m_data, param_);

      return status;
}


/*******************************************************************************
*************************** Auxilary Functions ********************************/
static avl_node_ty *GetRootIMP(const avl_ty *th_)
{
      return th_->m_dummy.m_children[LEFT];
}


static int GetCmpResultIMP(const avl_ty *th_, const void *data_a_, const void *data_b_)
{
      int cmp_ret = 0;
      
      /* Compare two data using the cmp_function */
      cmp_ret = th_->m_cmp_func(data_a_, data_b_, th_->m_param);

      return cmp_ret;
}

static side_ty WhichSideToFollowIMP(int cmp_ret)
{
      /* When cmp_ret value is negative (data_b_ > data_a_)
      means follow the Right Side. */
      return ((cmp_ret >= 0) * LEFT + (cmp_ret < 0) * RIGHT);
}




/*******************************************************************************
******************************** Unit Tests ***********************************/
/* CREDIT TO SECOND ANSWER HERE: https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console */
/* SOME CHANGES ADDED BY YANAL */

static int PrintTreeRecursive(avl_node_ty *node, int is_left, int offset, int depth, char s[20][255])
{
	char b[20];
    int width = 5;
	int i = 0;
	int left = 0;
	int right = 0;

    if (!node) return 0;

	/* Works only if you inserted (void*)1, (void*)2, etc. */
/*    sprintf(b, "(%s)", (char *)(node->data));*/
    sprintf(b, "(%03d)", *(int *)(node->m_data));
	/* If you use pointers to integers, then use the following line: */
    /* sprintf(b, "(%03d)", *((int*)node->data)); */

    left  = PrintTreeRecursive(node->m_children[LEFT],  1, offset, depth + 1, s);
    right = PrintTreeRecursive(node->m_children[RIGHT], 0, offset + left + width, depth + 1, s);

	for (i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }

	return left + width + right;
}

void PrintTree(avl_ty *tree)
{
	int i;
    char s[20][255];

    for (i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    PrintTreeRecursive(GetRootIMP(tree), 0, 0, 0, s);

    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}