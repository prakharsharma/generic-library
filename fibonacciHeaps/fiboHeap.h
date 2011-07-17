#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#define DFS 1

struct _heapData {
    void *data;
    void *key;
};
typedef struct _heapData heapData;

struct _fiboHeapNode {
    heapData *data;
    int degree;/*number of chlidren of this node*/
    short mark;
    struct _fiboHeapNode *parent;
    struct _fiboHeapNode *next;
    struct _fiboHeapNode *prev;
    struct _fiboHeapNode *child;
};
typedef struct _fiboHeapNode heapNode;

struct _fiboHeap {
    heapNode *min;
    int size;	
};
typedef struct _fiboHeap fiboHeap;

/*cmp(heapData* data1, heapData *data2)
 * return 0 if data1->key = data2->key
 * return 1 if data1->key < data2->key
 * return 2 if data1->key > data2->key*/
fiboHeap* insert (fiboHeap *heap, heapData *data, int (*cmp)(heapData *, heapData *));
heapNode* findMin (fiboHeap *heap);
fiboHeap* merge (fiboHeap **heap1, fiboHeap **heap2, int (*cmp)(heapData *, heapData *));
void deleteMin (fiboHeap *heap, void (*freeData)(heapData *), int (*cmp)(heapData *, heapData *));
void decreaseKey (fiboHeap *heap, heapNode *node, heapData *dataNew, int (*cmp)(heapData *, heapData *), void (*freeData)(heapData *));
void tarverse (fiboHeap *heap, void (*disp)(heapData *));
heapNode* lookup (fiboHeap *heap, heapData *data, int (*cmp)(heapData *, heapData *));

#endif
