#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// REGIAO CRITICA!!! "contador" eh o recurso compartilhado
// que causa condicao de disputa entre as threads
int contador = 0; 

pthread_mutex_t mutexContador = PTHREAD_MUTEX_INIT;

void* doacao(void *arg) {
    pthread_mutex_lock(mutexContador);
    contador++;
    pthread_mutex_unlock(mutexContador);

    // codigo da doacao

    pthread_mutex_lock(mutexContador);
    contador--;
    pthread_mutex_unlock(mutexContador);
    pthread_exit(NULL);
}