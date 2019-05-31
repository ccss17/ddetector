override CFLAGS +=  -g -ggdb
CC = gcc

all: libdd

libdd: libdd.c
	$(CC) $? -o libdd.a $(CFLAGS) -I../include -fPIC -ldl -shared -pthread

clean:
	if test -f libdd.a; then rm libdd.a; fi
