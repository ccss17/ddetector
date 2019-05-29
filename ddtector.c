#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MUTEX_CT 100
#define THREAD_CT 10

typedef struct {
    pthread_t tid;
    pthread_mutex_t * mid[MUTEX_CT];
} dd_node ;

dd_node dd_arr[THREAD_CT];

void show_status(){
    int i, j;

    for (i=0; i<THREAD_CT; i++){
    if (dd_arr[i].tid != 0) {
        printf("THREAD[%ld]:\n", dd_arr[i].tid);
        for (j=0; j<MUTEX_CT; j++) {
        if (dd_arr[i].mid[j] != NULL)
            printf("\tMUTEX OF THREAD:%p\n", dd_arr[i].mid[j]);
        }
    } }
}

int mutex_count(pthread_mutex_t * mid[]){ // UNTESTED
    int i, ct=0;
    for (i=0; i<MUTEX_CT; i++) {
        if (mid[i] != NULL) ct++;
    }
    return ct;
}

int set_dd_arr(pthread_t tid, pthread_mutex_t * mutex){
    int i, j;
    for (i = 0; i < THREAD_CT; i++) {
        if (dd_arr[i].tid == 0 || dd_arr[i].tid == tid) {
            if (dd_arr[i].tid == 0) dd_arr[i].tid = pthread_self();
            for (j = 0; j < MUTEX_CT; j++) {
                if (dd_arr[i].mid[j] == NULL) {
                    dd_arr[i].mid[j] = mutex;
                    break;
                }
            }
            break;
        }
    }
    return i;
}

void unset_dd_arr(pthread_t tid, pthread_mutex_t * mutex){
    int i, j;
    for (i = 0; i < THREAD_CT; i++) {
        if (dd_arr[i].tid == tid){
            for (j = 0; j < MUTEX_CT; j++) {
                if (dd_arr[i].mid[j] == mutex) {
                    dd_arr[i].mid[j] = NULL;
                    break;
                }
            }
            if (mutex_count(dd_arr[i].mid) == 0)
                dd_arr[i].tid = 0;
            break;
        }
    }
}

int ddtector(int current_thread_idx, pthread_t tid, pthread_mutex_t * mutex){
    int i, j, k, l;

    for (i = 0; i < THREAD_CT; i++) {
    if (dd_arr[i].tid != tid) { // dd_arr 중에서 현재 스레드의 tid 가 아닌 스레드를 비교 
    for (j = 0; j < MUTEX_CT; j++) {
        if (dd_arr[i].mid[j] == mutex) { // 만약 다른 스레드 중에 현재 스레드와 mutex 와 같은 게 있다면 또 다른 mutex 도 같은 게 있는지 확인 
        for (k = 0; k < MUTEX_CT; k++) {
        for (l = 0; l < MUTEX_CT; l++) {
            if (dd_arr[current_thread_idx].mid[k] != NULL &&
                dd_arr[current_thread_idx].mid[k] != mutex &&
                dd_arr[current_thread_idx].mid[k] == dd_arr[i].mid[l]){ // 두 스레드가 서로다른 두 개의 mutex 를 쥐고 있음. 데드락 발생 

                return 1;
            }
        }}} 
    }}}
    return 0;
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

    tid = pthread_self();
    current_thread_idx = set_dd_arr(tid, mutex);
    puts("In LOCK");
    show_status();
    if (ddtector(current_thread_idx, tid, mutex)) {
        snprintf(buf, 50, "deadlock!");
        fputs(buf, stderr) ;
    } 

    result = lockp(mutex);
	return  result; 	
}

int pthread_mutex_unlock (pthread_mutex_t * mutex) { 
    int (*unlockp)(pthread_mutex_t * ) ; 
	char * error ;
    int result;
	char buf[50] ;
	
	unlockp = dlsym(RTLD_NEXT, "pthread_mutex_unlock") ;
	if ((error = dlerror()) != 0x0) 
		exit(1) ;

    puts("In UNLOCK");
    show_status();
    unset_dd_arr(pthread_self(), mutex);

    result = unlockp(mutex);
	return result;
}
