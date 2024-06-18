#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *threadID) {
    int *ID   = (int  *) threadID;
    printf("Ola, mundo! Sou eu, a thread %d\n", *ID);
    pthread_exit((void *)ID);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];

    for(int t = 0; t < NUM_THREADS; t++) {
        printf("Na main: criando thread %d\n", t);
        taskids[t] = (int *) malloc(sizeof(int));
        *taskids[t] = t;

        pthread_create(&threads[t], NULL, PrintHello, (void *)taskids[t]);
    }

    for(int u = 0; u < NUM_THREADS; u++) {
        int *ID; // ponteiro simples pois eh o tipo do retorno da thread
        pthread_join(threads[u], (void *)&ID); // segundo argumento de join eh o que recebe o retorno da thread pelo exit. seu tipo eh **void
        // (void *)&ID deve ser feito dessa forma. o argumento eh um ponteiro duplo pois eh um ponteiro simples pro retorno (que eh um ponteiro simples). 
        printf("%d\n", *ID);
    }
    pthread_exit(NULL);
}
