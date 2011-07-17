#include "type.h"

int _offset = 0;
void setOffset ()
{
    Type *dummy = (Type*)malloc(sizeof(Type));
    _offset = (int)((long)(&(dummy->data)) - (long)(&(dummy->destructor)));
    free(dummy);

    return;
}

void* newObjectType (void *data, Destructor func, short nestingFlag)
{
    Type *retVal = NULL;

    retVal = (Type*)malloc(sizeof(Type));
    retVal->destructor = func;
    retVal->data = data;
    if (nestingFlag)
	retVal->nesting = 1;
    else
	retVal->nesting = 0;

    return &(retVal->data);
}

/*void destroyObject (void *object)
  {
  Type *pObj = NULL;

  if (!object)
  return;
  pObj = (Type*)((long)object - (long)_offset);
  if (pObj->nesting)
  destroyObject(pObj->data);
  else
  ((Destructor)(pObj->destructor))(pObj->data);
  pObj->data = NULL;
  free(pObj);
  pObj = NULL;

  return;
  }*/

void destroyObject (void *object)
{
    Type *pObj = NULL;

    if (!object)
	return;
    if (!_offset)
	setOffset();
    pObj = (Type*)((long)object - (long)_offset);
    ((Destructor)(pObj->destructor))(pObj->data);
    /*if (pObj->nesting)
      destroyObject(pObj->data);
      else
      ((Destructor)(pObj->destructor))(pObj->data);*/
    pObj->data = NULL;
    free(pObj);
    pObj = NULL;

    return;
}
