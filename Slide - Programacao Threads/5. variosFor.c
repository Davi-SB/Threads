#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_THREADS 5     

void *PrintHello(void *threadid){   
  
  int *tid = (int *)threadid; 
  char *ret = (char *)malloc(100);
  int i ;
  
  for(i=0; i<100000; i++) {
  	printf("hello %d\n", *tid);
  }
  sprintf(ret, "Olá, mundo! Sou eu, a thread %d", *tid);
  pthread_exit((void *) tid);
}

int main (int argc, char *argv[]){   
  pthread_t threads[NUM_THREADS]; int *taskids[NUM_THREADS];
   int rc; int t; int u;   
  for(t=0; t<NUM_THREADS; t++){
    printf("No main: criando thread %d\n", t);    
    taskids[t] = (int *) malloc(sizeof(int)); *taskids[t] = t;
	 pthread_create(&threads[t],NULL,PrintHello, (void *)taskids[t]);         
  }
  for(u=0; u<NUM_THREADS;u++) {
    int *res;
    pthread_join(threads[u], (void **) &res);
    printf("%d\n",*res);
  }   
  pthread_exit(NULL);
}