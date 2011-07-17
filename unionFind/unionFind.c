#include "unionFind.h"

element* makeSet (void *data)
{
    element *retVal = NULL;

    retVal = (element *)malloc(sizeof(element));
    retVal->data = data;
    retVal->rank = 0;
    retVal->leader = retVal;

    return retVal;
}

element* find (element *e)
{
    if (e != e->leader)
	e->leader = find(e->leader);
    return e->leader;
}

void setUnion (element *x, element *y)
{
    element *leaderX = find(x);
    element *leaderY = find(y);
    if (leaderX->rank > leaderY->rank)
	leaderY->leader = leaderX;
    else {
	leaderX->leader = leaderY;
	if (leaderX->rank == leaderY->rank)
	    ++(leaderY->rank);
    }

    return;
}

int main (void)
{
    return 0;
}
