#include "splayTree.h"

#define INSERT 1
#define DELETE 2
#define LOCATE 3

splayNode* allocSplayNode (splayNodeData *val)
{
    splayNode *node;

    node = (splayNode*)malloc(sizeof(splayNode));
    node->data = val;
    node->data->rc = 1;
    node->lChild = NULL;
    node->rChild = NULL;
    node->parent = NULL;
    return node;
}

splayNode* newSplayTree (splayNodeData *val)
{
    splayNode *root = NULL;

    root = allocSplayNode(val);
    return root;
}

void freeNodeData (splayNodeData **data);
void freeSplayNode (splayNode **node, void (*freeNodeData)(splayNodeData **data))
{
    if ((*node)->data->rc <= 1)
	freeNodeData(&((*node)->data));
    else
	--((*node)->data->rc);
    free(*node);
    *node = NULL;
    return;
}

void leftRotate (splayNode **node)
{
    splayNode *father = (*node)->parent;
    splayNode *rChild = (*node)->rChild;
    splayNode *grandSon = rChild->lChild;
    splayNode *tmp = *node;

    if (father) {
	if (father->lChild == *node)
	    father->lChild = rChild;
	else
	    father->rChild = rChild;
	rChild->parent = father;/**node gets changed at this point, because of the recursive nature of splayTree_fixup*/
    } else
	rChild->parent = NULL;
    rChild->lChild = tmp;
    tmp->rChild = grandSon;
    if (grandSon != NULL)
	grandSon->parent = tmp;
    tmp->parent = rChild;

    return;
}

void rightRotate (splayNode **node)
{
    splayNode *father = (*node)->parent;
    splayNode *lChild = (*node)->lChild;
    splayNode *grandSon = lChild->rChild;
    splayNode *tmp = *node;

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

    return;
}

void zig (splayNode **node, int dir)
{
    if (dir == 1)
	rightRotate(&((*node)->parent));
    else
	leftRotate(&((*node)->parent));

    return;
}

void zigZig (splayNode **node, int dir)
{
    splayNode *father = (*node)->parent;
    splayNode *grandFather = father->parent;

    if (dir == 1) {
	rightRotate(&grandFather);
	rightRotate(&father);
    } else {
	leftRotate(&grandFather);
	leftRotate(&father);
    }

    return;
}

void zigZag (splayNode **node, int dirNode, int dirFather)
{
    splayNode *father = (*node)->parent;
    splayNode *grandFather = father->parent;

    if (dirFather == 1 && dirNode == 2) {
	leftRotate(&father);
	rightRotate(&grandFather);
    } else if (dirFather == 2 && dirNode == 1) {
	rightRotate(&father);
	leftRotate(&grandFather);
    }

    return;
}

void splayTree_fixup (splayNode **node, int action)
{
    splayNode *father = NULL;
    splayNode *grandFather = NULL;
    splayNode *tmp = *node;
    int dir_node = 0;
    int dir_father = 0;

    if (*node == NULL)
	return;
    father = (*node)->parent;
    if (!father)
	return;
    dir_node = (father->lChild == *node) ? 1 : 2;
    grandFather = father->parent;
    if (!grandFather) {
	zig(node, dir_node);
	/*zig is the last step in splay operation, so no recursive call
	 * here*/
	return;
    }
    dir_father = (grandFather->lChild == father) ? 1 : 2;
    if (dir_node == dir_father) {
	zigZig(node, dir_node);
	if (action == 2)
	    splayTree_fixup(&tmp, action);
	else
	    splayTree_fixup(node, action);
    } else {
	zigZag(node, dir_node, dir_father);
	if (action == 2)
	    splayTree_fixup(&tmp, action);
	else
	    splayTree_fixup(node, action);
    }

    return;
}

splayNode* getRoot(splayNode *node)
{
    if (node->parent)
	return getRoot(node->parent);
    else
	return node;
}

/*cmp is the function pointer provided by the user
 * return codes of cmp expected by the insert function
 * key1 = key2 return 0
 * key1 < key2 return 1
 * key1 > key2 return 2
 * */
void add (splayNode **root, splayNodeData *val, int (*cmp)(void *key1, void *key2))
{
    splayNode *node = NULL;
    splayNode *tmp = NULL;
    int dir = 0;

    if (!(*root)) {
	*root = newSplayTree(val);
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
	tmp = allocSplayNode(val);
	tmp->parent = *root;
	if (dir == 1) {
	    (*root)->lChild = tmp;
	    splayTree_fixup(&tmp, INSERT);
	} else {
	    (*root)->rChild = tmp;
	    splayTree_fixup(&tmp, INSERT);
	}
    } else
	add(&tmp, val, cmp);

    return;
}

splayNode* insert (splayNode **root, splayNodeData *val, int (*cmp)(void *key1, void *key2))
{
    add(root, val, cmp);
    return getRoot(*root);
}

splayNode* search (splayNode *root, splayNodeData *val, int (*cmp)(void *key1, void *key2))
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

void replace (splayNode **src, splayNode **dest)
{
    splayNode *father = (*src)->parent;

    if (*dest != NULL)
	(*dest)->parent = (*src)->parent;
    if (father->lChild == *src)
	father->lChild = *dest;
    else
	father->rChild = *dest;

    return;
}

splayNode* largestInSubtree (splayNode *root)
{
    if (!(root->rChild))
	return root;
    return largestInSubtree(root->rChild);
}

splayNode* smallestInSubtree (splayNode *root)
{
    if (!(root->lChild))
	return root;
    return smallestInSubtree(root->lChild);
}

void removeNode (splayNode **node, void (*freeNodedata)(splayNodeData **data))
{
    splayNode *target = *node;
    splayNode *toRemove = NULL;
    splayNode *child = NULL;

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
    if (toRemove->lChild != NULL)
	child = toRemove->lChild;
    else if (toRemove->rChild != NULL)
	child = toRemove->rChild;
    else
	child = NULL;
    replace(&toRemove, &child);
    splayTree_fixup(&(target->parent), DELETE);
    freeSplayNode(&toRemove, freeNodedata);

    return;
}

splayNode* deleteNode (splayNode **root, splayNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(splayNodeData **data))
{
    splayNode *target = NULL;
    splayNode *toRemove = NULL;

    target = search(*root, val, cmp);
    if (!target)
	return;
    removeNode(&target, freeNodeData);

    return getRoot(*root);;
}

splayNode* locate (splayNode *node, splayNodeData *val, int (*cmp)(void *key1, void *key2))
{
    splayNode *retVal = NULL;
    splayNode *tmp = NULL;

    retVal = search(node, val, cmp);
    splayTree_fixup(&retVal, LOCATE);
    tmp = getRoot(node);

    return retVal;
}

void inorder (splayNode *root, void (*display)(splayNodeData *nodeData))
{
    if (root == NULL)
	return;

    inorder(root->lChild, display);
    display(root->data);
    inorder(root->rChild, display);
    return;
}

void postorder (splayNode *root, void (*display)(splayNodeData *nodeData))
{
    if (root == NULL)
	return;

    postorder(root->lChild, display);
    postorder(root->rChild, display);
    display(root->data);
    return;
}

void preorder (splayNode *root, void (*display)(splayNodeData *nodeData))
{
    if (root == NULL)
	return;

    display(root->data);
    preorder(root->lChild, display);
    preorder(root->rChild, display);
    return;
}

