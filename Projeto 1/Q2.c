#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arraySize;  // tamanho do array
int numThread;  // quantidade de threads
int *array;
pthread_barrier_t barrier; // Barreira para sincronização


void* bubbleSort(void* arg);
void* merge(void* arg);


int main() 
{
   printf("Por favor digite o numero de threads que dejesa utilizar\n");
   scanf("%d", &numThread);
   printf("Por favor digite o tamanho do array a ser montado\n");
   scanf("%d", &arraySize);
  
   array = malloc(arraySize * sizeof(int));
   if (array == NULL)
   {
       printf("Erro de alocacao de memoria\n");
       return 1;
   }
   for(int i = 0; i<arraySize ;i++)
   {
       printf("Por favor digite o valor da posicao %d do array\n",i);
       scanf("%d",&array[i]);
   }

   pthread_t threads[numThread + 1];
   int thread_ids[numThread];

   // Inicializa a barreira dinamicamente
   pthread_barrier_init(&barrier, NULL, numThread + 1);

   // Cria as threads de bubbleSort
   for (int i = 0; i < numThread; i++)
   {
       thread_ids[i] = i;
       int rc = pthread_create(&threads[i], NULL, bubbleSort, &thread_ids[i]);
       if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
   }

   // Cria a thread de merge
   pthread_create(&threads[numThread], NULL, merge, NULL);

   // Espera todas as threads terminarem a execucao
   for (int i = 0; i <= numThread; i++)
   {
       pthread_join(threads[i], NULL);
   }

   printf("Array ordenado: ");
   for (int i = 0; i < arraySize; i++)
   {
       printf("%d ", array[i]);
   }
   printf("\n");

   pthread_barrier_destroy(&barrier);
   free(array);
   pthread_exit(NULL);
   return 0;
}

void* bubbleSort(void* arg)
{
   int thread_id = *(int*)arg;
   int start = thread_id * (arraySize / numThread);
   int end = (thread_id + 1) * (arraySize / numThread);
   if (thread_id == numThread - 1) end = arraySize;

   for (int i = start; i < end - 1; i++)
   {
       for (int j = start; j < end - (i - start) - 1; j++)
       {
           if (array[j] > array[j + 1])
           {
               int temp = array[j];
               array[j] = array[j + 1];
               array[j + 1] = temp;
           }
       }
   }

   // Sincronizando todas as threads para que a thread final possa mesclar o resultado
   pthread_barrier_wait(&barrier);
   pthread_exit(NULL);
}

void* merge(void* arg)
{
   // a thread final entra na funcao mas ja é colocada rapidamente a espera das outras
   pthread_barrier_wait(&barrier);

   int array_aux[arraySize];
   for (int i = 0; i < arraySize; i++)
   {
       array_aux[i] = array[i];
   }

   int cont_array[numThread];
   int ponto_maximo[numThread];
   int ponto_de_quebra = arraySize / numThread;

   for (int i = 0; i < numThread; i++)
   {
       cont_array[i] = i * ponto_de_quebra;
       if (i == numThread - 1)
       {
           ponto_maximo[i] = arraySize;
       }
       else
       {
           ponto_maximo[i] = (i + 1) * ponto_de_quebra;
       }
   }
   int menor;
   for (int i = 0; i < arraySize; i++)
   {
       int indice;
       menor = 9999999;
       for (int j = 0; j < numThread; j++)
       {
           if (cont_array[j] != ponto_maximo[j] && array_aux[cont_array[j]] < menor)
           {
               menor = array_aux[cont_array[j]];
               indice = j;
           }
       }
       array[i] = menor;
       cont_array[indice]++;
   }
   pthread_exit(NULL);
}