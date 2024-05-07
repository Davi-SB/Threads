#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void *print_msg( void *ptr);

main() {
  pthread_t thread1, thread2;
  char *msg1 = "Thread 1"; char *msg2 = "Thread 2";
  int ret1 = pthread_create( &thread1, NULL, print_msg, (void*)msg1);
  int ret2 = pthread_create( &thread2, NULL, print_msg, (void*)msg2);
  pthread_join( thread1, NULL);
  printf("Thread 1 voltou: %d\n",ret1); 
  pthread_join( thread2, NULL);
  printf("Thread 2 voltou: %d\n",ret2);
  exit(0);
}
void *print_msg ( void *ptr ) {
  char *message;
  message = (char *) ptr;
  printf("%s \n", message);
  pthread_exit(NULL);
}
