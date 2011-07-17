#ifndef _LIST_H
#define _LIST_H
/*a generic circular doubly linked list implementation*/
#include <stdio.h>
#include <stdlib.h>
#include "type.h"

struct _listNode {
    void *data;
    void *next;
    void *prev;
};
typedef struct _listNode ListNode;

struct _list {
    void *head;
    void *curr;
    void *tail;
    int size;
};
typedef struct _list List;

#ifndef _DESTRUCTOR_
#define _DESTRUCTOR_
typedef void (*Destructor)(void*);
#endif

ListNode* allocListNode (void);
void freeListNode (ListNode *node, Destructor);
void* makeNode (void *data);
List* allocList (void);
void freeList (List *list);//, Destructor);
void insertAtListHead (List *list, void *node);
void insertAtListTail (List *list, void *node);
void* deleteFromListHead (List *list, short);
void* deleteFromListTail (List *list, short);
void deleteFromList (List *list, void *node, short);

#define LISTNODE_FROM_GENTYPE(x) ((ListNode*)(*((void**)(x))))

#endif

