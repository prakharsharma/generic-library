CC = gcc
CCOPTS = -c -g
DIR = /home/prakhar/dev/myLibrary
INC_PATH = $(DIR)/list

testTypes: testTypes.o type.o
	$(CC) -o testTypes testTypes.o type.o

type.o: type.c
	$(CC) $(CCOPTS) type.c

testTypes.o: testTypes.c
	$(CC) $(CCOPTS) testTypes.c

clean:
	rm -f *.o testTypes 
all:
	make clean
	make


