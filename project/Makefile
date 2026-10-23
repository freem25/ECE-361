
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

OBJS = main.o iom361_r4.o sensor.o fsm.o store.o aux.o report.o

.PHONY: all clean test

all: sauna 

sauna: $(OBJS)
	$(CC) $(CFLAGS) -o sauna $(OBJS) $(LDLIBS)

main.o: main.c iom361_r4.h sensor.h fsm.h store.h aux.h report.h
iom361_r4.o: iom361_r4.c iom361_r4.h
sensor.o: sensor.c sensor.h
fsm.o: fsm.c fsm.h
store.o: store.c store.h sensor.h
aux.o: aux.c aux.h
report.o: report.c report.h

test_store_week4: test_store_week4.c store.c store.h sensor.h
	$(CC) $(CFLAGS) test_store_week4.c store.c -o test_store_week4


test: sauna test_store_week4
	./sauna
	./test_store_week4

clean:
	rm -f sauna $(OBJS) test_store_week4

clean:
	rm -f sauna $(OBJS)
