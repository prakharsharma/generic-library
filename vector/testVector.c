#include "vector.h"

void freeInt(void *data)
{
    if (!data)
	return;
    free((int*)data);
    return;
}

void printMenu (void)
{
    printf("[1] Push back\n");
    printf("[2] Push front\n");
    printf("[3] Delete element at\n");
    printf("[4] Element at\n");
    printf("[5] Display Vector\n");
    printf("[6] Print stats\n");
    printf("[7] Exit\n");
    return;
}

void printStats (Vector *vector)
{
    printf("Size=%d, Used=%d\n", vector->size, vector->used);
    return;
}

void displayIntVector (Vector *vector)
{
    int i = 0;
    for (i = 0; i < vector->used; i++)
	printf("%d ", *((int*)elementAt(vector, i)));
    printf("\n");
    return;
}

int main (int argc, char **argv)
{
    Vector *vector = NULL;
    short cont = 1;
    short choice;
    int num;

    vector = newVector(3, sizeof(int), freeInt);
    printMenu();
    while (cont) {
	printf(">Choice: ");
	scanf("%hd", &choice);
	switch (choice) {
	    case 1:
		printf(">Number: ");
		scanf("%d", &num);
		push_back(vector, &num);
		break;
	    case 2:
		printf(">Number: ");
		scanf("%d", &num);
		push_front(vector, &num);
		break;
	    case 3:
		printf(">Index: ");
		scanf("%d", &num);
		erase(vector, num);
		break;
	    case 4:
		printf(">Index: ");
		scanf("%d", &num);
		printf("%d\n", *((int*)elementAt(vector, num)));
		break;
	    case 5:
		displayIntVector(vector);
		break;
	    case 6:
		printStats(vector);
		break;
	    case 7:
		cont = 0;
		break;
	    default:
		printMenu();
		break;
	}
    }
    printf("Thank you for using Vector\n");

    return 0;
}
