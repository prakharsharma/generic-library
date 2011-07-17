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

void freeList (List *list, Destructor freeData)
{
	ListNode *node;

	if (list == NULL)
		return;

	node = list->head;
	while (node) {
		list->head = (list->head)->next;
		freeListNode(node, freeData);
		--(list->size);
		node = list->head;
	}
	list->curr = NULL;
	list->head = NULL;
	free(list);
	return;
}

ListNode* makeNode (void *data)
{
	ListNode *node;

	node = allocListNode();
	node->data = data;
	node->next = node;
	node->prev = node;
	return node;
}

void insertAtListHead (List *list, ListNode *node)
{
	++(list->size);
	if (list->size == 1) {
		list->head = node;
		list->curr = node;
		list->tail = node;
	} else {
		(list->head)->prev = node;
		node->prev = list->tail;
		node->next = list->head;
		list->head = node;
		(list->tail)->next = node;
	}
	return;
}

void insertAtListTail (List *list, ListNode *node)
{
	++(list->size);
	if (list->size == 1) {
		list->head = node;
		list->curr = node;
		list->tail = node;
	}
	else {
		(list->tail)->next = node;
		node->prev = list->tail;
		list->tail = node;
		node->next = list->head;
		(list->head)->prev = node;
	}
	return;
}

ListNode* deleteFromListHead (List *list, Destructor freeData)
{
	void *err = NULL;
	ListNode *node = NULL;

	if (!(list->size))
		return err;
	--list->size;
	node = list->head;
	err = node;
	if (!(list->size)) {
		list->curr = NULL;
		list->head = NULL;
		list->tail = NULL;
	} else {
		if (list->head == list->curr)
			list->curr = (list->curr)->next;
		list->head = (list->head)->next;
		(list->head)->prev = list->tail;
		(list->tail)->next = list->head;
	}
	freeListNode(node, freeData);

	return err;
}

ListNode* deleteFromListTail (List *list, Destructor freeData)
{
	void *err = NULL;
	ListNode *node = NULL;

	if (!(list->size))
		return err;
	--(list->size);
	node = list->tail;
	err = node;
	if (!(list->size)) {
		list->curr = NULL;
		list->head = NULL;
		list->tail = NULL;
	} else {
		if (list->curr == list->tail)
			list->curr = (list->curr)->prev;
		list->tail = (list->tail)->prev;
		(list->tail)->next = list->head;
		(list->head)->prev = list->tail;
	}
	freeListNode(node, freeData);

	return err;
}

void deleteFromList (List *list, ListNode *node, Destructor freeData)
{
	if (!list || !(list->size))
		return;
	--(list->size);
	if (!(list->size)) {
		list->curr = NULL;
		list->head = NULL;
		list->tail = NULL;
	} else {
		if (list->curr == node)
			list->curr = node->next;
		if (list->head == node)
			list->head = node->next;
		if (list->tail == node)
			list->tail = node->prev;
		(node->prev)->next = node->next;
		(node->next)->prev = node->prev;
	}
	freeListNode(node, freeData);
		
	return;
}
