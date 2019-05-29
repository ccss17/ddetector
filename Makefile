override CFLAGS += -pthread -fPIC -ldl -shared -g -ggdb
CC = gcc
DDTECTOR = build/ddtector

all: ddtector test

ddtector: ddtector.c 
	if ! test -d build; then mkdir build; fi
	$(CC) -o $(DDTECTOR).so $? $(CFLAGS) 

test: deadlock/abba.c deadlock/dinning_deadlock.c
	make -C deadlock

clean:
	make clean -C deadlock
	if test -d build; then rm build/*; rmdir build; fi
