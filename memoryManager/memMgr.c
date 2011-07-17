#include "memMgr.h"

/*address based Hash function*/
#define LARGE_PRIME_NUMBER 303983 
long myHash( void *key)
{
    return ((long)key >> 3)%LARGE_PRIME_NUMBER;
}

/*for comparing keys of Hash function*/
int keyComparator (void *k1, void *k2)
{
    long addr1 = (long)k1;
    long addr2 = (long)k2;

    if (addr1 == addr2)
	return 0;
    else if (addr1 < addr2)
	return 1;
    else
	return 2;
}

struct _managedMemory {
    int rc;
    void *_listNodeHandle;
    void *data;
};
typedef struct _managedMemory MngdMem;

#define MNGDMEMNODE_FROM_GENTYPE(x) ((MngdMem*)(*((void**)(x))))

Destructor usrMemDestructor;/*dangerous stuff here, this pointer will get setup at runtime*/

void freeMngdMem (void *memory)
{
    MngdMem *mem = (MngdMem*)memory;
    //MngdMem *mem = MNGDMEMNODE_FROM_GENTYPE(memory);
#ifdef DEBUG
    printf("[%s:%d]Inside %s\n", __FILE__, __LINE__, __func__);
#endif
    if (mem->data)
	usrMemDestructor(mem->data);
    mem->data = NULL;
    mem->_listNodeHandle = NULL;
    free(mem);
    memory = NULL;

    return;
}

long setUpMemDiff ()
{
    long retVal = 0;
    MngdMem *memory = NULL;

    memory = (MngdMem*)malloc(sizeof(MngdMem));
    //memory->data = malloc(sizeof(int));
    retVal = (long)(&(memory->data)) - (long)(&(memory->rc));
    //retVal = (long)(memory->data) - (long)(&(memory->rc));
    //free(memory->data);
    free(memory);
    memory = NULL;

    return retVal;
}

void setUpFreeList (MemMgr *mgr)
{
    int i = 0;
    MngdMem *memory = NULL;
    //ListNode *node = NULL;
    void *node = NULL;
    void *baseTypObj = NULL;
    void *nestedTypObj = NULL;

    mgr->freeList = allocList();
    for (i = 0; i < mgr->initialSize; i++) {
	memory = (MngdMem*)malloc(sizeof(MngdMem));
	memory->rc = 0;
	memory->data = malloc(mgr->quanta);
	baseTypObj = newObjectType((void*)memory, freeMngdMem, 0);
	//node = makeNode((void*)memory);
	node = makeNode(baseTypObj);
	insertAtListHead(mgr->freeList, node);
	memory->_listNodeHandle = node;
    }

    return;
}

MemMgr* newMemMngr (int numArgs, int q, Destructor destructor, ...)
{
    va_list ap;
    MemMgr *mgr = NULL;

    mgr = (MemMgr*)malloc(sizeof(MemMgr));
    mgr->quanta = q;
    va_start(ap, destructor);
    if (numArgs > 3) {
	mgr->upperThreshold = va_arg(ap, int);
	if (numArgs > 4) {
	    mgr->lowerThreshold = va_arg(ap, int);
	    if (numArgs > 5)
		mgr->initialSize = va_arg(ap, int);
	    else
		mgr->initialSize = 50;
	} else {
	    mgr->lowerThreshold = 20;
	    mgr->initialSize = 50;
	}
    } else {
	mgr->upperThreshold = 80;
	mgr->lowerThreshold = 20;
	mgr->initialSize = 50;
    }
    va_end(ap);
    mgr->inUseList = allocList();
    setUpFreeList(mgr);
    mgr->freeAgent = destructor;
    usrMemDestructor = destructor; /*setting up global functon pointer*/
    mgr->diff = setUpMemDiff();
    mgr->hashTable = getNewHashTable(4, myHash, keyComparator, 199);

    return mgr;
}

void adjustRatio (MemMgr *mgr)
{
    double ratio = 0.0;

    ratio = (((mgr->inUseList)->size)/((mgr->freeList)->size))*100;
    if (ratio < (mgr->upperThreshold))
	return;
    setUpFreeList(mgr);

    return;
}

void* getMem (MemMgr *mgr, int num)
{
    /*remove a block from freeList and add it to inUseList
     * return a pointer to this block. Check to see if usage beyond
     * upperThreshold*/
    //ListNode *node = NULL;
    void *node = NULL, *lData = NULL;
    MngdMem *memory = NULL;
    ListNode *lNode = NULL;
    TableData *tableData = NULL;

    node = deleteFromListHead(mgr->freeList, 0/*NULL*/);/*passing NULL as freePointer just
							  removes an element from the list*/
    lNode = LISTNODE_FROM_GENTYPE(node);
    lData = lNode->data;
    memory = MNGDMEMNODE_FROM_GENTYPE(lData);
    ++(memory->rc);
    insertAtListTail(mgr->inUseList, node);
    memory->_listNodeHandle = node;
    adjustRatio(mgr);
    tableData = (TableData*)malloc(sizeof(TableData));
    tableData->key = memory->data;
    tableData->data = &(memory->data);
    insertIntoHashTable(mgr->hashTable, tableData);

    return (void*)(memory->data);
}

void purgeMem (MemMgr *mgr)
{
    double ratio = 0.0;
    int newFreeListSize = 0, i = 0;

    ratio = (((mgr->inUseList)->size)/((mgr->freeList)->size))*100;
    if (ratio < (mgr->lowerThreshold))
	return;
    newFreeListSize = ((mgr->inUseList)->size)*(100/(mgr->lowerThreshold));
    for (i = newFreeListSize; i > 0; i--) {
	deleteFromListHead(mgr->freeList, 1);
	//node = deleteFromListHead(mgr->freeList, NULL/*mgr->freeAgent*/);
	/*limitation of C/C++ type languages where functions can not be
	 * created at runtime. If they could have been then freeAgent
	 * could have been modified at runtime to handle instrumented
	 * data. Alas will have to do on our own.*/
	/*memory = (MngdMem*)((long)(node->data) - mgr->diff);
	  (mgr->freeAgent)(memory->data);
	  memory->_listNodeHandle = NULL;
	  free(memory);
	  memory = NULL;
	  node->next = NULL;
	  node->prev = NULL;
	  free(node);
	  node = NULL;*/
    }

    return;
}

void releaseMem (MemMgr *mgr, void *mem)
{
    MngdMem *memHandle = NULL;
    void *nodeHandle = NULL;
    TableData *tableData = NULL;
    int index = -1;

    tableData = lookupHashTable(mgr->hashTable, mem, &index);
    if (index == -1) {
#ifdef DEBUG
	printf("DEBUG: memory not found in hash table\n");
#endif
	return;
    }
    deleteFromHashTable(mgr->hashTable, mem, NULL);/*TODO: put a useful destructor in place of NULL*/
    memHandle = (MngdMem*)((long)(tableData->data) - mgr->diff);
    --(memHandle->rc);
    nodeHandle = memHandle->_listNodeHandle;
    deleteFromList(mgr->inUseList, nodeHandle, 0/*NULL*/);
    insertAtListTail(mgr->freeList, nodeHandle);
    purgeMem(mgr);

    return;
}

/*TODO:*/
void destroyMemMgr (MemMgr *mgr)
{
    /* iterate over the inUseList and freeList and purge all the memory*/
    freeList(mgr->freeList);
    freeList(mgr->inUseList);
    mgr->inUseList = NULL;
    mgr->freeList = NULL;
    free(mgr);

    return;
}
