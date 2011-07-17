#include "avlTree.h"
#define MAX 100

int intCompare (void *val1, void *val2)
{
    if (*((int *)val1) == *((int *)val2))
	return 0;
    if (*((int *)val1) < *((int *)val2))
	return 1;
    else
	return 2;
}

void intDisplay (avlNodeData *data)
{
    printf("%d ", *((int *)(data->key)));
    return;
}

void freeNodeData (avlNodeData **data)
{
    /*free((int *)((*data)->data));
      free((int *)((*data)->key));*/
    free(*data);
    *data = NULL;

    return;
}

int main (int argc, char **argv)
{
    /*sample use of avlTree
     * avlNodeData will be just integers*/
    avlNode *root = NULL;
    int i = 5;
    int val[] =  {5, 10, 2, 12, 11, 15, 4, 3};
    int len = 8;
    avlNodeData *data[MAX];

    for (i = 0; i < len; i++) {
	data[i] = (avlNodeData*)malloc(sizeof(avlNodeData));
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
    printf("\n\nDELETION\n\n");
    root = deleteNode(&root, data[4], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    printf("\n\nDELETION\n\n");
    root = deleteNode(&root, data[5], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    printf("\n\nDELETION\n\n");
    root = deleteNode(&root, data[6], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    printf("\n\nDELETION\n\n");
    root = deleteNode(&root, data[3], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    return 0;
}
