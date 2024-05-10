#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREADS 4
#define ITERACOES 5

pthread_barrier_t barrier; // nao pode ser inicializada estaticamente (aqui)
void *codigo_thread(void *threadid);

int main() {
    pthread_barrier_init(&barrier, NULL, THREADS); // inicializacao da barreira dinamicamente (destroy necessario depois). Terceiro argumento eh o numero de threads que a barreira vai alinhar. A barreira vai segurando threads e quando alcança essa quantidade, libera todas (sincronia)
    pthread_t threads[THREADS];
    int *ids[THREADS];

    for (int i = 0; i < THREADS; i++) {
        ids[i] = (int *)malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&threads[i], NULL, codigo_thread, (void *) ids[i]);
    }

    for (int i = 0; i < THREADS; i++) pthread_join(threads[i], NULL);
    pthread_barrier_destroy(&barrier); // libera a alocacao da barreira
    pthread_exit(NULL);
}

void *codigo_thread(void *threadid) {
    printf("Thread %d iniciou \n", *((int *)threadid) );

    // pthread_barrier_wait(&barrier);

    for (int i = 0; i < ITERACOES; i++) {
        printf("Thread %d iteracao %d \n", *((int *)threadid), i);
        pthread_barrier_wait(&barrier); // coloca a thread que chamou o barrier_wait na barreira, fica bloqueada lá
    }

    printf("Thread %d terminou \n", *((int *)threadid));
    pthread_exit(NULL);
}
