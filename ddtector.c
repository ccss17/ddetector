#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>

int
pthread_mutex_lock(pthread_mutex_t *mutex){

  int (*pthread_mutex_lockp)(pthread_mutex_t *mutex) ;
  char * error ;

	pthread_mutex_lockp = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0)
		exit(1) ;

	//pthread_mutex_lock과 동일한 기능을 하도록!!!
  int ptr = pthread_mutex_lockp(mutex);
  
  fprintf(stderr, "pthread_mutex_lockp(%p): %d\n", (void*)mutex, ptr);

	return ptr ;


}
