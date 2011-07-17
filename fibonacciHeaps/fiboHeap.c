#include "fiboHeap.h"

heapNode* allocHeapNode (heapData *data)
{
    heapNode *node = (heapNode *)malloc(sizeof(heapNode));
    node->data = data;
    node->degree = 0;
    node->mark = 0;
    //node->clr = WHITE;
    node->parent = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->child = NULL;

    return node;
}

fiboHeap* allocHeap (heapNode *node)
{
    fiboHeap *heap = NULL;
    heap = (fiboHeap *)malloc(sizeof(fiboHeap));
    node->next = node;
    node->prev = node;
    heap->min = node;
    heap->size = 1;

    return heap;
}

void freeHeap (fiboHeap **heap)
{
    (*heap)->min = NULL;
    free(*heap);
    *heap = NULL;
    return;
}

fiboHeap* insert (fiboHeap *heap, heapData *data, int (*cmp)(heapData *data1, heapData *data2))
{
    heapNode *node = NULL;
    heapNode *next = NULL;

    node = allocHeapNode(data);
    if (!heap)
	return allocHeap(node);
    ++(heap->size);
    next = heap->min->next;
    heap->min->next = node;
    node->next = next;
    node->prev = heap->min;
    next->prev = node;
    if (cmp(node->data, heap->min->data) < 2)
	heap->min = node;

    return heap;
}

heapNode* findMin (fiboHeap *heap)
{
    if (!heap)
	return NULL;
    return heap->min;
}

void addToList (heapNode *node, heapNode *list)
{
    if (!list)
	return;

    heapNode *lastC = list->prev;
    node->next = list;
    list->prev = node;
    if (lastC) {
	lastC->next = node;
	node->prev = lastC;
    } else
	node->prev = list;

    return;
}

void removeFromList (heapNode *toRemove)
{
    heapNode *n = toRemove->next;
    heapNode *p = toRemove->prev;
    p->next = n;
    n->prev = p;

    return;
}

fiboHeap* merge (fiboHeap **heap1, fiboHeap **heap2, int (*cmp)(heapData *, heapData *))
{
    heapNode *next1 = NULL;
    heapNode *prev2 = NULL;
    fiboHeap *retHeap = NULL;

    if (!(*heap1) && !(*heap2))
	return NULL;
    if (!(*heap1))
	return (*heap2);
    if (!(*heap2))
	return (*heap1);

    next1 = (*heap1)->min->next;
    prev2 = (*heap2)->min->prev;
    (*heap1)->min->next = (*heap2)->min;
    (*heap2)->min->prev = (*heap1)->min;
    prev2->next = next1;
    next1->prev = prev2;
    if (cmp((*heap2)->min->data, (*heap1)->min->data) < 2) {
	retHeap = (*heap2);
	retHeap->size += (*heap1)->size;
	freeHeap(heap1);
    } else {
	retHeap = (*heap1);
	retHeap->size += (*heap2)->size;
	freeHeap(heap2);
    }

    return retHeap;
}

int approxLg (int size)
{
    int log_2 = 0;
    float s = size;

    if (s == 1)
	return log_2;
    while (s > 1) {
	++log_2;
	s /= 2;
    }
    return (log_2 * 2);
}

heapNode* mergeDupes (heapNode *node, heapNode **B, int (*cmp)(heapData *, heapData *))
{
    heapNode *firstC = NULL, *lastC = NULL, *n = NULL, *p = NULL, *tmp = NULL;
    heapNode *remove = NULL, *root = NULL;
    heapNode *w = B[node->degree];

    tmp = node;
    root = node;
    while (w != NULL) {
	B[w->degree] = NULL;
	remove = tmp;
	root = w;
	if (cmp(tmp->data, w->data) < 2) {
	    remove = w;
	    root = tmp;
	}
	/*remove 'remove' from root list*/
	removeFromList(remove);
	remove->next = remove;
	remove->prev = remove;
	/*add remove to the children list of root*/
	++(root->degree);
	remove->parent = root;
	addToList(remove, root->child);
	root->child = remove;
	tmp = root;
	w = B[root->degree];
    }
    B[root->degree] = root;
    return root;
}

