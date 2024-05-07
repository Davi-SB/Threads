#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
void *PrintHello(void *threadid){     
  printf("Ola, mundo!\n"); 
  pthread_exit(NULL);
}
int main (int argc, char *argv[]){   
  pthread_t thread;   
  int rc;   
  rc = pthread_create(&thread, NULL, PrintHello, NULL);      
  if (rc){         
    printf("ERRO; código de retorno é %d\n", rc);         
    exit(-1);      
  }   
  printf("Main\n");
  pthread_exit(NULL);
}