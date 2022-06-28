
/*******************************************************************************
****************************** - BINARY_TREE - *********************************
***************************** DATA STRUCTURES **********************************
*
*	DESCRIPTION		Implementation Binary Sorted Tree
*	AUTHOR 			Liad Raz
*
*******************************************************************************/

#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */
#include <stdio.h>			/* printf */ /* TODO Test mode */


#include "utilities.h"
#include "binary_tree.h"


typedef enum side {LEFT = 0, RIGHT = 1} side_ty;


struct node
{
	node_ty *children[2];
	node_ty *parent;
	void *data;
};

struct bst
{
	node_ty *dummy;
	cmp_func_ty CmpFunc;
	void *param;
};

/* returns type of FindPlaceIMP */
typedef struct parent_and_side
{
	node_ty *parent;
	side_ty side;
	
	DEBUG_MODE(int cmp_return;)
	
}parent_and_side_ty;

/******************************************************************************/
			
/******************************************************************************/
static node_ty *CreateChildIMP(void *data);
static parent_and_side_ty FindPlaceIMP(bst_ty *th_, void *new_data_);
static side_ty SideToFollowIMP(int compare_return_); 

static void ConnectParentChildIMP(node_ty *parent_, node_ty *child_, side_ty side);
static void RemoveNode(node_ty *to_remove_);
static void RemoveStruct(bst_ty *to_remove_);

static bst_itr_ty PrevNextIMP(bst_itr_ty where_, side_ty side_);

static int IsSideIMP(node_ty *curr_, side_ty side_); /* TODO WhichSideMyChildMP same*/
static int HasChildIMP(node_ty *curr_);
static int HasChildrenIMP(node_ty *curr_);

static side_ty WhichSideIAmIMP(node_ty *curr_);
static side_ty WhichSideMyChildMP(node_ty *child_);
static node_ty *GoSideMostChildIMP(node_ty *curr_, side_ty side_);
static node_ty *GoSideMostAncesIMP(node_ty *curr_, side_ty side_);

DEBUG_MODE(static int IsDummy(bst_itr_ty itr);)
static node_ty *UnWrapItrIMP(bst_itr_ty itr_);
static bst_itr_ty WrapItrIMP(node_ty *node_ DEBUG_MODE (COMMA bst_ty *th_));


/*******************************************************************************
******************************** BSTCreate ************************************/
bst_ty *BSTCreate(cmp_func_ty CmpFunc_, void *param_)
{
	bst_ty *tree = NULL;
	node_ty *dummy = NULL;
	
	ASSERT_NOT_NULL(CmpFunc_, "BSTCreate: Function is not valid");
	
	/* Allocate memory for BST struct */
	tree = (bst_ty *)malloc(sizeof(bst_ty));
	
	if (NULL == tree)
	{
		return NULL;
	}
	
	/* Allocate memory for dummy node */
	dummy = (node_ty *)malloc(sizeof(node_ty));
	
	if (NULL == dummy)
	{
		free(tree);
		return NULL;
	}
	
	/* Initiate binary tree struct fields */
	tree->dummy = dummy;
	tree->CmpFunc = CmpFunc_;
	tree->param = param_;
	
	/* Initiate dummy node fields */
	dummy->children[LEFT] = NULL;
	dummy->children[RIGHT] = DEAD_MEM(node_ty *);
	dummy->parent = DEAD_MEM(node_ty *);
	dummy->data = DEAD_MEM(void *);
	
	return tree;
}


/*******************************************************************************
******************************** BTSDestroy ***********************************/
void BTSDestroy(bst_ty *th_)
{
	node_ty *to_free = NULL;	
	node_ty *sub_tree_root = NULL;	
	
	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	to_free = th_->dummy;
	
	/* traverse until list is empty */
	while (!BSTIsEmpty(th_))
	{
		/* Start in the leftmost node */
		to_free = GoSideMostChildIMP(to_free, LEFT);
		/* update the current root subtree we deal with */
		sub_tree_root = to_free->parent;
		
		/* In case, node is a single leaf */
		if (NULL == to_free->children[RIGHT])
		{
			/* Update node's parent to NULL */
			sub_tree_root->children[WhichSideIAmIMP(to_free)] = NULL;
			
			/* Remove node and break its fields */
			RemoveNode(to_free);			
			
			/* Bring up to_free to keep looking for right nodes */
			to_free = sub_tree_root;
		}
		/* In case, sub_tree has a right node */
		else
		{
			/* Jump to the right node */
			to_free = to_free->children[RIGHT];

			continue;
		}
	}
	
	/* free both dummy and BST struct */
	RemoveNode(th_->dummy);
	RemoveStruct(th_);
}

