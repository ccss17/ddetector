#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "dd.h"

pthread_mutex_t * marr[MUTEX_CT * THREAD_CT];

int pthread_mutex_lock (pthread_mutex_t * mutex) {
    int (*lockp)(pthread_mutex_t * ) ; 
	char * error ;
    int result, current_thread_idx ;
	char buf[50] ;
    pthread_t tid;
	
	lockp = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0) 
		exit(1) ;

    insert_mutex(mutex, marr);
    printf("\x1b[32mINSERT MUTEX: %p\x1b[0m\n", mutex);
    write_marr(marr,pthread_self());
    puts("");
    if (check_deadlock(marr) == 1) {
        printf("\x1b[31mDEADLOCK!!!" "\x1b[0m\n");
		/*exit(1) ;*/
    }

    result = lockp(mutex);
	return  result; 	
}

int pthread_mutex_unlock (pthread_mutex_t * mutex) { 
    int (*unlockp)(pthread_mutex_t * ) ; 
	char * error ;
    int result;
	char buf[50] ;
    pthread_t tid;
	
	unlockp = dlsym(RTLD_NEXT, "pthread_mutex_unlock") ;
	if ((error = dlerror()) != 0x0) 
		exit(1) ;

    remove_mutex(mutex, marr);
    printf("\x1b[33mREMOVE MUTEX: %p\n\x1b[0m", mutex);
    puts("");

    result = unlockp(mutex);

	return result;
}
