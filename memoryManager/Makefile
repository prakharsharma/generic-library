CC = gcc
CCOPTS = -c -g
FLAGS = DEBUG
DIR = /home/prakhar/dev/myLibrary/

testMemMgr: testMemMgr.o memMgr.o list.o type.o
	$(CC) -D$(FLAGS) -o testMemMgr testMemMgr.o memMgr.o list.o type.o ../hashTable/hashTable.o

type.o: type.c
	$(CC) -D$(FLAGS) $(CCOPTS) type.c

list.o: list.c
	$(CC) -D$(FLAGS) $(CCOPTS) list.c

memMgr.o: memMgr.c
	$(CC) $(CCOPTS) -D$(FLAGS) memMgr.c

testMemMgr.o: testMemMgr.c
	$(CC) $(CCOPTS) -D$(FLAGS) testMemMgr.c

clean:
	rm -f *.o 
	rm -f testMemMgr
all:
	make clean
	make


