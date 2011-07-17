CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG

testFiboHeap: testFiboHeap.o fiboHeap.o

testFiboHeap.o: testFiboHeap.c
	$(CC) -D$(FLAGS) $(CCOPTS) testFiboHeap.c

fiboHeap.o: fiboHeap.c
	$(CC) -D$(FLAGS) $(CCOPTS) fiboHeap.c

clean:
	rm -f *.o testFiboHeap

all:
	make clean
	make

