CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG

testSplayTree: testSplayTree.o splayTree.o

testSplayTree.o: testSplayTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) testSplayTree.c

splayTree.o: splayTree.c
	$(CC) -D$(FLAGS) $(CCOPTS) splayTree.c

clean:
	rm -f *.o testSplayTree

all:
	make clean
	make

