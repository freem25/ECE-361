
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
store.o: store.c store.h
aux.o: aux.c aux.h
report.o: report.c report.h

test: sauna
	./sauna

clean:
	rm -f sauna $(OBJS)
