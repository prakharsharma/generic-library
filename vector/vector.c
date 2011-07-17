#include "vector.h"

Vector* newVector(int n_args, int byteQuanta, Destructor destructor, ...)
{
    Vector *vector = NULL;
    va_list ap;

    vector = (Vector*)malloc(sizeof(Vector));
    vector->quanta = byteQuanta;
    vector->destructor = destructor;
    vector->size = _DEFAULT_VEC_SIZE_;
    vector->resizeThreshold = _DEFAULT_VECTOR_RESIZE_PERCENT_;
    vector->used = 0;
    va_start(ap, destructor);
    if (n_args > 3) {
	vector->size = va_arg(ap, int);
	if (n_args > 4)
	    vector->resizeThreshold = va_arg(ap, int);
    }
    va_end(ap);
    vector->memBlock = (void **)malloc(sizeof(void*) * vector->size);

    return vector;
}

void* elementAt(Vector* vector, int index)
{
    if (!vector)
	return;
    if (index < 0)
	return NULL;
    return (vector->memBlock)[index];
}

void resizeVector(Vector *vector)
{
    int i = 0;
    void **memPtr = NULL, **tmpMem = NULL;
    int sz = vector->size * 2;

    if (!vector)
	return;

    memPtr = (void**)malloc(sizeof(void*)*sz);
    for (i = 0; i < vector->used; i++) {
	memPtr[i] = (vector->memBlock)[i];
	(vector->memBlock)[i] = NULL;
    }
    tmpMem = vector->memBlock;
    vector->memBlock = memPtr;
    free(tmpMem);
    //vector->memBlock = realloc(vector->memBlock, vector->size);

    return;
}

void push_back(Vector *vector, void *data)
{
    void *mem = NULL;

    if (!vector)
	return;

    if ((((double)vector->used)/((double)vector->size))*100 >= (vector->resizeThreshold))
	resizeVector(vector);
    ++(vector->used);
    mem = malloc(vector->quanta);
    memset(mem, 0, vector->quanta);
    memcpy(mem, data, vector->quanta);
    (vector->memBlock)[vector->used - 1] = mem;

    return;
}

void push_front(Vector *vector, void *data)
{
    int i = 0;
    void *mem = NULL;

    if (!vector)
	return;

    if ((((double)vector->used)/((double)vector->size))*100 >= (vector->resizeThreshold))
	resizeVector(vector);
    ++(vector->used);
    mem = malloc(vector->quanta);
    memset(mem, 0, vector->quanta);
    memcpy(mem, data, vector->quanta);
    for (i = vector->used - 1; i >= 0; i--)
	(vector->memBlock)[i+1] = (vector->memBlock)[i];
    (vector->memBlock)[0] = mem;

    return;
}

void erase(Vector *vector, int index)
{
    void *toErase = NULL;
    int i = 0;

    if (!vector)
	return;
    if (index < 0 || index > vector->used -1)
	return;
    toErase = (vector->memBlock)[index];
    for (i = index; i < vector->used - 1; i++)
	(vector->memBlock)[i] = (vector->memBlock)[i+1];
    --(vector->used);
    (vector->destructor)(toErase);

    return;
}
