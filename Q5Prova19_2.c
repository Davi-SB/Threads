#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutexTela = PTHREAD_MUTEX_INIT;

void exibirTela(char *texto);

void* rotinahThreads(void *arg) {
    while(1) {
        pthread_mutex_lock(&mutexTela);
        exibirTela((char *) arg);
        pthread_mutex_unlock(&mutexTela);
    }
    pthread_exit(NULL);
}

/*
A condição existente no sistema é uma condição de corrida, onde há recusrsos
compartilhados entre mais de uma thread, uma região crítica. Nesse caso, a 
região crítica é a tela, região em que multiplas threads poderiam tentar
acessa-la ao mesmo tempo e, com uso da exclusão mutua (mutex), esse acesso 
ocorre de forma segura, uma thread por vez
*/

