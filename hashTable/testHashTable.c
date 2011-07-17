#include <stdio.h>
#include "hashTable.h"

#define MAX 100

void freeIntData (TableData *data)
{
	/*free((int *)((*data)->data));
	free((int *)((*data)->key));
	free(*data);
	*data = NULL;*/

	return;
}

void printTableStats (HashTable *table)
{
	printf("\ttable(%p): ", table);
	printf("size(%d), used(%d), wasted(%d), probe(%d)\n", table->size, table->used, table->wasted, table->probe);
	return;
}

int intCompare (void *val1, void *val2)
{
	if (*((int *)val1) == *((int *)val2))
		return 0;
	if (*((int *)val1) < *((int *)val2))
		return 1;
	else
		return 2;
}

long myHash (void *key)
{
	return (*((int *)key))%10;
}

void printMenu (void)
{
	printf("(1) Insert\n");
	printf("(2) Delete\n");
	printf("(3) Lookup\n");
	printf("(4) Print stats\n");
	printf("(0) Exit\n");

	return;
}

int main (void)
{
	HashTable *table = NULL;
	int i = 0, index, toFind, toDel;
	/*int val[] =  {5, 10, 2, 12, 11, 15, 4, 3};
	int val[] =  {5, 10, 2, 12, 11};
	int len = 5;*/
	TableData *found = NULL;//, *data[MAX];
	int choice = 10, num, cont = 1;
	TableData *tData = NULL;
	int *toInsert = NULL;

	table = getNewHashTable(2, myHash, intCompare);
	printMenu();
	while (cont) {
		printf("Choice: ");
		scanf("%d", &choice);
		//printf("\n");
		switch (choice) {
			case 0:
				cont = 0;
				break;
			case 1:
				printf("\tNumber to insert: ");
				scanf("%d", &num);
				toInsert = (int*)malloc(sizeof(int));
				*toInsert = num;
				tData = (TableData*)malloc(sizeof(TableData));
				tData->data = NULL;
				tData->key = (void*)toInsert;
				index = insertIntoHashTable(table, tData);
				if (index == -1)
					printf("\tInsertion unsuccessful (key already present)\n");
				else
					printf("\t\tInserted %d at index = %d in hashTable\n", num, index);
				break;
			case 2:
				printf("\tNumber to delete: ");
				scanf("%d", &num);
				index = deleteFromHashTable(table, (void *)(&num), freeIntData);
				if (index == -1)
					printf("\t\tDeletion unsuccessful (key not found)\n");
				else
					printf("\t\tdeleted %d from index %d\n", num, index);
				break;
			case 3:
				printf("\tNumber to lookup: ");
				scanf("%d", &num);
				found = lookupHashTable(table, (void*)(&num), &index);
				if (found)
					printf("\t\tfound(%d) at index %d\n", num, index);
				else
					printf("\t\t%d not in table\n", num);
				break;
			case 4:
				printTableStats(table);
				break;
			default:
				printMenu();
				break;
		}
	}
	printf("Thank you for using Hash Table.\n");

	return 0;
}
/*
	for (i = 0; i < len ; i++) {
		data[i] = (TableData *)malloc(sizeof(TableData));
		data[i]->data = NULL;
		data[i]->key = (void *)(&val[i]);
		index = insert(table, data[i]);
		printf("Inserted(%d) at index = %d\n", val[i], index);
		printTableStats(table);
		printf("*********************************************************\n");
	}
	index = 0;
	toFind = val[1];
	toFind = 13;
	found = lookup(table, &index, (void *)(&toFind), intCompare);
	if (found)
		printf("found(%d) at index %d\n", toFind, index);
	else
		printf("%d not in table\n", toFind);
	toDel = val[1];
	index = deleteFromTable(table, (void *)(&toDel), intCompare, freeIntData);
	if (index == -1)
		printf("deletion unsuccessful\n");
	else
		printf("deleted %d from index %d\n", toDel, index);
	printTableStats(table);

}*/