heapNode* cleanup (heapNode *first, int lg, int (*cmp)(heapData *data1, heapData *data2))
{
    /*dynamically allocate an array of integers of size lg*/
    heapNode **B = NULL, *newmin = first, *nextNode = NULL, *tmp = NULL, *prev = NULL;
    int i;

    B = (heapNode **)malloc(sizeof(heapNode *) * lg);
    for (i = 0; i < lg; i++)
	B[i] = NULL;

    B[first->degree] = first;
    prev = first;
    nextNode = first->next;
    do {
	nextNode->parent = NULL;
	if (cmp(nextNode->data, newmin->data) < 2)
	    newmin = nextNode;
	tmp = mergeDupes(nextNode, B, cmp);
	if (tmp == nextNode && prev == nextNode->prev)
	    nextNode = nextNode->next;
	else {
	    if (tmp == prev)
		nextNode = prev->next;
	    else {
		if (prev == first)
		    first = tmp;
		prev = tmp;
		nextNode = prev->next;
	    }
	}
    } while (nextNode != first);

    return newmin;
}

void freeNode (heapNode **node, void (*freeData)(heapData *))
{
    freeData((*node)->data);
    free(*node);
    *node = NULL;
    return;
}

void deleteMin (fiboHeap *heap, void (*freeData)(heapData *data), int (*cmp)(heapData *, heapData *))
{
    /*Step1 : first splice off the children of node to the root list
     * Step2: cleanup, so that there is only one binomial tree of a
     * particular size*/
    heapNode *firstC;
    heapNode *lastC;

    if (!heap)
	return;
    firstC = heap->min->child;
    lastC = (firstC) ? firstC->next : NULL;
    /*add children to root list*/
    if (heap->min != heap->min->next) {
	if (heap->min->prev) {
	    firstC = (firstC) ? firstC : heap->min->next;
	    (heap->min->prev)->next = firstC;
	}
	if (heap->min->next)
	    (heap->min->next)->prev = (lastC) ? lastC : heap->min->prev;
    }
    freeNode(&(heap->min), freeData);
    --(heap->size);
    if (heap->size)
	heap->min = cleanup(firstC, approxLg(heap->size), cmp);
    else {
	freeHeap(&heap);
	heap->min = NULL;
    }

    return;
}

void promote (heapNode *node, heapNode *rootList)
{
    heapNode *par = node->parent;
    node->mark = 0;
    if (!par)
	return;
    /*remove node from parent's list of children*/
    if (node->next == node) /*this implies that node is the only element in the list of children*/
	(par)->child = NULL;
    else
	removeFromList(par);
    --(par->degree);
    /*insert node into root list*/
    addToList(node, rootList);
    node->parent = NULL;
    if (par->mark)
	promote(par, rootList);
    else
	par->mark = 1;

    return;
}

void decreaseKey (fiboHeap *heap, heapNode *node, heapData *dataNew, int (*cmp)(heapData *, heapData *), void (*freeData)(heapData *))
{
    heapNode *rootList = heap->min;
    freeData(node->data);
    node->data = dataNew;
    if (cmp(node->data, heap->min->data) < 2)
	heap->min = node;
    promote(node, rootList);

    return;
}

void dfs (heapNode *node, void (*dsp)(heapData *))
{
    heapNode *start = node->child, *iter = NULL;
    if (!node)
	return;
    if (start) {
	iter = start;
	do {
	    dfs(iter, dsp);
	    iter = iter->next;
	} while (iter != start); 
    }
    dsp(node->data);
    return;
}

void traverse (fiboHeap *heap, void (*disp)(heapData *))
{
    heapNode *iter = heap->min;
    if (!iter)
	return;
    do {
	dfs(iter, disp);
	printf("\n");
	iter = iter->next;
    } while (iter && iter != heap->min);
    return;
}

heapNode* srch (heapNode *node, heapData *data, int (*cmp)(heapData *, heapData *))
{
    heapNode *retVal = NULL;
    heapNode *start = node->child, *iter = NULL;
    if (!node)
	return NULL;
    if (start) {
	iter = start;
	do {
	    retVal = srch(iter, data, cmp);
	    if (retVal)
		return retVal;
	    iter = iter->next;
	} while (iter != start); 
    }
    if (!cmp(node->data, data))
	return node;
    else
	return NULL;
}

heapNode* lookup (fiboHeap *heap, heapData *data, int (*cmp)(heapData *, heapData *))
{
    heapNode *retVal = NULL;
    heapNode *iter = heap->min;
    if (!iter)
	return;
    do {
	retVal = srch(iter, data, cmp);
	if (retVal)
	    return retVal;
	printf("\n");
	iter = iter->next;
    } while (iter && iter != heap->min);

    return retVal;
}


