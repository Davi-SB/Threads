#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_CLIENTES 5
#define NUM_OPS 20

typedef struct conta{
  int id_cliente, saldo;
} Conta;

pthread_mutex_t mutex_banco = PTHREAD_MUTEX_INITIALIZER;
Conta *contas[NUM_CLIENTES];

typedef struct escolhas{
  int op, id_conta;
} Escolha;

void *saldo(void *thread_id){
  int tid = *((int *)thread_id);
  pthread_mutex_lock(&mutex_banco);
  printf("Cliente %d possui %d de saldo\n", tid, contas[tid]->saldo);
  pthread_mutex_unlock(&mutex_banco);
  pthread_exit(NULL);
}

void *deposito(void *thread_id){
  int tid = *((int *)thread_id);
  pthread_mutex_lock(&mutex_banco);
  srand(time(NULL));
  int valor_deposito = rand() % 400; //valor aleatorio para deposito, poderia ser um scanf para o cliente digitar um valor
  contas[tid]->saldo += valor_deposito;
  printf("Cliente %d depositou %d Saldo C%d: %d\n", tid, valor_deposito, tid, contas[tid]->saldo);
  pthread_mutex_unlock(&mutex_banco);
  pthread_exit(NULL);
}

void *saque(void *thread_id){
  int tid = *((int *)thread_id);
  pthread_mutex_lock(&mutex_banco);
  srand(time(NULL));
  int valor_saque = rand() % 400; //valor aleatorio para saque, poderia ser um scanf para o cliente digitar um valor
  contas[tid]->saldo -= valor_saque;
  printf("Cliente %d sacou %d Saldo C%d: %d\n", tid, valor_saque, tid, contas[tid]->saldo);
  pthread_mutex_unlock(&mutex_banco);
  pthread_exit(NULL);
}

void *bancos(){
  pthread_t clientes[NUM_CLIENTES];
  Escolha operacoes[NUM_OPS];

  for(int i = 0; i < NUM_CLIENTES; i++){
    contas[i] = (Conta *) malloc (sizeof(Conta));
    contas[i]->id_cliente = i;
    contas[i]->saldo = 0;
  }

  for (int i = 0; i < NUM_OPS; i++) {
    operacoes[i].op = rand() % 3;
    operacoes[i].id_conta = rand() % NUM_CLIENTES;
  
    switch(operacoes[i].op){
      case(0):
        pthread_create(&clientes[operacoes[i].id_conta], NULL, saldo, &operacoes[i].id_conta);
        break;
      
      case(1):
        pthread_create(&clientes[operacoes[i].id_conta], NULL, deposito, &operacoes[i].id_conta);
        break;

      case(2):
        pthread_create(&clientes[operacoes[i].id_conta], NULL, saque, &operacoes[i].id_conta);
        break;
    }
  }

  for (int i = 0; i < NUM_CLIENTES; i++) {
    pthread_join(clientes[i], NULL);
  }

  for (int i = 0; i < NUM_CLIENTES; i++) {
    free(contas[i]);
  }
  pthread_exit(NULL);
}

int main(){
  pthread_t banco;
  pthread_create(&banco, NULL, bancos, NULL);
  pthread_join(banco, NULL);
  pthread_exit(NULL);
  return 0;
}