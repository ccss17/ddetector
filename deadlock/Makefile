override CFLAGS += -pthread -g -ggdb
CC = gcc
OUT = build

all: deadlock
	
deadlock: abba.c dinning_deadlock.c cabbac.c single_thread_cycle.c thread_creation.c
	if ! test -d build; then mkdir build; fi
	$(CC) thread_creation.c -o $(OUT)/thread_creation $(CFLAGS) 
	$(CC) abba.c -o $(OUT)/abba $(CFLAGS) 
	$(CC) single_thread_cycle.c -o $(OUT)/single_thread_cycle $(CFLAGS) 
	$(CC) cabbac.c -o $(OUT)/cabbac $(CFLAGS) 
	$(CC) dinning_deadlock.c -o $(OUT)/dinning_deadlock $(CFLAGS) 

clean:
	if test -d build; then rm build/*; rmdir build; fi
