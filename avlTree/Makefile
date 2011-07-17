CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG

testAvlTree: testAvlTree.o avlTree.o

testAvlTree.o: testAvlTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) testAvlTree.c

avlTree.o: avlTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) avlTree.c

clean:
	rm -f *.o testAvlTree

all:
	make clean
	make

