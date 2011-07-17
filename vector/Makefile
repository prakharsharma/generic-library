CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG

testVector: testVector.o vector.o
	$(CC) -D$(FLAGS) -o testVector testVector.o vector.o

testVector.o: testVector.c
	$(CC) -D$(FLAGS) $(CCOPTS) testVector.c

vector.o: vector.c
	$(CC) -D$(FLAGS) $(CCOPTS) vector.c

clean:
	rm -f *.o testVector

all:
	make clean
	make
