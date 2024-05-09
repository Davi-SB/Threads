#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS 200 // num de itens a serem inseridos

int buff[BUFFER_SIZE];
int items = 0; // num de itens no buffer
int first = 0;
int last  = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;// Enquanto o mutex eh usado para garantir a exclusao mutua, as variaveis de condicao sao usadas para notificar outras threads quando uma condicao importante eh alcancada.
pthread_cond_t empty  = PTHREAD_COND_INITIALIZER; // VARIAVEIS DE CONDICAO PRECISAM SER SEMPRE USADAS COM MUTEX!!!!
pthread_cond_t fill   = PTHREAD_COND_INITIALIZER; // Elas permitem que threads esperem por determinadas condicoes antes de continuar sua execucao

void *producer();
void *consumer();
void put(int e);
int  get();

int main() {
    pthread_t consumer_thread;
    pthread_t producer_thread;
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_create(&producer_thread, NULL, producer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
}

/*
pthread_cond_wait(): Bloqueia a thread atual que que seja sinalizado que a condicao associada foi alcancada
pthread_cond_signal(): Sinaliza que a condicao foi alcancada, despertando uma thread que estra esperando por essa condicao.
pthread_cond_broadcast(): Sinaliza que a condicao foi alcancada, despertando todas as threads que estao esperando por essa condicao. 
*/

void put(int e) {
    pthread_mutex_lock(&mutex); // Esta linha trava o mutex para garantir que apenas um thread por vez possa executar a secao critica do codigo, ou seja, a parte do codigo que pode acessar e modificar variaveis compartilhadas, como items, first, last e buff. tem que estar antes de qualquer coisa nesse caso pois tudo acessa as variaveis compartilhadas

    while(items == BUFFER_SIZE) pthread_cond_wait(&empty, &mutex); // while eh boa pratica para evitar sinais falsos (spurious wakeup). em caso de buffer cheio, o wait vai ser chamado. Ao chamar wait, a thread eh bloqueada e o mutex (segundo argumento) eh automaticamente liberado. Assim outras threads podem usar o mutex e acessar a sessao critica enquanto essa thread continua bloquada. o primeiro argumento eh a condicao de desbloqueio da thread. Quando o singal manda um sinal pra variavel do primeiro argumento, essa thread eh liberada (desperada) e tenta automaticamente pegar o mutex "de volta", travar, e assim entao poder continuar a sua execucao
    buff[last] = e; // insere o elemento
    printf("pos %d: ", last); // informa o a posicao em que o ultimo elemento inserido se encontra
    items++; last++;

    last %= BUFFER_SIZE; // if (last == BUFFER_SIZE) last = 0; // modular (array circular)
    if (items == 1) pthread_cond_signal(&fill); // se items eh 1, ele estava vazio antes. Logo, o consumidor pode estar bloqueado (wait). Entao eh enviado um sinal para a variavel em que ele espera o sinal em caso de bloqueio, o fill, para que seja desbloqueado

    pthread_mutex_unlock(&mutex);
}

void *producer() {
    printf("Produtor\n");
    for (int i = 0; i < NUM_ITEMS; i++) { // produtor produz NUM_ITEMS itens e depois termina
        put(i);
        printf("Produzi %d \n", i);
    }
    pthread_exit(NULL);
}

int get() {
    int result;
    pthread_mutex_lock(&mutex);

    while (items == 0) pthread_cond_wait(&fill, &mutex); // espera no caso de buff vazio ate que receba um sinal para encher (fill)
    result = buff[first]; // retorno eh o elemento que removeu, o primeiro
    printf("pos %d: ", first);
    items--; first++; // lazy deletion
    first %= BUFFER_SIZE; // if (first == BUFFER_SIZE) first = 0;
    if (items == BUFFER_SIZE - 1) pthread_cond_signal(&empty); // se estava cheio, o produtor pode estar bloqueado. Se for o caso, tem que acordar
    
    pthread_mutex_unlock(&mutex);
    return result;
}

void *consumer() {
    printf("Consumidor\n");
    for (int i = 0; i < NUM_ITEMS; i++) { // consome NUM_ITEMS itens e depois termina
        int v = get();
        printf("Consumi %d\n", v);
    }
    pthread_exit(NULL);
}
