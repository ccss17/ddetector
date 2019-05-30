override CFLAGS += -pthread -fPIC -ldl -shared -g -ggdb
CC = gcc
DDTECTOR = build/ddtector
DMONITOR = build/dmonitor
DPREDICTOR = build/dpredictor

all: ddtector dmonitor dpredictor test

ddtector: ddtector.c 
	if ! test -d build; then mkdir build; fi
	$(CC) -o $(DDTECTOR).so $? $(CFLAGS) 

dmonitor: dmonitor.c
	$(CC) -o $(DMONITOR).so $? $(CFLAGS) 

dpredictor: dpredictor.c
	$(CC) -o $(DPREDICTOR) $? -g -ggdb

test: deadlock/abba.c deadlock/dinning_deadlock.c
	make -C deadlock

clean:
	make clean -C deadlock
	if test -d build; then rm build/*; rmdir build; fi
