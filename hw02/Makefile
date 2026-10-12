
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJS = main.o reading.o stats.o histogram.o

.PHONY: all test clean

all: readings

readings: $(OBJS)
	$(CC) $(CFLAGS) -o readings $(OBJS)

main.o: main.c reading.h stats.h histogram.h
	$(CC) $(CFLAGS) -c main.c

reading.o: reading.c reading.h
	$(CC) $(CFLAGS) -c reading.c

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c

histogram.o: histogram.c histogram.h
	$(CC) $(CFLAGS) -c histogram.c

tests/test_stats: tests/test_stats.c stats.o stats.h
	$(CC) $(CFLAGS) -o tests/test_stats tests/test_stats.c stats.o

test: readings tests/test_stats
	./tests/test_stats
	sh regress.sh

clean:
	rm -f readings $(OBJS) tests/test_stats