/*******************************************************************************
******************************** BSTInsert ************************************/
bst_itr_ty BSTInsert(bst_ty *th_, void *new_data_)
{
	node_ty *new_child = NULL;
	parent_and_side_ty parent_side = {NULL};

	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	/* Create new node, and insert the provided data to it */
	new_child = CreateChildIMP(new_data_);
	
	/* In case, creation failed return invalid iterator */
	if (NULL == new_child)
	{
		return BSTEnd(th_);
	}
	
	/* Get the parent and side which will be the location to insert node */
	parent_side = FindPlaceIMP(th_, new_data_);
	
	assert (0 != parent_side.cmp_return 
	&& "BSTInsert: Data already exists in BST");
	
	/* Connect and Update new child and where's parent */
	ConnectParentChildIMP(parent_side.parent, new_child, parent_side.side);
	
	/* return iterator to the new child */
	return WrapItrIMP(new_child DEBUG_MODE (COMMA th_));
}


/*******************************************************************************
****************************** BSTRemove **************************************/
bst_itr_ty BSTRemove(bst_itr_ty to_remove_)
{
	node_ty *to_remove = NULL;
	node_ty *next = NULL;
	side_ty parent_side = LEFT;
	side_ty child_side = LEFT;

	ASSERT_NOT_NULL(to_remove_.tree, "BSTRemove: BST is invalid");
	assert (!IsDummy(to_remove_) 
	&& "BSTRemove: Cannot remove an invalid iterator");
	
	to_remove = to_remove_.node;
	
	/* find in which side I am to my parent */
	parent_side = WhichSideIAmIMP(to_remove);

	/* get the next node of to_remove node */
	next = UnWrapItrIMP(BSTNext(to_remove_));

	/* In case, node has TWO children */
	if (HasChildrenIMP(to_remove))
	{
		/* Right side =>
		Connect and Update to_remove parent with to_remove right child */
		ConnectParentChildIMP(to_remove->parent, to_remove->children[RIGHT], parent_side);

		/* Left side =>
		Connect and Update to_remove LEFT child with next node */
		ConnectParentChildIMP(next, to_remove->children[LEFT], LEFT);
	}
	/* In case, node has at least ONE children */
	else if (HasChildIMP(to_remove))
	{
		/* find in which side my child is to remove node */
		child_side = WhichSideMyChildMP(to_remove);

		/* Connect and Update to_remove node parent with its only child */
		ConnectParentChildIMP(to_remove->parent, to_remove->children[child_side], parent_side);
	}
	else
	{
		/* update to_remove_ parent to point NULL */
		to_remove->parent->children[parent_side] = NULL;
	}
	
	/* free to_remove node, update its parent and break its fields */
	RemoveNode(to_remove);
	
	/* return iterator to the next removed node */
	return WrapItrIMP(next DEBUG_MODE (COMMA to_remove_.tree));
}


/*******************************************************************************
******************************** BSTSize **************************************/
size_t BSTSize(const bst_ty *th_)
{
	bst_itr_ty runner = {NULL};
	bst_itr_ty end = {NULL};
	size_t counter = 0;
	
	ASSERT_NOT_NULL(th_, "BSTSize: BST is invalid");
	
	runner = BSTBegin((bst_ty *)th_);
	end = BSTEnd((bst_ty *)th_);
	
	/* traverse from begin to end */
	while (!BSTIterIsEqual(runner, end))
	{
		/* jump to the next node */
		runner = BSTNext(runner);
				
		/* Count each node encountered */
		++counter;
	}
	
	return counter;	
}


/*******************************************************************************
****************************** BSTIsEmpty *************************************/
int BSTIsEmpty(const bst_ty *th_)
{
	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	return (NULL == th_->dummy->children[LEFT]);
}


/*******************************************************************************
****************************** BSTBegin ***************************************/
bst_itr_ty BSTBegin(bst_ty *th_)
{
	node_ty *begin = NULL;
	
	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	begin = th_->dummy;
	
	/* Go to the leftmost node */
	begin = GoSideMostChildIMP(begin, LEFT);
	
	return WrapItrIMP(begin DEBUG_MODE (COMMA th_));
}


/*******************************************************************************
****************************** BSTEnd *****************************************/
bst_itr_ty BSTEnd(bst_ty *th_)
{
	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	return WrapItrIMP(th_->dummy DEBUG_MODE (COMMA th_));
}


/*******************************************************************************
******************************** BSTPrev **************************************/
bst_itr_ty BSTPrev(bst_itr_ty where_)
{
	ASSERT_NOT_NULL(where_.tree, "BSTPrev: BST is invalid");
	assert (!BSTIterIsEqual(where_, BSTBegin(where_.tree)) 
	&& "BSTPrev: Cannot prev iterator to begin");
	
	return PrevNextIMP(where_, LEFT);
}


