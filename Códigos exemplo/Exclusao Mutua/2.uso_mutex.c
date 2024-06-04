#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long contador = 0;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER; // geralmente eh criado um mutex para cada regiao critica

void *inc(void *threadid) {
    for (int i = 0; i < 9000000; i++) {
        pthread_mutex_lock(&myMutex); // se T1 tentar travar o mutex e ele ja estiver travado, T1 fica travada ate que o mutex esteja desbloqueado
        contador++;
        pthread_mutex_unlock(&myMutex);
    }
}

void *dec(void *threadid) {
    for (int i = 0; i < 9000000; i++) {
        pthread_mutex_lock(&myMutex); // lock eh bloqueante, mas trylock, nao. Espera ocupada, se tivver travado continua checando. retorna 0 quando consegue bloquear. 
        contador--;
        pthread_mutex_unlock(&myMutex);
    }
}

int main(int argc, char *argv[]) {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, inc, NULL);
    pthread_create(&thread2, NULL, dec, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Valor final do contador: %ld\n", contador);
    pthread_exit(NULL);
}
