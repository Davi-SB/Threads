#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5 // numero de threads (numero de arquivos)

int count = 0;
char *target = "MEDEMOTIVO"; // palavra buscada
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

void *countWords(void *filePath) {
    FILE *text;
    char currWord[50];

    text = fopen((char *)filePath, "rt");
    if(text == NULL) { printf("error fopen: %s\n", (char *)filePath); exit(-1); }

    while (fscanf(text, "%49s", currWord) != EOF) {
        pthread_mutex_lock(&Mutex);
        if (strcmp(currWord, target) == 0) count++; // target e count fazem parte da regiao critica, portanto, o mutex realiza a exclusao mutua para que o acesso seja realizado corretamente
        pthread_mutex_unlock(&Mutex);
    }

    fclose(text);
    pthread_exit(NULL);
}

int main() {
    pthread_t myThread[THREADS];
    char *path[THREADS] = {"text1.txt", "text2.txt", "text3.txt", "text4.txt", "text5.txt"}; // nao eh necessario alocacao dinamica nem declarar globalmente visto que eh garantido que todas as threads irao terminar sua execucao antes da main como garante o "for" de "join" em todas as threads do array "myThread"

    for (int i = 0; i < THREADS; i++) {
        int rc = pthread_create(&myThread[i], NULL, countWords, path[i]);
        if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    }
    for (int i = 0; i < THREADS; i++) pthread_join(myThread[i], NULL); // garante que todas as threads terminem sua execucao antes de continuar a execucao da main. Isso eh importante para que o valor de "count" esteja correto, visto que eh necessario terminar a busca por completo em todos os arquivos
    
    // cada arquivo possui a palavra alvo 5 vezes, dessa forma, a saida esperada eh 25
    printf("A palavra <%s> foi encontrada %d vezes nos arquivos.\n", target, count); // apesar de "target" e "count" serem variaveis de regiao critica, nao eh necessario travar o mutex nesse momento da main para acessa-las pois o "for" de join" garante que todas as outras threads criadas (exceto a main) ja finalizaram sua execucao. Assim, nao ha disputa
    pthread_exit(NULL);
}
