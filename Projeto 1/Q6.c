#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 5
#define TAREFAS 100

void* addTarefas(void*arg);
void* escalonador(void*arg);
void* rotina1(void*arg);
void* rotina2(void*arg);
void* rotina3(void*arg);

pthread_mutex_t myMutex         = PTHREAD_MUTEX_INITIALIZER; // um mutex apenas pra evitar deadlock e principalmente por causa do pthread_cond_wait. pergunta que eu explico!!!
pthread_cond_t  condEscalonador = PTHREAD_COND_INITIALIZER;
int nThreadsRodando = 0;

pthread_t MyThread[TAREFAS];
int indexThread = 0;
int rc = 0;

void* addTarefas(void* arg) {
    // Queue* q = (Queue *) arg;

    for (int i = 0; i < TAREFAS; i++) { // abastece o a lista_pronto, adiciona tarefas pendentes
        pthread_mutex_lock(&myMutex); // trava mutex fila
        /*
        // push na nossa fila de rotinas (o que thread deve executar quando for criada)
        int random = rand()%2;
        if(random == 0)      q.push(rotina1);
        else if (random = 1) q.push(rotina2);
        else                 q.push(rotina3);
        */
        pthread_cond_signal(&condEscalonador);
        pthread_mutex_unlock(&myMutex); // destrava mutex fila
    }
    pthread_exit(NULL);
}

void* escalonador(void* arg) {
    // Queue* q = (Queue *) arg;

    for (int i = 0; i < TAREFAS; i++) { // nesse caso o escalonador poderia rodar sem fim (while(1)), mas apenas para o código poder terminar, ele apenas até que todas as tarefas produzidas por addTarefas sejam concluidas
        pthread_mutex_lock(&myMutex); // trava mutex fila e nThreadsRodando
        while(nThreadsRodando < N /*&& !fila.empty()*/) { // enquanto puder ter mais threads rodando E threads (rotinas) prontas  
            // ponteiroPFuncaoVoid rotina = q.pop();
            nThreadsRodando++;
            // rc = pthread_create(MyThread[i], NULL, rotina, NULL);
            indexThread++; // acredito que nao precise de mutex pq so essa thread acessa indexThread
            if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
        }
        pthread_cond_wait(&condEscalonador, &myMutex);
        pthread_mutex_unlock(&myMutex); // trava mutex fila e nThreadsRodando
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    // Queue* q = QueueCreate();
    pthread_t escalonador_t, addTarefas_t;
    // rc = pthread_create(escalonador_t, NULL, escalonador, (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    // pthread_create( addTarefas_t, NULL, addTarefas , (void*) q);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
    pthread_join(escalonador_t, NULL);
    // desalocar q
    pthread_exit(NULL);
}

void* rotina1(void* arg) {
    // fazer algo diferente aqui // for(int i = 0; i < 9999; i++);

    pthread_mutex_lock(&myMutex); // trava mutex nThreads            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador);
    pthread_mutex_unlock(&myMutex); // destrava mutex nThreads
    pthread_exit(NULL);
}

void* rotina2(void* arg) {
    // fazer algo diferente aqui // for(int i = 0; i < 9999; i++);

    pthread_mutex_lock(&myMutex); // trava mutex nThreads            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador);
    pthread_mutex_unlock(&myMutex); // destrava mutex nThreads
    pthread_exit(NULL);
}

void* rotina3(void* arg) {
    // fazer algo diferente aqui // for(int i = 0; i < 9999; i++);

    pthread_mutex_lock(&myMutex); // trava mutex nThreads            
    nThreadsRodando--;
    pthread_cond_signal(&condEscalonador);
    pthread_mutex_unlock(&myMutex); // destrava mutex nThreads
    pthread_exit(NULL);
}