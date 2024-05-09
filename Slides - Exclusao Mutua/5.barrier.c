#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREADS 4
#define ITERACOES 5

pthread_barrier_t barrier;

void *codigo_thread(void *threadid);

int main() {

    pthread_t threads[THREADS];
    int *ids[THREADS];

    pthread_barrier_init(&barrier, NULL, THREADS);

    for (int i = 0; i < THREADS; i++) {
        ids[i] = (int *)malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&threads[i], NULL, codigo_thread, (void *)ids[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
    return 0;
}

void *codigo_thread(void *threadid) {
    printf("Thread %d iniciou \n", *((int *)threadid));

    pthread_barrier_wait(&barrier);

    for (int i = 0; i < ITERACOES; i++) {
        printf("Thread %d iteracao %d \n", *((int *)threadid), i);
        pthread_barrier_wait(&barrier);
    }

    printf("Thread %d terminou \n", *((int *)threadid));
    pthread_exit(NULL);
}
