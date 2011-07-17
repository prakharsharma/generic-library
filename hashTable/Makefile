CC = gcc
CCOPTS =  -c -g
FLAGS = DEBUG

testHashTable: testHasTable.o hashTable.o
	$(CC) -D$(FLAGS) -o testHashTable testHashTable.o hashTable.o

hashTable.o: hashTable.c
	$(CC) -D$(FLAGS) $(CCOPTS) hashTable.c

testHasTable.o: testHashTable.c
	$(CC) -D$(FLAGS) $(CCOPTS) testHashTable.c

clean:
	rm -f *.o testHashTable

all:
	make clean
	make


