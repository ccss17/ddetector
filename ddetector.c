#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

/**
TODO
1. cycle 확인 여부 에러 고치기
2. unlock되었을 때 array에서 어떻게 처리할 것인지 고민
**/

//노드를 담는 배열(100개) 선언
pthread_mutex_t *vertex[100] ;

//엣지를 담는 배열 선언 (100개 차기 전에 데드락 잡힌다는 가정)
pthread_mutex_t *edge[100] ;

static int cnt = 0;

int
pthread_mutex_lock(pthread_mutex_t *mutex){

  int (*pthread_mutex_lockp)(pthread_mutex_t *mutex) ;
  char * error ;

	pthread_mutex_lockp = dlsym(RTLD_NEXT, "pthread_mutex_lock") ;
	if ((error = dlerror()) != 0x0)
		exit(1) ;

    int isDup = FALSE ;
    int isCycled = FALSE ;

    //traverse and check duplication
    if(cnt == 0){
      if(vertex[cnt] == mutex){
          isDup = TRUE ;
      }
    }else{
      for(int i =0 ; i < cnt ; i++){
        if(vertex[i] == mutex){
            isDup = TRUE ;
            break ;
        }
      }
    }

    //중복되지 않는다면 vertex와 edge에 넣어주기
    if(!isDup){
      vertex[cnt] = mutex ;
      if(cnt == 0){
        //처음 뮤텍스가 들어온 상황. 아무것도 안 해도 됨.
      }else{
        edge[cnt-1] = mutex ;
      }
      cnt++ ;
    }

    //cycle여부 확인 (for문이 두개 필요함)
    // for(int i = 0; i < cnt; i++){
    //   pthread_mutex_t *src = edge[i] ;
    //   pthread_mutex_t *dest = vertex[i] ;
    //
    //   for(int j = i + 1; j < cnt - 1; j++){
    //     if(*src == *vertex[j] && *dest == *edge[j])
    //       isCycled = TRUE ;
    //   }
    // }

    if(isCycled){
      fprintf(stderr, "*****CAUTION : Deadlock has detected. ******\n") ;
      exit(0);
    }

  //hooking test
    int stat = pthread_mutex_lockp(mutex);
    fprintf(stderr, "pthread_mutex_lockp(%p): %d\n", (void*)mutex, stat);

	return stat ;
}

int
pthread_mutex_unlock(pthread_mutex_t *mutex){
  int (*pthread_mutex_unlockp)(pthread_mutex_t *mutex) = NULL ;
  char * error ;

  if(mutex == 0x0)
    return -1;//err msg

	pthread_mutex_unlockp = dlsym(RTLD_NEXT, "pthread_mutex_unlock") ;
	if ((error = dlerror()) != 0x0)
		exit(1) ;
    //mutex와 관련된 vertex와 edge 모두 삭제

    //hooking test
    int stat = pthread_mutex_unlockp(mutex);
    fprintf(stderr, "pthread_mutex_unlockp(%p): %d\n", (void*)mutex, stat);

  return stat;
}
