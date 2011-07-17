CC = g++
CCOPTS = -c -g
FLAGS = DEBUG
INCPATH = 

testRadixTree: testRadixTree.o radixTree.o
	$(CC) -D$(FLAGS) -o testRadixTree testRadixTree.o radixTree.o

testSuffixTree.o: testRadixTree.cpp
	$(CC) -D$(FLAGS) $(CCOPTS) testRadixTree.cpp

suffixTree.o: radixTree.cpp
	$(CC) -D$(FLAGS) $(CCOPTS) radixTree.cpp

clean:
	rm -f *.o testRadixTree

all:
	make clean
	make


