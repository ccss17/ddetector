override CFLAGS += -pthread -fPIC -ldl -shared -g -ggdb 
CC = gcc
DDTECTOR = build/ddtector
DMONITOR = build/dmonitor
DPREDICTOR = build/dpredictor
INCLUDE = -Iinclude
LIB = -Llib -ldd

all: lib/libdd.a ddtector dmonitor dpredictor test

lib/libdd.a: lib/libdd.c
	make -C lib

ddtector: ddtector.c 
	if ! test -d build; then mkdir build; fi
	$(CC) -o $(DDTECTOR).so $? $(CFLAGS) $(INCLUDE) $(LIB)

dmonitor: dmonitor.c
	$(CC) -o $(DMONITOR).so $? $(CFLAGS) $(INCLUDE) $(LIB)

dpredictor: dpredictor.c
	$(CC) -o $(DPREDICTOR) $? -g -ggdb $(INCLUDE) $(LIB)

test: deadlock/abba.c deadlock/dinning_deadlock.c
	make -C deadlock

clean:
	make clean -C deadlock
	if test -d build; then rm build/*; rmdir build; fi
	if test -f dmonitor.trace; then rm dmonitor.trace; fi
