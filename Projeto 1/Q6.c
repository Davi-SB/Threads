#include "queueQ6.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 3
#define TAREFAS 10
typedef void *(*funcPtr)(void *);

void* addTarefas(void*arg);
void* escalonador(void*arg);
void* rotina1(void*arg);
void* rotina2(void*arg);
void* rotina3(void*arg);
funcPtr rotina[3] = {rotina1, rotina2, rotina3};
int IDS[TAREFAS];

pthread_mutex_t mutexFilaRodando = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexFilaPronto  = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condEscalonador  = PTHREAD_COND_INITIALIZER;
int nThreadsRodando = 0;

pthread_t MyThread[TAREFAS];
int indexThread = 0;
int rc = 0;

void* addTarefas(void* arg) {
    int random;
    Queue* q = (Queue *) arg;

    // abastece o a lista_pronto, adiciona tarefas pendentes
    for (int i = 0; i < TAREFAS; i++) { 
        random = rand()%3;

        // trava mutex, a fila faz parte da nossa regiao critica
        pthread_mutex_lock(&mutexFilaPronto); 
        
        // push na nossa fila de rotinas 
        // (o que thread cada deve executar quando for criada)                           
        enqueue(q, rotina[random]);
        printf("Tarefa %d produzida\n", i+1);
                                      
        // manda um sinal para que o escalonador acorde caso
        // esteja dormindo pela fila estar vazia, pois 
        // agora (apos o enqueue), nao esta mais     
        pthread_cond_signal(&condEscalonador); 
        
        // destrava mutex (fila)
        pthread_mutex_unlock(&mutexFilaPronto); 
    }
    pthread_exit(NULL);
}

void* escalonador(void* arg) {
    Queue* q = (Queue *) arg;
    int i = 0, j = 0;

    // nesse caso o escalonador poderia rodar sem fim (while(1)), mas apenas 
    // para o código poder terminar, ele apenas até que todas as tarefas produzidas 
    // por addTarefas sejam concluidas
    while(i < TAREFAS) { 
        
        // trava mutex (fila e nThreadsRodando)
        pthread_mutex_lock(&mutexFilaPronto); // trava o enqueue
        pthread_mutex_lock(&mutexFilaRodando); // trava o dequeue

        // enquanto puder ter mais threads rodando E threads (rotinas) prontas  
        while(nThreadsRodando < N && !empty(q)) {
            funcPtr rotina = dequeue(q);
            rc = pthread_create(&MyThread[i], NULL, rotina, (void*) &IDS[i]);
            if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
            else printf("Tarefa %d comecou a excecucao\n", i+1);
            
            nThreadsRodando++;
            indexThread++; // apenas essa thread acessa indexThread!!!
            i++;
        }

        // escalonador nao deve dormir quando a fila estiver vazia mas todas as tarefas ja forem executadas
        while((nThreadsRodando >= N || empty(q)) && i != TAREFAS) { 
            // redundancia na condicao do while para lidar corretamente com casos de spurious wakeup
            pthread_mutex_unlock(&mutexFilaPronto);
            pthread_cond_wait(&condEscalonador, &mutexFilaRodando);
            pthread_mutex_lock(&mutexFilaPronto);
        }
        
        // trava mutex fila e nThreadsRodando
        pthread_mutex_unlock(&mutexFilaRodando);
        pthread_mutex_unlock(&mutexFilaPronto);
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < TAREFAS; i++) IDS[i] = i;

    Queue* q = createQueue();
    pthread_t escalonador_t, addTarefas_t;

    rc = pthread_create(&escalonador_t, NULL, escalonador, (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    
    pthread_create(&addTarefas_t, NULL, addTarefas, (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    
    // quando a thread do escalonador terminar sua execucao, eh certo que addTarefas ja terminou tambem
    pthread_join(escalonador_t, NULL); 

    deleteQueue(q);
    pthread_exit(NULL);
}

void* rotina1(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 9999; i++);
    printf("Tarefa %d finalizada\n", *((int *) arg)+1);

    // trava mutex (nThreadsRodando)
    pthread_mutex_lock(&mutexFilaRodando); 
    nThreadsRodando--;
    
    // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_cond_signal(&condEscalonador); 
    
    // destrava mutex (nThreadsRodando)
    pthread_mutex_unlock(&mutexFilaRodando);
    pthread_exit(NULL);
}

void* rotina2(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 999; i++);
    printf("Tarefa %d finalizada\n", *((int *) arg)+1);

    // trava mutex (nThreadsRodando)
    pthread_mutex_lock(&mutexFilaRodando); 
    nThreadsRodando--;
    
    // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_cond_signal(&condEscalonador); 

    // destrava mutex (nThreadsRodando)
    pthread_mutex_unlock(&mutexFilaRodando);
    pthread_exit(NULL);
}

void* rotina3(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 99; i++);
    printf("Tarefa %d finalizada\n", *((int *) arg)+1);

    // trava mutex (nThreadsRodando)
    pthread_mutex_lock(&mutexFilaRodando); 
    nThreadsRodando--;
    
    // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_cond_signal(&condEscalonador); 
    
    // destrava mutex (nThreadsRodando)
    pthread_mutex_unlock(&mutexFilaRodando); 
    pthread_exit(NULL);
}