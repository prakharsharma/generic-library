#include "memMgr.h"
#include "/home/prakhar/dev/myLibrary/list/list.h"

void freeInt (void *data)
{
    int *num = (int*)data;
#ifdef DEBUG
    printf("[%s:%d]Inside %s\n", __FILE__, __LINE__, __func__);
#endif
    free(num);
    return;
}

void printMenu (void)
{
    printf("(1) Insert data\n");
    printf("(2) Delete data\n");
    printf("(3) Print stats\n");
    printf("(0) Exit\n");
    printf("\n");
    //printf("(4) Insert data\n");

    return;
}

void printStats (MemMgr *mgr)
{
    printf("\tSize of inUseList = %d\n", (mgr->inUseList)->size);
    printf("\tSize of freeList = %d\n", (mgr->freeList)->size);

    return;
}

#define MAX_SIZE 10

int main (int argc, char **argv)
{
    MemMgr *mgr = NULL;
    int cont = 1;
    int choice = 0, input, sz = 0, del = 0;
    int *num = NULL;
    int* arrPntr[MAX_SIZE];

    mgr = newMemMngr(6, sizeof(int), freeInt, 80, 20, 100);
    printMenu();
    while (cont) {
	printf ("Choice: ");
	scanf("%d", &choice);
	switch (choice) {
	    case 0:
		cont = 0;
		break;
	    case 1:
		printf("\tNumber to insert: ");
		scanf("%d", &input);
		num = (int*)getMem(mgr, 1);
		*num = input;
		arrPntr[sz++] = num;
		break;
	    case 2:
		releaseMem(mgr, arrPntr[del++]);
		break;
	    case 3:
		printStats(mgr);
		break;
	    default:
		printMenu();
		break;
	}
    }
    destroyMemMgr(mgr);
    printf("Thank you for using custom memory manager.\n");

    return 0;
}
