#include <stdio.h>
#include <stdlib.h>

struct _pckt {
	int rc;
	void *data;
};
typedef struct _pckt pckt;

void printSizes ()
{
	printf("sizeof(int) = %d\n", sizeof(int));
	printf("sizeof(double) = %d\n", sizeof(double));
	printf("sizeof(void*) = %d\n", sizeof(void*));
	printf("sizeof(pckt) = %d\n", sizeof(pckt));
	/*printf("sizeof() = %d\n", sizeof());
	printf("sizeof() = %d\n", sizeof());*/
}

int main (int argc, char **argv)
{
	long diff;
	pckt *mem = NULL;

	pckt *haha = (pckt*)malloc(sizeof(pckt));
	haha->data = (void*)malloc(sizeof(int));
	haha->rc = 100;
	printSizes();
	printf("\n");
	printf("starting address of pckt = %p\n", haha);
	printf("starting address of metadata = %p\n", &(haha->rc));
	printf("starting address of data = %p\n", haha->data);
	diff = (long)(haha->data) - (long)(haha);
	printf("difference b/w starting address of pckt and data = %p\n", (int)(haha->data) - (int)(haha));
	printf("starting address of pckt = %p\n", (long)(haha->data) - diff);
	mem = (pckt*)((long)(haha->data) - diff);
	printf("result from memory = %d\n", mem->rc);
	return 0;
}
