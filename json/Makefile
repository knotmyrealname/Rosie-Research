CC = gcc
CFLAGS = -Wall -std=c99 -g $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

rosiejson: -ljson-c


clean:
	rm -f rosiejson
