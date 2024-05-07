#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5     
void *PrintHello(void *threadid){   
  int tid = *((int *)threadid);   
  printf("Ola, mundo! Sou eu, a thread #%d!\n", tid); 
  pthread_exit(NULL);
}
int main (int argc, char *argv[]){   
  pthread_t threads[NUM_THREADS];
  int *taskids[NUM_THREADS];

  int rc;   int t;   
  for(t=0; t<NUM_THREADS; t++){      
   taskids[t] = (int *) malloc(sizeof(int)); *taskids[t] = t;
	 printf("No main: criando thread %d\n", t);      
    rc = pthread_create(&threads[t], NULL, PrintHello, (void *) taskids[t]);      
    if (rc){         
      printf("ERRO; código de retorno é %d\n", rc);         
      exit(-1);      
    }   
  }   
  pthread_exit(NULL);
}