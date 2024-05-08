#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_msg (void *ptr) {
	char *message = (char *) ptr;
	printf("%s\n", message);
	pthread_exit(NULL);
}

int main() {
	pthread_t thread1, thread2;
	char *msg1 = "I"; char *msg2 = "O";
	int ret1 = pthread_create( &thread1, NULL, print_msg, (void*)msg1);
	int ret2 = pthread_create( &thread2, NULL, print_msg, (void*)msg2);
	
	pthread_join(thread1, NULL); // para a execucao da main ate que a thread  1 termine. sabe que a thread terminou via pthread_exit
	printf("Thread 1 voltou: %d\n", ret1); 
	
	pthread_join(thread2, NULL);
	printf("Thread 2 voltou: %d\n", ret2);
	exit(0);
}

