#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 10
#define NUM_THREADS 2

int array[ARRAY_SIZE] = {5, 2, 4, 7, 1, 3, 2, 6, 9, 8};
pthread_barrier_t barrier; // Barreira para sincronização

void *bubbleSort(void *arg)
{
    int thread_id = *(int *)arg;

    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = (thread_id + 1) * (ARRAY_SIZE / NUM_THREADS);
    if (thread_id == NUM_THREADS - 1)
        end = ARRAY_SIZE;

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

    pthread_barrier_wait(&barrier); // Sincroninzando todas as threads para que a thread final possa mesclar o resultado
    pthread_exit(NULL);
}

void *merge(void *arg)
{
    pthread_barrier_wait(&barrier); // a thread final entra na funcao mas ja é colocada rapidamente a espera das outras

    int array_aux[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array_aux[i] = array[i];
    }

    int left = 0;
    int right = ARRAY_SIZE / NUM_THREADS;

    int merged[ARRAY_SIZE];
    int k = 0;
    while (left < ARRAY_SIZE / NUM_THREADS && right < ARRAY_SIZE)
    {
        if (array_aux[left] <= array_aux[right])
        {
            merged[k++] = array_aux[left++];
        }
        else
        {
            merged[k++] = array_aux[right++];
        }
    }
    while (left < ARRAY_SIZE / NUM_THREADS)
    {
        merged[k++] = array_aux[left++];
    }
    while (right < ARRAY_SIZE)
    {
        merged[k++] = array_aux[right++];
    }

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = merged[i];
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS + 1];
    int thread_ids[NUM_THREADS];

    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, bubbleSort, &thread_ids[i]);
    }

    pthread_create(&threads[NUM_THREADS], NULL, merge, NULL); // criando a thread de mesclar fora do loop das thread de ordenar

    //for (int i = 0; i <= NUM_THREADS; i++)
    //{
    //    pthread_join(threads[i], NULL);
    //}
    pthread_join(threads[NUM_THREADS], NULL);

    printf("Array ordenado: ");
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
    //return 0;
}
