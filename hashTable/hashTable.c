#include "hashTable.h"
const char _WASTED[20] = "deadbead";

HashTable* getNewHashTable (int n_args, HashFunc func, Comparator cmp, ...)
{
	va_list ap;
	int tableSize = _DEFAULT_TABLE_SIZE;
	Probing probe = _QUADRATIC;
	HashTable *retTable = NULL;
	int i;

	va_start(ap, cmp);
	if (n_args > 3) {
		tableSize = va_arg(ap, int);
		if (n_args > 4)
			probe = va_arg(ap, int);
	}
	va_end(ap);

	retTable = (HashTable *)malloc(sizeof(HashTable));
	retTable->size = tableSize;
	retTable->used = 0;
	retTable->wasted = 0;
	retTable->array = (TableData **)malloc(sizeof(TableData *) * tableSize);
	for (i = 0; i < retTable->size; i++)
		(retTable->array)[i] = NULL;
	retTable->probe = probe;
	retTable->funcArr = func;
	retTable->keyCmpFunc = cmp;

	return retTable;
}

/*INSERTION: action == 0
 * LOOKUP: action == 1*/
int findIndex (HashTable *table, void *key, int action)
{
	int index, try = 0, hash;

	hash = (table->funcArr)(key);
	if (hash <= table->size)
		index = hash;
	else
		index = hash%(table->size);
retry:
	if (!((table->array)[index])) {
		if (action)
			return -1;
		else
			return index;
	} else if ((((table->array)[index]) != (void*)_WASTED) && !(table->keyCmpFunc)(((table->array)[index])->key, key)) {
		if (action)
			return index;
		else
			return -1;
	}
	/*if (!((table->array)[index]))
	  return index;*/
	++try;
	index = (hash + (try*try))%(table->size);
	goto retry;
}

void resizeRehash (HashTable **table);

int insertIntoHashTable (HashTable *table, TableData *val)
{
	int index, threshold;

	threshold = (table->size)/4;
	if (table->used >= threshold)
		resizeRehash(&table);
	index = findIndex(table, val->key, 0);
	if (index != -1) {
		(table->array)[index] = val;
		++(table->used);
	}

	return index;
}

void resizeRehash (HashTable **table)
{
	TableData **newArray, **oldArray;
	int index, i, count = 0, prevUsed = 0;
	int _size = (*table)->size;

	oldArray = (*table)->array;
	prevUsed = (*table)->used;
	newArray = (TableData **)malloc(sizeof(TableData *)*_size*2);
	for (i = 0; i < _size*2; i++)
		newArray[i] = NULL;
	(*table)->array = newArray;
	(*table)->size = _size * 2;
	(*table)->used = 0;
	for (i = 0; i < _size; i++) {
		if (!(oldArray[i]))
			continue;
		else if (oldArray[i] == (void *)_WASTED) {
			continue;
			/*newArray[i] = (void *)_WASTED;
			++((*table)->used);*/
		}
		insertIntoHashTable(*table, oldArray[i]);/*don't have to worry about rCode here, bcoz hashTable doesn't has duplicates anyways*/
		++count;
		if (count == prevUsed)
			break;
	}
	free(oldArray);

	return;
}

TableData* lookupHashTable (HashTable *table, void *key, int *index)
{
	*index = findIndex(table, key, 1);
	if (*index == -1)
		return NULL;
	return (table->array)[*index];
}

void clearMarksRehash (HashTable **table)
{
	TableData **newArray, **oldArray;
	int index, i;
	int _size = (*table)->size;

	oldArray = (*table)->array;
	newArray = (TableData **)malloc(sizeof(TableData *)*_size);
	for (i = 0; i < _size; i++)
		newArray[i] = NULL;
	(*table)->array = newArray;
	(*table)->wasted = 0;
	for (i = 0; i < _size; i++) {
		if (!(oldArray[i]))
			continue;
		if (oldArray[i] == (void *)_WASTED)
			continue;
		insertIntoHashTable(*table, oldArray[i]);
		++((*table)->used);
	}
	free(oldArray);

	return;
}

int deleteFromHashTable (HashTable *table, void *key, FreeTableData freeData)
{
	int index, threshold = (table->size)/2;

	index = findIndex(table, key, 1);
	if (index == -1)
		return -1;
	if (freeData)
		freeData((table->array)[index]);
	(table->array)[index] = (void *)_WASTED;
	--(table->used);
	++(table->wasted);
	if (table->wasted > threshold)
		clearMarksRehash(&table);

	return index;
}
