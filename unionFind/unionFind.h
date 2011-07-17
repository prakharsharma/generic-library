#include <stdio.h>
#include <stdlib.h>

struct elem_data {
    void *data;
    struct elem_data *leader;
    int rank;
};
typedef struct elem_data element;

element* makeSet (void *);
element* find (element *);
void setUnion (element *, element *);
