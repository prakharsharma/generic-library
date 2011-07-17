#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void mainMenu (void)
{
	printf("1. to push back\n");
	printf("2. to push front\n");
	printf("3. to delete from front\n");
	printf("4. to delete from end\n");
	printf("5. to display\n");
	printf("6. to exit\n");
	printf("9. anytime to go back to the main menu\n");
	return;
}

void freeMyData (void *data)
{
	free(data);
	return;
}

void displayList (List *myList)
{
	ListNode *node = myList->head;
	while (node != myList->tail) {
		printf(" %d", *((int*)(node->data)));
		node = node->next;
	}
	printf(" %d\n\n", *((int*)(node->data)));
	return;
}

int main (int argc, char **argv)
{
	List *myList = NULL;
	void *data = NULL;
	int cntrl = 1, choice, num;

	mainMenu();
	myList = allocList();
	while (cntrl) {
		printf("Action: ");
		scanf("%d", &choice);
		printf("\n");
		switch(choice) {
			case 1:
				printf("\tNumber to insert ? \n");
				scanf("%d", &num);
				data = malloc(sizeof(int));
				*((int*)data) = num;
				insertAtListTail(myList, makeNode(data));
				break;
			case 2:
				printf("\tNumber to insert ? \n");
				scanf("%d", &num);
				data = malloc(sizeof(int));
				*((int*)data) = num;
				insertAtListHead(myList, makeNode(data));
				break;
			case 3:
				deleteFromListHead(myList, freeMyData);
				break;
			case 4:
				deleteFromListTail(myList, freeMyData);
				break;
			case 5:
				displayList(myList);
				break;
			case 6:
				cntrl = 0;
				break;
			case 9:
				mainMenu();
				break;
			default:
				printf("Invalid input\n");
				mainMenu();
				break;
		}
	}
	return 0;
}
