#ifndef _LIST_H
#define _LIST_H
/*a generic circular doubly linked list implementation*/
#include <stdio.h>
#include <stdlib.h>

struct _listNode {
	void *data;
	struct _listNode *next;
	struct _listNode *prev;
};
typedef struct _listNode ListNode;

struct _list {
	ListNode *head;
	ListNode *curr;
	ListNode *tail;
	int size;
};
typedef struct _list List;

#ifndef _DESTRUCTOR_
#define _DESTRUCTOR_
typedef void (*Destructor)(void*);
#endif

ListNode* allocListNode (void);
void freeListNode (ListNode *node, Destructor);
ListNode* makeNode (void *data);
List* allocList (void);
void freeList (List *list, Destructor);
void insertAtListHead (List *list, ListNode *node);
void insertAtListTail (List *list, ListNode *node);
ListNode* deleteFromListHead (List *list, Destructor);
ListNode* deleteFromListTail (List *list, Destructor);
void deleteFromList (List *list, ListNode *node, Destructor);
#endif

