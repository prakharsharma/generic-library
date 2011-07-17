#include "fiboHeap.h"

void intDisplay (heapData *data)
{
    printf("%d ", *((int *)(data->key)));
    return;
}

int intCompare (heapData *data1, heapData *data2)
{
    void *val1 = data1->key, *val2 = data2->key;

    if (*((int *)val1) == *((int *)val2))
	return 0;
    if (*((int *)val1) < *((int *)val2))
	return 1;
    else
	return 2;
}

void freeIntData (heapData *data)
{
    /*free((int *)((*data)->data));
      free((int *)((*data)->key));
      free(*data);
     *data = NULL;*/

    return;
}

#define MAX 100
int main (void)
{
    fiboHeap *heap = NULL;
    int i = 0;
    //int val[] =  {5, 10, 2, 12, 11, 15, 4, 3};
    int val[] =  {5, 10, 2, 12, 11};
    int len = 5;
    heapData *data[MAX], *data3_new;
    int j = 9;
    heapNode *node;

    for (i = 0; i < len ; i++) {
	data[i] = (heapData *)malloc(sizeof(heapData));
	data[i]->data = NULL;
	data[i]->key = (void *)(&val[i]);
	heap = insert(heap, data[i], intCompare);
	traverse(heap, intDisplay);
	printf("*********************************************************\n");
    }
    deleteMin(heap, freeIntData, intCompare);
    traverse(heap, intDisplay);
    printf("*********************************************************\n");
    node = lookup(heap, data[3], intCompare);
    data3_new = (heapData *)malloc(sizeof(heapData));
    data3_new->data = NULL;
    data3_new->key = (void *)(&j);
    decreaseKey(heap, node, data3_new, intCompare, freeIntData);
    traverse(heap, intDisplay);
    printf("*********************************************************\n");
    return 0;
}
