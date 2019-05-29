override CFLAGS += -pthread -g -ggdb
CC = gcc

all: deadlock
	
deadlock: abba.c dinning_deadlock.c
	$(CC) abba.c -o abba $(CFLAGS) 
	$(CC) dinning_deadlock.c -o dinning_deadlock $(CFLAGS) 

clean:
	if test -f abba; then rm abba; fi
	if test -f dinning_deadlock; then rm dinning_deadlock; fi
