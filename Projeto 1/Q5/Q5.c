#define _XOPEN_SOURCE 600
#define numVar 3//Numero de linhas/colunas na matriz quadrada bem como o numeros de "X" que eh preciso encontrar
#define numIteracoes 100 //Numero de aproximacoes do algoritmo
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float matriz_base[numVar][numVar] = {{6.0f,4.0f,1.0f},
                                     {4.0f,7.0f,2.0f},
                                     {1.0f,2.0f,4.0f}};

float vetor_resultados[numVar]    = {25.0f, 36.0f,14.0f};
float vetor_variveis[numVar]      = {1.0f, 1.0f,1.0f};   // inicializando o erro como 1
float proximo_X[numVar];
int NUM_THREADS;

pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

void *jacobi(void * arg);

int main() 
{
  printf("Por favor digite o numero de threads a serem utilizadas\n");
  scanf("%d", &NUM_THREADS);
  pthread_t *arrayThreads = (pthread_t *) malloc(NUM_THREADS * (sizeof(pthread_t)));
  int IDS[NUM_THREADS];
  // alocacao dinamica das barreiras
  pthread_barrier_init(&barrier1, NULL, NUM_THREADS);
  pthread_barrier_init(&barrier2, NULL, NUM_THREADS);

  for(int i = 0; i < NUM_THREADS; i++)
  {
    IDS[i] = i;
    int rc = pthread_create(&arrayThreads[i], NULL, jacobi, ((void*) &IDS[i]));
    if (rc) { printf("Erro na criacao da thread %d\n", i); exit(1); }
  }

  for(int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(arrayThreads[i], NULL);
  }

  for(int i = 0; i < numVar; i++)
  {
    printf("x%d: %f\n", i, vetor_variveis[i]);
  }

  // liberacao da memoria alocada dinamicamente
  pthread_barrier_destroy(&barrier1);
  pthread_barrier_destroy(&barrier2);
  pthread_exit(NULL);
  return 0;
}

void *jacobi(void *arg)
{
  int id = *((int *) arg);
  int k = 1;               // Começa da 1 iteracao assumindo o valor da variavel sendo 1
  while(k <= numIteracoes) // Sendo k a iteracao atual e p eh o numero de iteracoes totais 
  {  
    for (int i = id; i < numVar; i += NUM_THREADS)
    {
      // calculando o somatorio
      float soma = 0;
      for(int j = 0; j < numVar; j++)
      {
        if(i == j) continue;
        soma += (matriz_base[i][j]*vetor_variveis[j]);
      }

      // definindo o proximoX
      proximo_X[i] = (vetor_resultados[i] - soma)/matriz_base[i][i];
    }

    pthread_barrier_wait(&barrier1);

    for (int i = id; i < numVar; i += NUM_THREADS)
    {
      vetor_variveis[i] = proximo_X[i];
    }
    pthread_barrier_wait(&barrier2);
    k++;
  }
  pthread_exit(NULL);
}