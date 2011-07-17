#include "rbTree.h"
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

void intDisplay (rbNodeData *data, nodeClr clr)
{
    printf("%d(%s) ", *((int *)(data->key)), (clr == RED)?"R":"B");
    return;
}

void freeNodeData (rbNodeData **data)
{
    /*free((int *)((*data)->data));
      free((int *)((*data)->key));*/
    free(*data);
    *data = NULL;
    return;
}

int main (int argc, char **argv)
{
    /*sample use of rbtree
     * rbNodeData will be just integers*/
    rbNode *root = NULL;
    int i = 5, j = 10, k = 2, l = 12, m = 20, n = 15;
    int val[] =  {5, 10, 2, 12, 20, 15, 4};
    int len = 7;
    rbNodeData *data[MAX];

    for (i = 0; i < len; i++) {
	data[i] = (rbNodeData*)malloc(sizeof(rbNodeData));
	data[i]->data = (void *)(&val[i]);
	data[i]->key = (void *)(&val[i]);
	insert(&root, data[i], intCompare);
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
    //deleteNode(&root, &data[3], intCompare, freeNodeData);
    deleteNode(&root, data[5], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    printf("\n\nDELETION\n\n");
    deleteNode(&root, data[6], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    printf("\n\nDELETION\n\n");
    deleteNode(&root, data[3], intCompare, freeNodeData);
    printf("Inorder:\t");
    inorder(root, intDisplay);
    printf("\n");
    printf("Postorder:\t");
    postorder(root, intDisplay);
    printf("\n");

    return 0;
}

