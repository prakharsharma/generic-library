#ifndef _HASTABLE_H_
#define _HASTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define _DEFAULT_TABLE_SIZE 7

struct _TableData {
	void *data;
	void *key;
};
typedef struct _TableData TableData;

enum _Probing {_LINEAR, _QUADRATIC, _DOUBLE};
typedef enum _Probing Probing;

typedef long (*HashFunc)(void *key);
typedef int (*Comparator)(void*, void*);
typedef void (*FreeTableData)(TableData *);

struct _table {
	int size;
	int used;
	int wasted;
	TableData **array;
	Probing probe;
	HashFunc funcArr;
	Comparator keyCmpFunc;
};
typedef struct _table HashTable;

HashTable* getNewHashTable (int, HashFunc, Comparator, ...);
int insertIntoHashTable (HashTable *, TableData *);
TableData* lookupHashTable (HashTable *, void *key, int *index);
int deleteFromHashTable (HashTable *, void *, FreeTableData);

#endif
