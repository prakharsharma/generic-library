#include "rbTree.h"

rbNodeData sentinelData = {
    .data = NULL,
    .key = NULL,
    .rc = 2,
};
rbNode sentinel = {
    .data = &sentinelData,
    .clr = BLACK,
    .lChild = NULL,
    .rChild = NULL,
    .parent = NULL,
};

rbNode* allocRbNode (rbNodeData *val)
{
    rbNode *node;

    node = (rbNode*)malloc(sizeof(rbNode));
    node->data = val;
    node->data->rc = 1;
    node->clr = NA;
    node->lChild = &sentinel;
    node->rChild = &sentinel;
    node->parent = NULL;
    return node;
}

rbNode* newRbTree (rbNodeData *val)
{
    rbNode *root = NULL;

    root = allocRbNode(val);
    root->clr = BLACK;
    return root;
}

void freeNodeData (rbNodeData **data);
void freeRbNode (rbNode **node, void (*freeNodeData)(rbNodeData **data))
{
    if ((*node)->data->rc <= 1)
	freeNodeData(&((*node)->data));
    else
	--((*node)->data->rc);
    free(*node);
    *node = NULL;
    return;
}

void leftRotate (rbNode **node)
{
    rbNode *father = (*node)->parent;
    rbNode *rChild = (*node)->rChild;
    rbNode *grandSon = rChild->lChild;

    if (father->lChild == *node)
	father->lChild = rChild;
    else
	father->rChild = rChild;
    rChild->parent = father;
    rChild->lChild = *node;
    (*node)->rChild = grandSon;
    if (grandSon != &sentinel || grandSon != NULL)
	grandSon->parent = *node;
    (*node)->parent = rChild;
    return;
}

void rightRotate (rbNode **node)
{
    rbNode *father = (*node)->parent;
    rbNode *lChild = (*node)->lChild;
    rbNode *grandSon = lChild->rChild;

    if (father->lChild == *node)
	father->lChild = lChild;
    else
	father->rChild = lChild;
    lChild->parent = father;
    lChild->rChild = *node;
    (*node)->lChild = grandSon;
    if (grandSon != &sentinel || grandSon != NULL)
	grandSon->parent = *node;
    (*node)->parent = lChild;
    return;
}

void rbTree_insertFixup (rbNode **node)
{
    rbNode *father = (*node)->parent;
    rbNode *grandFather = (father) ? father->parent : NULL;
    rbNode *uncle = NULL;

    if ((*node)->parent == &sentinel) {
	(*node)->clr = BLACK;
	return;
    }
    if (father->clr == BLACK)
	return;
    uncle = (grandFather->lChild == father) ? grandFather->rChild : grandFather->lChild;
    if (father->clr == RED && uncle->clr == RED) {
	grandFather->clr = RED;
	father->clr = BLACK;
	uncle->clr = BLACK;
	rbTree_insertFixup(&grandFather);
    } else if (father->clr == RED && uncle->clr == BLACK) {
	grandFather->clr = RED;
	if (*node == father->lChild) {
	    if (father == grandFather->lChild) {
		father->clr = BLACK;
		rightRotate(&grandFather);
	    } else {
		rightRotate(&father);
		(*node)->clr = BLACK;
		leftRotate(&grandFather);
	    }
	} else {
	    if (father == grandFather->rChild) {
		father->clr = BLACK;
		leftRotate(&grandFather);
	    } else {
		leftRotate(&father);
		(*node)->clr = BLACK;
		rightRotate(&grandFather);
	    }
	}
    }
}

/*cmp is the function pointer provided by the user
 * return codes of cmp expected by the insert function
 * key1 = key2 return 0
 * key1 < key2 return 1
 * key1 > key2 return 2
 * */
void insert (rbNode **root, rbNodeData *val, int (*cmp)(void *key1, void *key2))
{
    rbNode *node = NULL;
    rbNode *tmp = NULL;
    int dir = 0;

    if (!(*root)) {
	*root = newRbTree(val);
	(*root)->parent = &sentinel;/*parent of root is sentinel*/
	return;
    }
    if (cmp(val->key, (*root)->data->key) <= 1) {
	tmp = (*root)->lChild;
	dir = 1;
    } else {
	tmp = (*root)->rChild;
	dir = 2;
    }
    if (tmp == &sentinel) {
	tmp = allocRbNode(val);
	tmp->parent = *root;
	tmp->clr = RED;
	if (dir == 1) {
	    (*root)->lChild = tmp;
	    rbTree_insertFixup(&((*root)->lChild));
	} else {
	    (*root)->rChild = tmp;
	    rbTree_insertFixup(&((*root)->rChild));
	}
    } else
	insert(&tmp, val, cmp);
    return;
}

rbNode* search (rbNode *root, rbNodeData *val, int (*cmp)(void *key1, void *key2))
{
    if (root == &sentinel)
	return NULL;
    if (cmp(val->key, root->data->key) == 0)
	return root;
    else if (cmp(val->key, root->data->key) == 1)
	return search(root->lChild, val, cmp);
    else
	return search(root->rChild, val, cmp);
}

