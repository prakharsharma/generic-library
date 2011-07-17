#include "splayTree.h"

int intCompare (void *val1, void *val2)
{
    if (*((int *)val1) == *((int *)val2))
	return 0;
    if (*((int *)val1) < *((int *)val2))
	return 1;
    else
	return 2;
}

void intDisplay (splayNodeData *data)
{
    printf("%d ", *((int *)(data->key)));
    return;
}

void freeNodeData (splayNodeData **data)
{
    /*free((int *)((*data)->data));
      free((int *)((*data)->key));*/
    free(*data);
    *data = NULL;

    return;
}

#define MAX 100

int main (int argc, char **argv)
{
    /*sample use of splayTree
     * splayNodeData will be just integers*/
    splayNode *root = NULL;
    int i = 5, j = 10, k = 2, l = 12, m = 20, n = 15;
    int val[] =  {5, 10, 2, 12, 11, 15, 4, 3};
    int len = 8;
    splayNodeData *data[MAX];

    for (i = 0; i < len; i++) {
	data[i] = (splayNodeData*)malloc(sizeof(splayNodeData));
	data[i]->data = (void *)(&val[i]);
	data[i]->key = (void *)(&val[i]);
	root = insert(&root, data[i], intCompare);
	printf("\n*********** ITERATION %d ***********\n", i);
	printf("Inorder:\t");
	inorder(root, intDisplay);
	printf("\n");
	printf("Postorder:\t");
	postorder(root, intDisplay);
	printf("\n");
	printf("***********************************\n");
    }
    printf("\nLOCATE\n\n");
    root = locate(root, data[4], intCompare);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    return 0;
}

