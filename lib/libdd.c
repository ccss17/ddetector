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

void write_marr(pthread_mutex_t * marr[], pthread_t tid){
    int i, last;
    FILE * f;
    f = fopen("dmonitor.trace", "a");

    last = end_of_mutex(marr);
    /*printf("THREAD ID: %ld\n", tid);*/
    fprintf(f, "%ld,", tid);
    for (i=0; i<=last; i++)
        /*if (marr[i] != NULL) printf("\t%p\n", marr[i]);*/
        if (marr[i] != NULL) fprintf(f, "%p,", marr[i]);
    fprintf(f, "\n");

    fclose(f);
}


void insert_mutex(pthread_mutex_t * mutex, pthread_mutex_t * marr[]){
    int i;
    for (i=0; i<MUTEX_CT * THREAD_CT; i++) {
        if (marr[i] == NULL) {
            marr[i] = mutex;
            /*printf("\x1b[32mINSERT MUTEX in (%d): %p\x1b[0m\n", i, mutex);*/
            break;
        }
    }
}

void remove_mutex(pthread_mutex_t * mutex, pthread_mutex_t * marr[]){
    int i;
    for (i=MUTEX_CT * THREAD_CT -1; i>=0; i--) {
        if (marr[i] == mutex) {
            marr[i] = NULL;
            break;
        }
    }
}

int size_marr(pthread_mutex_t * marr[]){
    int i, size;
    for (i=0, size=0; i<MUTEX_CT * THREAD_CT; i++) 
        if (marr[i] != NULL) size ++;
    return size;
}

int check_deadlock(pthread_mutex_t *marr[]){
    int i, j, last, flag;
    pthread_mutex_t * tmp_marr[MUTEX_CT * THREAD_CT];

    if (size_marr(marr) == 2)
        return 0;

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
