override CFLAGS += -pthread -g -ggdb
CC = gcc

all: deadlock
	
deadlock: abba.c dinning_deadlock.c cabbac.c single_thread_cycle.c thread_creation.c
	$(CC) thread_creation.c -o thread_creation $(CFLAGS) 
	$(CC) abba.c -o abba $(CFLAGS) 
	$(CC) single_thread_cycle.c -o single_thread_cycle $(CFLAGS) 
	$(CC) cabbac.c -o cabbac $(CFLAGS) 
	$(CC) dinning_deadlock.c -o dinning_deadlock $(CFLAGS) 

clean:
	if test -f single_thread_cycle; then rm single_thread_cycle; fi
	if test -f abba; then rm abba; fi
	if test -f thread_creation; then rm thread_creation; fi
	if test -f cabbac; then rm cabbac; fi
	if test -f dinning_deadlock; then rm dinning_deadlock; fi
