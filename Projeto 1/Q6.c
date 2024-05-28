#include "queueQ6.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 5
#define TAREFAS 100
typedef void *(*funcPtr)(void *);

void* addTarefas(void*arg);
void* escalonador(void*arg);
void* rotina1(void*arg);
void* rotina2(void*arg);
void* rotina3(void*arg);

pthread_mutex_t myMutex         = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condEscalonador = PTHREAD_COND_INITIALIZER;
int nThreadsRodando = 0;

pthread_t MyThread[TAREFAS];
int indexThread = 0;
int rc = 0;

void* addTarefas(void* arg) {
    Queue* q = (Queue *) arg;

    for (int i = 0; i < TAREFAS; i++) { // abastece o a lista_pronto, adiciona tarefas pendentes
        int random = rand()%3;

        pthread_mutex_lock(&myMutex); // trava mutex, a fila faz parte da nossa regiao critica
        // push na nossa fila de rotinas (o que thread deve executar quando for criada)
        if(random == 0)       enqueue(q, rotina1);
        else if (random == 1) enqueue(q, rotina2);
        else                  enqueue(q, rotina3);

        pthread_cond_signal(&condEscalonador); // manda um sinal para que o escalonador acorde caso
        // esteja dormindo por que a fila estava vazia, pois agora (apos o enqueue), nao esta mais
        pthread_mutex_unlock(&myMutex); // destrava mutex (fila)
    }
    pthread_exit(NULL);
}

void* escalonador(void* arg) {
    Queue* q = (Queue *) arg;

    for (int i = 0; i < TAREFAS; i++) { // nesse caso o escalonador poderia rodar sem fim (while(1)), mas apenas 
    // para o código poder terminar, ele apenas até que todas as tarefas produzidas por addTarefas sejam concluidas
        
        pthread_mutex_lock(&myMutex); // trava mutex (fila e nThreadsRodando)
        while(nThreadsRodando < N && !empty(q)) { // enquanto puder ter mais threads rodando E threads (rotinas) prontas  
            funcPtr rotina = dequeue(q);
            rc = pthread_create(&MyThread[i], NULL, rotina, NULL);
            nThreadsRodando++;
            indexThread++; // apenas essa thread acessa indexThread
            if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
        }
        
        while(!(nThreadsRodando < N && !empty(q))) pthread_cond_wait(&condEscalonador, &myMutex); // redundancia na 
        // condicao do while para lidar corretamente com casos de spurious wakeup
        
        pthread_mutex_unlock(&myMutex); // trava mutex fila e nThreadsRodando
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    Queue* q = QueueCreate();
    pthread_t escalonador_t, addTarefas_t;

    rc = pthread_create(&escalonador_t, NULL, escalonador, (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    
    pthread_create(&addTarefas_t, NULL, addTarefas, (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    
    pthread_join(escalonador_t, NULL); // quando a thread do escalonador 
    // terminar sua execucao, eh certo que addTarefas ja terminou tambem

    deleteQueue(q);
    pthread_exit(NULL);
}

void* rotina1(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 9999; i++);

    pthread_mutex_lock(&myMutex); // trava mutex (nThreads)            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador); // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_mutex_unlock(&myMutex); // destrava mutex (nThreads)
    pthread_exit(NULL);
}

void* rotina2(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 999; i++);

    pthread_mutex_lock(&myMutex); // trava mutex (nThreads)            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador); // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_mutex_unlock(&myMutex); // destrava mutex (nThreads)
    pthread_exit(NULL);
}

void* rotina3(void* arg) {
    // fazer algo diferente aqui 
    for(int i = 0; i < 99; i++);

    pthread_mutex_lock(&myMutex); // trava mutex (nThreads)            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador); // acorda o escalonador ja que, como uma thread terminou
    // sua execucao, ele deve checar se outra ja pode começar a executar
    pthread_mutex_unlock(&myMutex); // destrava mutex (nThreads)
    pthread_exit(NULL);
}

/*
In file included from Q6.c:1:
queueQ6.h:10:5: error: unknown type name ‘Node’
   10 |     Node *next;
      |     ^~~~
queueQ6.h: In function ‘enqueue’:
queueQ6.h:35:19: warning: assignment to ‘int *’ from incompatible pointer type ‘Node *’ {aka ‘struct node *’} [-Wincompatible-pointer-types]
   35 |     q->rear->next = createNode(func0);
      |                   ^
queueQ6.h:36:13: warning: assignment to ‘Node *’ {aka ‘struct node *’} from incompatible pointer type ‘int *’ [-Wincompatible-pointer-types]
   36 |     q->rear = q->rear->next;
      |             ^
queueQ6.h: In function ‘dequeue’:
queueQ6.h:42:34: error: request for member ‘func’ in something not a structure or union
   42 |     funcPtr it = (q->front)->next->func;
      |                                  ^~
queueQ6.h:44:18: warning: initialization of ‘Node *’ {aka ‘struct node *’} from incompatible pointer type ‘int *’ [-Wincompatible-pointer-types]
   44 |     Node *temp = (q->front)->next; // (q->front) eh o header Node
      |                  ^
queueQ6.h:45:40: error: request for member ‘next’ in something not a structure or union
   45 |     (q->front)->next = (q->front)->next->next;
      |                                        ^~
queueQ6.h: In function ‘deleteQueue’:
queueQ6.h:60:18: warning: assignment to ‘Node *’ {aka ‘struct node *’} from incompatible pointer type ‘int *’ [-Wincompatible-pointer-types]
   60 |         q->front = q->front->next; // front passa a apontar para o proximo
      |                  ^
Q6.c: In function ‘main’:
Q6.c:69:16: warning: implicit declaration of function ‘QueueCreate’ [-Wimplicit-function-declaration]
   69 |     Queue* q = QueueCreate();
      |                ^~~~~~~~~~~
Q6.c:69:16: warning: initialization of ‘Queue *’ {aka ‘struct queue *’} from ‘int’ makes pointer from integer without a cast [-Wint-conversion]
*/