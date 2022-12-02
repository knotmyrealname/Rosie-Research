CC = gcc
CFLAGS = -Wall -std=c99 -g $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

rosiejson_processor: -ljson-c input.o utility.o output.o

input.o: input.h utility.h
output.o: input.h utility.h output.h
utility.o: utility.h

clean:
	rm -f rosiejson_processor
	rm input.o output.o utility.o
