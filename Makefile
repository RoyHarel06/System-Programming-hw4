CC=gcc

all: graph

graph: graphs.c
	$(CC) -Wall -g -o graph graphs.c
	
.PHONY: clean valgrind
	
clean:
	rm -f *.o *.a *.so isort txtfind