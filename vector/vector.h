#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define _DEFAULT_VEC_SIZE_ 20
#define _DEFAULT_VECTOR_RESIZE_PERCENT_ 70

#ifndef _DESTRUCTOR_
#define _DESTRUCTOR_
typedef void (*Destructor)(void*);
#endif

struct _Vector {
    int quanta;
    int size;
    int resizeThreshold;
    int used;
    Destructor destructor;
    void **memBlock;
};
typedef struct _Vector Vector;

Vector* newVector(int numArgs, int byteQuanta, Destructor, ...);
void* elementAt(Vector*, int);
void push_back(Vector*, void*);
void push_front(Vector*, void*);
void erase(Vector*, int);

#endif
