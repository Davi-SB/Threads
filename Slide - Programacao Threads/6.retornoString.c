#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 5     

void *PrintHello(void *threadid) {  
    int  *tid = (int  *) threadid; 
    char *ret = (char *) malloc(100);

    sprintf(ret, "Olá, mundo! Sou eu, a thread %d", *tid);
    pthread_exit((void *)ret);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        printf("No main: criando thread %d\n", t);    
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        pthread_create(&threads[t], NULL, PrintHello, (void *)taskids[t]);
    }

    for (int u = 0; u < NUM_THREADS; u++) {
        char *res;
        pthread_join(threads[u], (void **)&res);
        printf("%s\n", res);
        free(res);
    } 
	// garantia que todas as outras threads que nao a main ja terminaram
	for (int i = 0; i < NUM_THREADS; i++) free(taskids[i]);
    pthread_exit(NULL);
}
