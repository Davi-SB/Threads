#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_THREADS 7

void *minhaRotina(void *threadID) {
    int *ID = (int *) threadID;
    printf("Thread %d rodando...\n", *ID);
    pthread_exit((void *)ID);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int IDs[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) {
        printf("Na main: criando thread %d\n", i);
        IDs[i] = i;
        pthread_create(&threads[i], NULL, minhaRotina, (void *) &IDs[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        // ponteiro simples pq eh o tipo do retorno da thread
        int *ID;
        
        // segundo argumento de join eh o que recebe o retorno da thread pelo exit. seu tipo eh **void
        // (void *)&ID deve ser feito dessa forma. 
        // o argumento eh um ponteiro duplo pois eh um ponteiro simples pro retorno (que eh um ponteiro simples). 
        pthread_join(threads[i], (void *)&ID);
        printf("%d\n", *ID);
    }

    pthread_exit(NULL);
}

