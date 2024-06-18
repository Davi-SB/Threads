#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_barrier_t barreiraJogo;

void* imagem(void* arg) {
    while(1) {
        // codigo qualquer dentro do laco de cada thread

        pthread_barrier_wait(&barreiraJogo);
    }
    pthread_exit(NULL);
}

void* som(void* arg) {
    while(1) {
        // codigo qualquer dentro do laco de cada thread

        pthread_barrier_wait(&barreiraJogo);
    }
    pthread_exit(NULL);
}

void* controle(void* arg) {
    while(1) {
        // codigo qualquer dentro do laco de cada thread

        pthread_barrier_wait(&barreiraJogo);
    }
    pthread_exit(NULL);
}

void* comunicacao(void* arg) {
    while(1) {
        // codigo qualquer dentro do laco de cada thread

        pthread_barrier_wait(&barreiraJogo);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_barrier_init(&barreiraJogo, NULL, 4);
    // criacao das 4 threads aqui
    // pthread_join nas 4 threads aqui
    pthread_barrier_destroy(&barreiraJogo);
    pthread_exit(NULL);
}




/*
Exclusão Mútua não é uma técnica apropriada para esse problema.
Visto que exclusão mútua é uma pratica para evitar o uso conflitante de recursos 
compartilhados e a questão trata de sincronia entre threads sem que haja região 
crítica, fica claro que exclusão mútua não é uma técnica apropriada, mas sim, 
barreiras.
*/