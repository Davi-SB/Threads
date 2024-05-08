#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
    int *ID = (int *)threadid;
    char *ret = (char *)malloc(100); // memoria liberada em free(res)

    for (int i = 0; i < 10; i++) printf("hello %d\n", *ID);

    sprintf(ret, "Olá, mundo! Sou eu, a thread %d", *ID);
    pthread_exit((void *)ID);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        printf("Na main: criando thread %d\n", t);
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        
        pthread_create(&threads[t], NULL, PrintHello, (void *)taskids[t]);
    }

    for (int u = 0; u < NUM_THREADS; u++) {
        int *res;
        pthread_join(threads[u], (void **)&res);
        printf("%d\n", *res);
        free(res); // MUITO IMPORTANTE!!!!!
    }
    pthread_exit(NULL);
}
