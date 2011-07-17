#include "avlTree.h"

#define INSERT 1
#define DELETE 2

avlNode* allocAvlNode (avlNodeData *val)
{
    avlNode *node;

    node = (avlNode*)malloc(sizeof(avlNode));
    node->data = val;
    node->data->rc = 1;
    node->height = 0;
    node->lChild = NULL;
    node->rChild = NULL;
    node->parent = NULL;
    return node;
}

avlNode* newAvlTree (avlNodeData *val)
{
    avlNode *root = NULL;

    root = allocAvlNode(val);
    return root;
}

void freeNodeData (avlNodeData **data);
void freeAvlNode (avlNode **node, void (*freeNodeData)(avlNodeData **data))
{
    if ((*node)->data->rc <= 1)
	freeNodeData(&((*node)->data));
    else
	--((*node)->data->rc);
    free(*node);
    *node = NULL;
    return;
}

int getHeight (avlNode *node)
{
    int h_lChild = (node->lChild) ?  node->lChild->height + 1: 0;
    int h_rChild = (node->rChild) ?  node->rChild->height + 1: 0;

    return ((h_lChild >= h_rChild) ? h_lChild : h_rChild);
}

void leftRotate (avlNode **node)
{
    avlNode *father = (*node)->parent;
    avlNode *rChild = (*node)->rChild;
    avlNode *grandSon = rChild->lChild;
    avlNode *tmp = *node;

    if (father) {
	if (father->lChild == *node)
	    father->lChild = rChild;
	else
	    father->rChild = rChild;
	rChild->parent = father;/**node gets changed at this point, because of the recursive nature of avlTree_fixup*/
    } else
	rChild->parent = NULL;
    rChild->lChild = tmp;
    tmp->rChild = grandSon;
    if (grandSon != NULL)
	grandSon->parent = tmp;
    tmp->parent = rChild;
    /*update heights*/
    tmp->height = getHeight(tmp);
    rChild->height = getHeight(rChild);

    return;
}

void rightRotate (avlNode **node)
{
    avlNode *father = (*node)->parent;
    avlNode *lChild = (*node)->lChild;
    avlNode *grandSon = lChild->rChild;
    avlNode *tmp = *node;

    if (father) {
	if (father->lChild == *node)
	    father->lChild = lChild;
	else
	    father->rChild = lChild;
	lChild->parent = father;
    } else
	lChild->parent = NULL;
    lChild->rChild = tmp;
    tmp->lChild = grandSon;
    if (grandSon != NULL)
	grandSon->parent = *node;
    tmp->parent = lChild;
    /*update heights*/
    tmp->height = getHeight(tmp);
    lChild->height = getHeight(lChild);

    return;
}

inline int balanceFactor (avlNode *node)
{
    int h_lChild = (node->lChild) ?  node->lChild->height + 1: 0;
    int h_rChild = (node->rChild) ?  node->rChild->height + 1: 0;

    return h_rChild - h_lChild;
}

avlNode* getRoot(avlNode *node)
{
    if (node->parent)
	return getRoot(node->parent);
    else
	return node;
}

void avlTree_fixup (avlNode **node, int action)
{
    int bf = 0;
    int ht = -1;
    avlNode *father = NULL;

    if (*node == NULL)
	return;
    ht = getHeight(*node);
    (*node)->height = ht;
    bf = balanceFactor(*node);
    if (bf == 0 && action == 1) /*for the case of insertion - no change in height*/
	return;
    if (bf == 0 && action == 2) {/*for the case of deletion*/
	avlTree_fixup(&((*node)->parent), action);
	return;
    }
    if (bf == 1 || bf == -1) {/*=> that one node has been added to the right/left subtree, can disturb the balancing at the parent*/
	if (action == 1) {
	    avlTree_fixup(&((*node)->parent), action);
	    return;
	}
	else /*no change in height*/
	    return;
    }
    if (bf == 2) {
	int bf_rChild = balanceFactor((*node)->rChild);
	if (bf_rChild == 1) {
	    avlNode *tmp = (*node)->rChild;
	    leftRotate(node);
	    /*node is now by itself set to parent of node before
	     * rotation*/
	    if (action == 1)
		avlTree_fixup(node, action);
	    else
		avlTree_fixup(&((*node)->parent), action);
	    return;
	} else if (bf_rChild == -1) {
	    rightRotate(&((*node)->rChild));
	    (*node)->height = getHeight(*node);
	    if (action == 1)
		leftRotate(&((*node)->parent));
	    else
		leftRotate(node);
	    avlTree_fixup(&((*node)->parent), action);
	    return;
	} else if (bf_rChild == 0) {/*for the case of deletion*/
	    leftRotate(node);
	    avlTree_fixup(&((*node)->parent), action);
	    return;
	}
    } else if (bf == -2) {
	int bf_lChild = balanceFactor((*node)->lChild);
	if (bf_lChild == -1) {
	    rightRotate(node);
	    /*node is now by itself set to parent of node before
	     * rotation*/
	    if (action == 1)
		avlTree_fixup(node, action);
	    else
		avlTree_fixup(&((*node)->parent), action);
	    return;
	} else if (bf_lChild == 1) {
	    leftRotate(&((*node)->lChild));
	    (*node)->height = getHeight(*node);
	    if (action == 1)
		rightRotate(&((*node)->parent));
	    else
		rightRotate(node);
	    avlTree_fixup(&((*node)->parent), action);
	    return;
	} else if (bf_lChild == 0) {/*for the case of deletion*/
	    rightRotate(node);
	    avlTree_fixup(&((*node)->parent), action);
	    return;
	}
    }
}

