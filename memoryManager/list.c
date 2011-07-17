#include "list.h"

ListNode* allocListNode (void)
{
    return ((ListNode *)malloc(sizeof(ListNode)));
}

List* allocList (void)
{
    List *retVal = NULL;

    retVal = (List *)malloc(sizeof(List));
    retVal->head = NULL;
    retVal->curr = NULL;
    retVal->tail = NULL;
    retVal->size = 0;
    return retVal;
}

void freeListNode (ListNode *node, Destructor freeData)
{
    if (!freeData)
	return;/*this is necessary to give the functionality to just remove
		 and element from the list*/
    node->next = NULL;
    node->prev = NULL;
    freeData(node->data);
    node->data = NULL;
    free(node);
    node = NULL;
    return;
}

void freeList (List *list)/*, Destructor freeData)*/
{
    ListNode *node;
#ifdef DEBUG
    printf("[%s:%d]Inside %s\n", __FILE__, __LINE__, __func__);
#endif
    if (list == NULL)
	return;

    while (list->size) {
	deleteFromListHead(list, 1);
	/*node = list->head;
	  list->head = (list->head)->next;
	  --(list->size);
	  freeListNode(node, freeData);
	  node = list->head;*/
    }
    list->curr = NULL;
    list->head = NULL;
    free(list);
    return;
}

void _freeListNode (void *mem)
{
    ListNode *node = (ListNode*)mem;
#ifdef DEBUG
    printf("[%s:%d]Inside %s\n", __FILE__, __LINE__, __func__);
#endif
    node->next = NULL;
    node->prev = NULL;
    destroyObject(node->data);

    return;
}

//ListNode* makeNode (void *data)
void* makeNode (void *data)
{
    ListNode *node;
    void *genTypObj = NULL;

    node = allocListNode();
    node->data = data;
    node->next = (void*)node;
    node->prev = (void*)node;
    genTypObj = newObjectType((void*)node, _freeListNode, 1);

    //return node;
    return genTypObj;
}

//void insertAtListHead (List *list, ListNode *node)
void insertAtListHead (List *list, void *node)
{
    ListNode *lNode = LISTNODE_FROM_GENTYPE(node);
    ++(list->size);
    if (list->size == 1) {
	list->head = node;
	list->curr = node;
	list->tail = node;
    } else {
	(LISTNODE_FROM_GENTYPE(list->head))->prev = node;
	lNode->prev = list->tail;
	lNode->next = list->head;
	list->head = node;
	(LISTNODE_FROM_GENTYPE(list->tail))->next = node;
    }
    return;
}

//void insertAtListTail (List *list, ListNode *node)
void insertAtListTail (List *list, void *node)
{
    ListNode *lNode = LISTNODE_FROM_GENTYPE(node);
    ++(list->size);
    if (list->size == 1) {
	list->head = node;
	list->curr = node;
	list->tail = node;
    }
    else {
	(LISTNODE_FROM_GENTYPE(list->tail))->next = node;
	lNode->prev = list->tail;
	list->tail = node;
	lNode->next = list->head;
	(LISTNODE_FROM_GENTYPE(list->head))->prev = node;
    }
    return;
}

//ListNode* deleteFromListHead (List *list, Destructor freeData)
void* deleteFromListHead (List *list, short freeData)
{
    void *err = NULL;

    if (!(list->size))
	return err;
    --list->size;
    err = list->head;
    if (!(list->size)) {
	list->curr = NULL;
	list->head = NULL;
	list->tail = NULL;
    } else {
	if (list->head == list->curr)
	    list->curr = (LISTNODE_FROM_GENTYPE(list->curr))->next;
	list->head = (LISTNODE_FROM_GENTYPE(list->head))->next;
	(LISTNODE_FROM_GENTYPE(list->head))->prev = list->tail;
	(LISTNODE_FROM_GENTYPE(list->tail))->next = list->head;
    }
    if (freeData)
	destroyObject(err);

    return err;
}

//ListNode* deleteFromListTail (List *list, Destructor freeData)
void* deleteFromListTail (List *list, short freeData)
{
    void *err = NULL;

    if (!(list->size))
	return err;
    --(list->size);
    err = list->tail;
    if (!(list->size)) {
	list->curr = NULL;
	list->head = NULL;
	list->tail = NULL;
    } else {
	if (list->curr == list->tail)
	    list->curr = (LISTNODE_FROM_GENTYPE(list->curr))->prev;
	list->tail = (LISTNODE_FROM_GENTYPE(list->tail))->prev;
	(LISTNODE_FROM_GENTYPE(list->tail))->next = list->head;
	(LISTNODE_FROM_GENTYPE(list->head))->prev = list->tail;
    }
    if (freeData)
	destroyObject(err);

    return err;
}

//void deleteFromList (List *list, ListNode *node, Destructor freeData)
void deleteFromList (List *list, void *_node, short freeData)
{
    ListNode *node = LISTNODE_FROM_GENTYPE(_node);
    if (!list || !(list->size))
	return;
    --(list->size);
    if (!(list->size)) {
	list->curr = NULL;
	list->head = NULL;
	list->tail = NULL;
    } else {
	if (list->curr == _node)
	    list->curr = node->next;
	if (list->head == _node)
	    list->head = node->next;
	if (list->tail == _node)
	    list->tail = node->prev;
	(LISTNODE_FROM_GENTYPE(node->prev))->next = node->next;
	(LISTNODE_FROM_GENTYPE(node->next))->prev = node->prev;
    }
    if (freeData)
	destroyObject(_node);
    //freeListNode(node, freeData);

    return;
}