rbNode* findSibling (rbNode *node, rbNode *father)
{
    if (father->lChild == node)
	return father->rChild;
    else
	return father->lChild;
}

void replace (rbNode **src, rbNode **dest)
{
    rbNode *father = (*src)->parent;

    if (*dest != &sentinel)
	(*dest)->parent = (*src)->parent;
    if (father->lChild == *src)
	father->lChild = *dest;
    else
	father->rChild = *dest;

    return;
}

void swapColor (rbNode **node1, rbNode **node2)
{
    nodeClr tmp = (*node1)->clr;
    (*node1)->clr = (*node2)->clr;
    (*node2)->clr = tmp;

    return;
}

void rbTree_removeFixup (rbNode **node, rbNode *father)
{
    //rbNode *father = NULL;
    rbNode *sibling = NULL;

    /*if (*node == &sentinel)
      father = target->parent;
      else
      father = (*node)->parent;*/
    sibling = findSibling(*node, father);
    /*case 1*/
    if (*node != &sentinel && (*node)->parent == &sentinel) /*implies that (*node) is the new root*/
	return;
    /*case 2*/
    if (father->clr == BLACK && sibling->clr == RED) {
	swapColor(&father, &sibling);
	if (sibling == father->rChild)
	    leftRotate(&father);
	else
	    rightRotate(&father);
	sibling = findSibling(*node, father);
    }
    /*case 3*/
    if (father->clr == BLACK && sibling->clr == BLACK && sibling->lChild->clr == BLACK && sibling->rChild->clr == BLACK) {
	sibling->clr = RED;
	rbTree_removeFixup(&father, father->parent);
	return;
    }
    /*case 4*/
    if (father->clr == RED && sibling->clr == BLACK && sibling->lChild->clr == BLACK && sibling->rChild->clr == BLACK) {
	swapColor(&father, &sibling);
	return;
    }
    /*case 5*/
    if (sibling->clr == BLACK && sibling->lChild->clr == RED && sibling->rChild->clr == BLACK) {
	swapColor(&sibling, &(sibling->lChild));
	if (*node == father->lChild)
	    rightRotate(&sibling);
	else
	    leftRotate(&sibling);
	sibling = findSibling(*node, father);
    }
    /*case 6*/
    if (sibling->clr == BLACK && sibling->lChild->clr == BLACK && sibling->rChild->clr == RED) {
	swapColor(&father, &sibling);
	sibling->rChild->clr = BLACK;
	if (*node == father->lChild)
	    leftRotate(&father);
	else
	    rightRotate(&father);
    }
    return;
}

void removeNode (rbNode **node, void (*freeNodedata)(rbNodeData **data))
{
    rbNode *child = NULL;
    rbNode *father = NULL;

    father = (*node)->parent;
    if ((*node)->lChild != &sentinel)
	child = (*node)->lChild;
    else if ((*node)->rChild != &sentinel)
	child = (*node)->rChild;
    else
	child = &sentinel;

    if ((*node)->clr == RED) {
	replace(node, &child);
	freeRbNode(node, freeNodedata);
	return;
    }
    if ((*node)->clr == BLACK && child->clr == RED) {
	child->clr = BLACK;
	replace(node, &child);
	freeRbNode(node, freeNodedata);
	return;
    }

    /*implies that both *node and child are BLACK*/
    replace(node, &child);
    rbTree_removeFixup(&child, (*node)->parent);
    freeRbNode(node, freeNodedata);

    return;
}

rbNode* largestInSubtree (rbNode *root)
{
    if (!(root->rChild) || (root->rChild == &sentinel))
	return root;
    return largestInSubtree(root->rChild);
}

rbNode* smallestInSubtree (rbNode *root)
{
    if (!(root->lChild) || (root->lChild == &sentinel))
	return root;
    return smallestInSubtree(root->lChild);
}

void deleteNode (rbNode **root, rbNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(rbNodeData **data))
{
    rbNode *target = NULL;
    rbNode *toRemove = NULL;

    target = search(*root, val, cmp);
    if (!target)
	return;
    if (target->lChild != &sentinel)
	toRemove = largestInSubtree(target->lChild);
    else if (target->rChild != &sentinel)
	toRemove = smallestInSubtree(target->rChild);
    else
	toRemove = target;
    if (target != toRemove) {
	++(toRemove->data->rc);
	target->data = toRemove->data;
    }
    removeNode(&toRemove, freeNodeData);

    return;
}

void inorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr))
{
    if (root == NULL || root == &sentinel)
	return;

    inorder(root->lChild, display);
    display(root->data, root->clr);
    inorder(root->rChild, display);
    return;
}

void postorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr))
{
    if (root == NULL || root == &sentinel)
	return;

    postorder(root->lChild, display);
    postorder(root->rChild, display);
    display(root->data, root->clr);
    return;
}

void preorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr))
{
    if (root == NULL || root == &sentinel)
	return;

    display(root->data, root->clr);
    preorder(root->lChild, display);
    preorder(root->rChild, display);
    return;
}