/*cmp is the function pointer provided by the user
 * return codes of cmp expected by the insert function
 * key1 = key2 return 0
 * key1 < key2 return 1
 * key1 > key2 return 2
 * */
void add (avlNode **root, avlNodeData *val, int (*cmp)(void *key1, void *key2))
{
    avlNode *node = NULL;
    avlNode *tmp = NULL;
    int dir = 0;

    if (!(*root)) {
	*root = newAvlTree(val);
	(*root)->parent = NULL;
	//return *root;
	return;
    }
    if (cmp(val->key, (*root)->data->key) <= 1) {
	tmp = (*root)->lChild;
	dir = 1;
    } else {
	tmp = (*root)->rChild;
	dir = 2;
    }
    if (tmp == NULL) {
	tmp = allocAvlNode(val);
	tmp->parent = *root;
	if (dir == 1) {
	    (*root)->lChild = tmp;
	    avlTree_fixup(root, INSERT);
	} else {
	    (*root)->rChild = tmp;
	    avlTree_fixup(root, INSERT);
	}
    } else
	add(&tmp, val, cmp);

    //return ((*root)->parent) ? (*root)->parent : *root;
    return;
}

avlNode* insert (avlNode **root, avlNodeData *val, int (*cmp)(void *key1, void *key2))
{
    add(root, val, cmp);
    return getRoot(*root);
}

avlNode* search (avlNode *root, avlNodeData *val, int (*cmp)(void *key1, void *key2))
{
    if (root == NULL)
	return NULL;
    if (cmp(val->key, root->data->key) == 0)
	return root;
    else if (cmp(val->key, root->data->key) == 1)
	return search(root->lChild, val, cmp);
    else
	return search(root->rChild, val, cmp);
}

void replace (avlNode **src, avlNode **dest)
{
    avlNode *father = (*src)->parent;

    if (*dest != NULL)
	(*dest)->parent = (*src)->parent;
    if (father->lChild == *src)
	father->lChild = *dest;
    else
	father->rChild = *dest;

    return;
}

void removeNode (avlNode **node, void (*freeNodedata)(avlNodeData **data))
{
    avlNode *child = NULL;
    avlNode *father = NULL;

    father = (*node)->parent;
    if ((*node)->lChild != NULL)
	child = (*node)->lChild;
    else if ((*node)->rChild != NULL)
	child = (*node)->rChild;
    else
	child = NULL;

    replace(node, &child);
    avlTree_fixup(&((*node)->parent), DELETE);
    freeAvlNode(node, freeNodedata);

    return;
}

avlNode* largestInSubtree (avlNode *root)
{
    if (!(root->rChild))
	return root;
    return largestInSubtree(root->rChild);
}

avlNode* smallestInSubtree (avlNode *root)
{
    if (!(root->lChild))
	return root;
    return smallestInSubtree(root->lChild);
}

avlNode* deleteNode (avlNode **root, avlNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(avlNodeData **data))
{
    avlNode *target = NULL;
    avlNode *toRemove = NULL;

    target = search(*root, val, cmp);
    if (!target)
	return;
    if (target->lChild != NULL)
	toRemove = largestInSubtree(target->lChild);
    else if (target->rChild != NULL)
	toRemove = smallestInSubtree(target->rChild);
    else
	toRemove = target;
    if (target != toRemove) {
	++(toRemove->data->rc);
	target->data = toRemove->data;
    }
    removeNode(&toRemove, freeNodeData);

    return getRoot(*root);;
}


avlNode* locate (avlNode *root, avlNodeData *val, int (*cmp)(void *key1, void *key2))
{
    return search(root, val, cmp);
}

void inorder (avlNode *root, void (*display)(avlNodeData *nodeData))
{
    if (root == NULL)
	return;

    inorder(root->lChild, display);
    display(root->data);
    inorder(root->rChild, display);
    return;
}

void postorder (avlNode *root, void (*display)(avlNodeData *nodeData))
{
    if (root == NULL)
	return;

    postorder(root->lChild, display);
    postorder(root->rChild, display);
    display(root->data);
    return;
}

void preorder (avlNode *root, void (*display)(avlNodeData *nodeData))
{
    if (root == NULL)
	return;

    display(root->data);
    preorder(root->lChild, display);
    preorder(root->rChild, display);
    return;
}

