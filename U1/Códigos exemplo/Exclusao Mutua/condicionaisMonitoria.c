#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
int pode_prosseguir = 0;

void* thread1(void* arg) {
    printf("Thread 1: comecei\n");

    pthread_mutex_lock(&mutex);
    printf("Thread 1: vou dormir...\n");
    
    // while eh boa pratica para evitar sinais falsos (spurious wakeup)
    while (!pode_prosseguir) pthread_cond_wait(&cond, &mutex);
    
    // quando acordar
    printf("Thread 1: acordei\n");
    pthread_mutex_unlock(&mutex);

    printf("Thread 1: finalizei\n");
    pthread_exit(NULL);
}

void* thread2(void* arg) {
    printf("Thread 2: comecei\n");

    pthread_mutex_lock(&mutex);

    // Simula algum trabalho antes de conceder permissão
    for (int i = 1; i <= 5; i++) {
        sleep(1);
        printf("Thread 2: trabalhando (%d)\n", i);
    }
     
    printf("Thread 2: vou conceder permissão...\n");
    pode_prosseguir = 1; // Sinaliza que a thread 1 pode continuar
    pthread_cond_broadcast(&cond); // Notifica a thread 1 disso
    
    printf("Thread 2: permissão concedida!\n");
    pthread_mutex_unlock(&mutex);

    printf("Thread 2: finalizei \n");
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_exit(NULL);
}

