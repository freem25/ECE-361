CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: ptr.o tests/test_ptr

ptr.o: ptr.c ptr.h
	$(CC) $(CFLAGS) -c ptr.c

tests/test_ptr: tests/test_ptr.c ptr.o ptr.h
	$(CC) $(CFLAGS) -o tests/test_ptr tests/test_ptr.c ptr.o

test: tests/test_ptr
	./tests/test_ptr

clean:
	rm -f ptr.o tests/test_ptr



