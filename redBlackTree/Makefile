CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG

testRbTree: testRbTree.o rbTree.o

testRbTree.o: testRbTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) testRbTree.c

rbTree.o: rbTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) rbTree.c

clean:
	rm -f *.o testRbTree

all:
	make clean
	make

