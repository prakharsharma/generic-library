CC = gcc
CCOPTS = -c -g

list: testList.o list.o
	$(CC) -o list testList.o list.o
list.o: list.c
	$(CC) $(CCOPTS) list.c
testList.o: testList.c
	$(CC) $(CCOPTS) testList.c
clean:
	rm -f list
	rm -f *.o
all:
	make clean
	make