/*******************************************************************************
******************************** BSTNext **************************************/
bst_itr_ty BSTNext(bst_itr_ty where_)
{

	ASSERT_NOT_NULL(where_.tree, "BSTNext: BST is invalid");
	assert (!IsDummy(where_) 
	&& "BSTNext: Cannot next iterator to 1");

	return PrevNextIMP(where_, RIGHT);
}


/*******************************************************************************
***************************** BSTIterIsEqual **********************************/
int BSTIterIsEqual(bst_itr_ty itr1, bst_itr_ty itr2)
{
	return (itr1.node == itr2.node);
}


/*******************************************************************************
***************************** BSTGetData **************************************/
void *BSTGetData(bst_itr_ty where_)
{
	ASSERT_NOT_NULL(where_.tree, 
	"BSTGetData: Cannot getdata from Iterator is invalid");
	
	return where_.node->data;
}


/*******************************************************************************
****************************** BSTFind ****************************************/
bst_itr_ty BSTFind(bst_ty *th_, const void *data_)
{
	parent_and_side_ty parent_pack = {NULL};
	node_ty *ret = {NULL};
	
	ASSERT_NOT_NULL(th_, "BSTInsert: BST is invalid");
	
	/* Get the parent and side which will be the searched node */
	parent_pack = FindPlaceIMP(th_, (void *)data_);
	ret = parent_pack.parent->children[parent_pack.side];
	
	/* In case, data has not found, return invalide iterator */
	if (ret == NULL)
	{
		return BSTEnd(th_);
	}

	return WrapItrIMP(ret DEBUG_MODE (COMMA th_));
} 


	/* TODO seperate to another file TODO */
/*******************************************************************************
****************************** BSTForEach *************************************/
int BSTForEach(bst_itr_ty from_, bst_itr_ty to_, exe_func_ty ExeFunc_, void *param_)
{
	int exe_ret = 0;
	bst_itr_ty runner = {NULL};
	
	ASSERT_NOT_NULL(from_.node, "BSTForEach: iterator is invalid");
	ASSERT_NOT_NULL(ExeFunc_, "BSTForEach: Function is not valid");
	assert (from_.tree == to_.tree 
	&& "BSTForEach: iterators come from different trees");
	
	runner = from_;
	
	while (!BSTIterIsEqual(runner, to_))	
	{
		exe_ret = ExeFunc_(BSTGetData(runner), param_);
		
		/* if Exe Fucntion failed stop the loop and return zero */
		if (!exe_ret)
		{
			return exe_ret;
		}
		
		runner = BSTNext(runner);
	}
	
	return exe_ret;
}


/*******************************************************************************
******************************** Side Functions *******************************/
static node_ty *CreateChildIMP(void *data_)
{
	node_ty *new_node = NULL;
	
	/* Allocate memory for a new node */
	new_node = (node_ty *)malloc(sizeof(node_ty));
	
	if (NULL == new_node)
	{
		return NULL;
	}
	
	/* Initiate new_node fields */
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	new_node->parent = NULL;
	new_node->data = data_;
	
	return new_node;
}


static parent_and_side_ty FindPlaceIMP(bst_ty *th_, void *data_)
{
	node_ty *where = NULL;
	parent_and_side_ty ret= {NULL};
	int cmp_ret = 0;
	
	/* Assign data to parent_side fields */
	ret.parent = th_->dummy;
	ret.side = LEFT;
	DEBUG_MODE(ret.cmp_return = -1);

	/* Start looking from the tree's root */
	where = th_->dummy->children[LEFT];
	
	/* traverse the tree until where pointer gets to null */
	while (NULL != where)
	{
		/* compare new_data with the current node */
		cmp_ret = th_->CmpFunc(where->data , data_, th_->param);
		
		/* In case, data already exists */
		if (cmp_ret == 0)
		{
			DEBUG_MODE(ret.cmp_return = cmp_ret);
			break;
		}
		
		/* Update parent_side fields */
		ret.parent = where;
		ret.side = SideToFollowIMP(cmp_ret);
		
		where = ret.parent->children[ret.side];
	}
	 
	return ret;
}


static side_ty SideToFollowIMP(int compare_return_)
{
	/* In case, data's node is smaller than data return LEFT
		Otherwise return RIGHT */
	return (compare_return_ < 0) ? LEFT : RIGHT;
}


static void ConnectParentChildIMP(node_ty *parent_, node_ty *child_, side_ty side)
{
	/* update parent's children field */
	parent_->children[side] = child_;
	
	/* update child's parent field */
	child_->parent = parent_;
}


