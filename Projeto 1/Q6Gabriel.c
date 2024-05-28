#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_THREADS 5
#define NUM_FUNCOES 40

// int num_threads;
int threadsSimultaneas = 0;

typedef int (*funcPtr)(int);

typedef struct{
  funcPtr functions[NUM_FUNCOES];
  int head, tail, size;
} funcQueue; //fila de funcoes que o escalonador vai passar pras threads

void criarFila(funcQueue* q) {
  q->head = 0;
  q->tail = -1;
  q->size = 0;
}

int enqueue(funcQueue* q, funcPtr func) {
  q->tail = (q->tail + 1);
  q->functions[q->tail] = func;
  q->size++;
  return 0;
}

funcPtr dequeue(funcQueue* q) {
  funcPtr func = q->functions[q->head];
  q->head = (q->head + 1);
  q->size--;
  return func;
}

funcQueue fila;

pthread_mutex_t mutex_acao = PTHREAD_MUTEX_INITIALIZER; //mutex pra travar threadsSimultaneas para alteracao
pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER; //mutex para travar a fila para alteracao
pthread_cond_t filaVazia = PTHREAD_COND_INITIALIZER; //condicional para caso a fila esteja vazia
pthread_cond_t maxThreads = PTHREAD_COND_INITIALIZER; //condicional caso o numero maximo de threads simultaneas seja atingido

int fun1(int x) {
  return x + 1;
}

int fun2(int x) {
  return x * 2;
}

int fun3(int x) {
  return x - 3;
}

int fun4(int x) {
  return x*x;
}

void *enlistar(){
  pthread_mutex_lock(&mutex_fila); //trava pra alterar (add funcao) a fila
  for(int i = 0; i < NUM_FUNCOES; i++){
    int qualFuncao = rand() % 4; //passa uma funcao aleatoria para a fila
    switch(qualFuncao){
      case 0:
        enqueue(&fila, fun1);
        break;
      case 1:
        enqueue(&fila, fun2);
        break;
      case 2:
        enqueue(&fila, fun3);
        break;
      case 3:
        enqueue(&fila, fun4);
        break;
    }
    pthread_cond_signal(&filaVazia); //apos add uma funcao na fila, sinaliza que a fila nao esta vazia
  }
  pthread_mutex_unlock(&mutex_fila); //destrava apos alteracao (add funcao) a fila
  pthread_exit(NULL);
}

void *threads_escalonadas(void *tid){
  int id_thread = *((int *)tid);
  srand(id_thread);
  int inputFun = rand() % 50; //input aleatorio para a funcao
  pthread_mutex_lock(&mutex_fila); //trava a fila para alteracoes (dequeue)
  funcPtr func = dequeue(&fila);
  printf("thread %d, input %d = %d\n", id_thread, inputFun, func(inputFun));
  pthread_mutex_unlock(&mutex_fila); //destrava apos manipulacoes (dequeue) na fila

  pthread_mutex_lock(&mutex_acao); //trava pra alterar a variavel de threadsSimultaneas, indicando que uma thread que estava em uso acabou
  threadsSimultaneas--;
  pthread_mutex_unlock(&mutex_acao); //destrava apos a alteracao

  pthread_cond_signal(&maxThreads); //apos o fim de uma thread, sinaliza que o numero de threads sendo usadas nao eh mais o numero maximo
  pthread_exit(NULL);
}

void *escalonar(){
  pthread_t threads_trabalhadoras[NUM_FUNCOES];
  int *thread_ids[NUM_FUNCOES];

  for(int i = 0; i < NUM_FUNCOES; i++){
    while(fila.size == 0){
      pthread_cond_wait(&filaVazia, &mutex_fila); //se a fila estiver vazia, espera um sinal indicando que foi add um item na fila
    }
    thread_ids[i] = (int *) malloc(sizeof(int));
    *thread_ids[i] = i;
    pthread_create(&threads_trabalhadoras[i], NULL, threads_escalonadas, (void *) thread_ids[i]);
    
    pthread_mutex_lock(&mutex_acao); //trava para alterar a variavel de threadsSimultaneas, indicando que mais uma thread esta em uso
    threadsSimultaneas++;
    while(threadsSimultaneas >= NUM_THREADS){
      pthread_cond_wait(&maxThreads, &mutex_acao); //espera a liberacao caso o numero maximo de threads esteja sendo executado
    }
    pthread_mutex_unlock(&mutex_acao); //destrava apos a alteracao
  }
  pthread_exit(NULL);
}

int main(){
  // printf("Qual o numero de threads?\n");
  // scanf("%d", &num_threads);
  criarFila(&fila);

  // int (*funcArray[4])(int);

  // funcArray[0] = fun1;
  // funcArray[1] = fun2;
  // funcArray[2] = fun3;
  // funcArray[3] = fun3;

  
  pthread_t enlistador;
  pthread_t escalonador;
  
  pthread_create(&enlistador, NULL, enlistar, NULL);
  pthread_create(&escalonador, NULL, escalonar, NULL);

  pthread_join(escalonador, NULL);
  pthread_join(enlistador, NULL);
  
  pthread_exit(NULL);
  return 0;
}