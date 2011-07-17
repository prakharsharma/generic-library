#ifndef _MEMMGR_H
#define _MEMMGR_H
/*a simple custom memory manager based on linked list
 * an instance of mngr can be created by specifying the qunata size.
 * It allocates memory in blocks of quanta size.
 * Mngr allocates addtitonal memory everytime the available memory goes below
 * 80% or whatever specified as parameter*/
#include <stdarg.h>
#include "list.h"
#include "type.h"
#include "/home/prakhar/dev/myLibrary/hashTable/hashTable.h"

#ifndef _DESTRUCTOR_
#define _DESTRUCTOR_
typedef void (*Destructor)(void*);
#endif

struct _memMgr {
    int quanta;
    double upperThreshold;
    double lowerThreshold;
    int initialSize;
    List *inUseList;
    List *freeList;
    Destructor freeAgent;/*destructor for the memory that you allocate*/
    long diff;
    HashTable *hashTable;
};
typedef struct _memMgr MemMgr;

MemMgr* newMemMngr (int numArgs, int size, Destructor, ...);
void* getMem (MemMgr *mgr, int num);
void releaseMem (MemMgr *mgr, void*);
/*TODO:*/void destroyMemMgr (MemMgr *mgr);
#endif
