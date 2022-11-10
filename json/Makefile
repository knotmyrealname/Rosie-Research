CC = gcc
CFLAGS = -Wall -std=c99 -g $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

rosiejson: -ljson-c input.o utility.o

input.o: input.h utility.h
utility.o: utility.h

clean:
	rm -f rosiejson
	rm input.o utility.o
