#ifndef _TYPE_H
#define _TYPE_H

#include <stdio.h>
#include <stdlib.h>

#define NESTED_FLAG 0x1
#define NOT_NESTED_FLAG 0x0

#ifndef _DESTRUCTOR_
#define _DESTRUCTOR_
typedef void (*Destructor)(void*);
#endif

struct _Type {
    void *destructor;
    void *data;
    short nesting;
};
typedef struct _Type Type;
typedef struct _Type* pType;

void *newObjectType (void *, Destructor destructor, short);
void destroyObject (void *);

#endif
