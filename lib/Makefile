override CFLAGS += -c -g -ggdb
CC = gcc

all: libdd

libdd: libdd.c
	$(CC) $? -o libdd.a $(CFLAGS) -I../include

clean:
	if test -f libdd.a; then rm libdd.a; fi
