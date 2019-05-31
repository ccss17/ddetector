#ifndef _DDETECTOR
#define _DDETECTOR

#define _GNU_SOURCE
#define MUTEX_CT 100
#define THREAD_CT 10
#define DEMONITOR_TRACE "dmonitor.trace"

void insert_mutex(pthread_mutex_t * mutex, pthread_mutex_t * marr[]);
void show_marr(pthread_mutex_t * marr[]);
void write_marr(pthread_mutex_t * marr[], pthread_t tid);
void remove_mutex(pthread_mutex_t * mutex, pthread_mutex_t * marr[]);
int size_marr(pthread_mutex_t * marr[]);
int check_deadlock(pthread_mutex_t * marr[]);
#endif
