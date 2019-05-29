#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MUTEX_CT 100
#define THREAD_CT 10

pthread_mutex_t * marr[MUTEX_CT * THREAD_CT];

int end_of_mutex(pthread_mutex_t * marr[]){ // UNTESTED
    int i;
    for (i = MUTEX_CT * THREAD_CT - 1; marr[i] == NULL && i >= 0; i--) ;
    return i;
}

void show_marr(pthread_mutex_t * marr[]){
    int i, last;

    last = end_of_mutex(marr);
    for (i=0; i<=last; i++)
        if (marr[i] != NULL) printf("MUTEX: %p\n", marr[i]);
}


void insert_mutex(pthread_mutex_t * mutex){
    int i;
    for (i=0; i<MUTEX_CT * THREAD_CT; i++) {
        if (marr[i] == NULL) {
            marr[i] = mutex;
            break;
        }
    }
}

void remove_mutex(pthread_mutex_t * mutex){
    int i;
    for (i=MUTEX_CT * THREAD_CT -1; i>=0; i--) {
        if (marr[i] == mutex) {
            marr[i] = NULL;
            break;
        }
    }
}

int check_deadlock(){
    int i, j, last, flag;
    pthread_mutex_t * tmp_marr[MUTEX_CT * THREAD_CT];

    for (i=0; i<MUTEX_CT * THREAD_CT; i++) 
        tmp_marr[i] = NULL;
    /*memset(tmp_marr, NULL, sizeof(pthread_mutex_t *) * (MUTEX_CT * THREAD_CT));*/

    for (i=0, j=0; i<MUTEX_CT * THREAD_CT; i++) 
        if (marr[i] != NULL)  
            tmp_marr[j++] = marr[i];
    
    /*show_marr(tmp_marr);*/
    /*puts("");*/

    flag = 1;
    for (i=0; i<MUTEX_CT * THREAD_CT; i++) {
        if (tmp_marr[i] != NULL) {
            for (j = i+1; j<MUTEX_CT * THREAD_CT; j++) {
                if (tmp_marr[i] == tmp_marr[j]) {
                    tmp_marr[i] = tmp_marr[j] = NULL;
                    flag = 0;
                    break;
                }
                flag = 1;
            }
        }
        if (flag == 1) return 0;
    }
    return 1;
}

int pthread_mutex_lock (pthread_mutex_t * mutex) {
    int (*lockp)(pthread_mutex_t * ) ; 
	char * error ;
    int result, current_thread_idx ;
	char buf[50] ;
    pthread_t tid;
	
	lockp = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0) 
		exit(1) ;

    insert_mutex(mutex);
    show_marr(marr);
    if (check_deadlock() == 1) {
        printf("\x1b[31mDEADLOCK!!!" "\x1b[0m\n");
    }
    puts("");

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

    remove_mutex(mutex);

    result = unlockp(mutex);
	return result;
}