static void RemoveNode(node_ty *to_remove_)
{
	DEBUG_MODE (
	to_remove_->children[RIGHT] = DEAD_MEM(node_ty *);
	to_remove_->children[LEFT] = DEAD_MEM(node_ty *);
	to_remove_->parent = DEAD_MEM(node_ty *);
	to_remove_->data = DEAD_MEM(void *);
	);
	
	free(to_remove_);
}


static void RemoveStruct(bst_ty *to_remove_)
{
	DEBUG_MODE (
	to_remove_->dummy = DEAD_MEM(node_ty *);
	to_remove_->param = DEAD_MEM(void *);
	);
	
	free(to_remove_);
}


static bst_itr_ty PrevNextIMP(bst_itr_ty where_, side_ty side_)
{
	/* Unwrap iterator to currernt node */
	node_ty *curr = where_.node;
	
	/* In case, node has NEXT-right / PREV-left child */
	if (IsSideIMP(curr, side_))
	{
		/* go to the NEXT-right / PREV-left child
		and NEXT-leftmost / PREV-rightmost child */
		curr = GoSideMostChildIMP(curr->children[side_], !side_);
	}
	/* In case, node has NO NEXT-right / PREV-left child */
	else
	{
		/* go to the NEXT-leftmost / PREV-rightmost parent, 
		and return its NEXT-right / PREV-left parent */
		curr = GoSideMostAncesIMP(curr, !side_);
	}
	
	return WrapItrIMP(curr DEBUG_MODE (COMMA where_.tree));
}


/* Checks if child exists in provided side */
static int IsSideIMP(node_ty *curr_, side_ty side_)
{
	return (NULL != curr_->children[side_]);
}


/* Checks if node has at least one child */
static int HasChildIMP(node_ty *curr_)
{
	return ((IsSideIMP(curr_, LEFT)) || (IsSideIMP(curr_, RIGHT)));
}


/* Checks if node has two children exists in side_ */
static int HasChildrenIMP(node_ty *curr_)
{
	return (NULL != curr_->children[RIGHT] && NULL != curr_->children[LEFT]);
}


/* return in which side the child is, to the provided parent */
static side_ty WhichSideIAmIMP(node_ty *curr_)
{
	return (curr_ == curr_->parent->children[RIGHT]);
}


/* In case I have one child, return which side it is */
static side_ty WhichSideMyChildMP(node_ty *child_)
{
	return (NULL == child_->children[LEFT]);
}


static node_ty *GoSideMostChildIMP(node_ty *curr_, side_ty side_)
{
	while (IsSideIMP(curr_, side_))
	{
		curr_ = curr_->children[side_];
	}
	
	return curr_;
}


static node_ty *GoSideMostAncesIMP(node_ty *curr_, side_ty child_side_)
{
	/* rnu until I am standing at the correct side */
	while (child_side_ != WhichSideIAmIMP(curr_))
	{
		curr_ = curr_->parent;
	}
	return curr_->parent;
}


/* Works only in Debug Mode */
DEBUG_MODE(
static int IsDummy(bst_itr_ty itr_)
{
	return BSTIterIsEqual(BSTEnd(itr_.tree), itr_);
}
)

static bst_itr_ty WrapItrIMP(node_ty *node_ DEBUG_MODE (COMMA bst_ty *th_))
{
	bst_itr_ty itr = {NULL};

	/* Inititate iterator fields */
	itr.node = node_;
	
	DEBUG_MODE(itr.tree = th_);
	
	return itr;
}

static node_ty *UnWrapItrIMP(bst_itr_ty itr_)
{
	return itr_.node;
}

/*******************************************************************************
******************************** Unit Tests ***********************************/

/* CREDIT TO SECOND ANSWER HERE: https://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console */
/* SOME CHANGES ADDED BY YANAL */

int PrintTreeRecursive(node_ty *node, int is_left, int offset, int depth, char s[20][255])
{
	char b[20];
    int width = 5;
	int i = 0;
	int left = 0;
	int right = 0;

    if (!node) return 0;

	/* Works only if you inserted (void*)1, (void*)2, etc. */
/*    sprintf(b, "(%s)", (char *)(node->data));*/
    sprintf(b, "(%03d)", *(int *)(node->data));
	/* If you use pointers to integers, then use the following line: */
    /* sprintf(b, "(%03d)", *((int*)node->data)); */

    left  = PrintTreeRecursive(node->children[LEFT],  1, offset, depth + 1, s);
    right = PrintTreeRecursive(node->children[RIGHT], 0, offset + left + width, depth + 1, s);

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

void PrintTree(bst_ty *tree)
{
	int i;
    char s[20][255];

    for (i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    PrintTreeRecursive(tree->dummy->children[LEFT], 0, 0, 0, s);

    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}



