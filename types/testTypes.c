#include "type.h"

void freeInt (void *data)
{
	if (!data)
		return;
	free((int*)data);

	return;
}

int main (int argc, char **argv)
{
	void *num = NULL;
	void *leafObj = NULL, *parObj = NULL;

	setOffset();
	num = malloc(sizeof(int));
	*((int*)num) = 10;
	leafObj = newObjectType(num, freeInt, 0);
	parObj = newObjectType(leafObj, freeInt, 1);
	destroyObject(parObj);

	return 0;
}
